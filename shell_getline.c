#include "shell.h"

/**
 * buffer_commands - buffers chained commands
 * @info: parameter struct
 * @buffer: address of buffer
 * @length: address of length var
 *
 * Return: bytes read
 */
ssize_t buffer_commands(info_t *info, char **buffer, size_t *length)
{
	ssize_t bytes_read = 0;
	size_t len_pointer = 0;

	if (!*length) /* if nothing left in the buffer, fill it */
	{
		free(*buffer);
		*buffer = NULL;
		signal(SIGINT, shell_sigintHandler);
#if USE_GETLINE
		bytes_read = getline(buffer, &len_pointer, stdin);
#else
		bytes_read = _getline(info, buffer, &len_pointer);
#endif
		if (bytes_read > 0)
		{
			if ((*buffer)[bytes_read - 1] == '\n')
			{
				(*buffer)[bytes_read - 1] = '\0'; /* remove trailing newline */
				bytes_read--;
			}
			info->linecount_flag = 1;
			remove_comments(*buffer);
			build_history_list(info, *buffer, info->histcount++);
			/* if (_strchr(*buffer, ';')) is this a command chain? */
			{
				*length = bytes_read;
				info->cmd_buf = buffer;
			}
		}
	}
	return (bytes_read);
}

/**
 * obtain_input - gets a line minus the newline
 * @info: parameter struct
 *
 * Return: bytes read
 */
ssize_t obtain_input(info_t *info)
{
	static char *buffer; /* the ';' command chain buffer */
	static size_t start, end, length;
	ssize_t bytes_read = 0;
	char **buffer_pointer = &(info->arg), *pointer;

	_putchar(BUF_FLUSH);
	bytes_read = buffer_commands(info, &buffer, &length);
	if (bytes_read == -1) /* EOF */
		return (-1);
	if (length)	/* we have commands left in the chain buffer */
	{
		end = start; /* init new iterator to current buffer position */
		pointer = buffer + start; /* get pointer for return */

		check_chain(info, buffer, &end, start, length);
		while (end < length) /* iterate to semicolon or end */
		{
			if (is_chain(info, buffer, &end))
				break;
			end++;
		}

		start = end + 1; /* increment past nulled ';'' */
		if (start >= length) /* reached end of buffer? */
		{
			start = length = 0; /* reset position and length */
			info->cmd_buf_type = CMD_NORM;
		}

		*buffer_pointer = pointer; /* pass back pointer to current command position */
		return (_strlen(pointer)); /* return length of current command */
	}

	*buffer_pointer = buffer; /* else not a chain, pass back buffer from _getline() */
	return (bytes_read); /* return length of buffer from _getline() */
}

/**
 * read_buffer - reads a buffer
 * @info: parameter struct
 * @buffer: buffer
 * @size: size
 *
 * Return: bytes read
 */
ssize_t read_buffer(info_t *info, char *buffer, size_t *size)
{
	ssize_t bytes_read = 0;

	if (*size)
		return (0);
	bytes_read = read(info->readfd, buffer, READ_BUF_SIZE);
	if (bytes_read >= 0)
		*size = bytes_read;
	return (bytes_read);
}

/**
 * _getline - gets the next line of input from STDIN
 * @info: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: bytes read
 */
int _getline(info_t *info, char **ptr, size_t *length)
{
	static char buffer[READ_BUF_SIZE];
	static size_t start, end;
	size_t k;
	ssize_t bytes_read = 0, chars_read = 0;
	char *pointer = NULL, *new_pointer = NULL, *character;

	pointer = *ptr;
	if (pointer && length)
		chars_read = *length;
	if (start == end)
		start = end = 0;

	bytes_read = read_buffer(info, buffer, &end);
	if (bytes_read == -1 || (bytes_read == 0 && end == 0))
		return (-1);

	character = _strchr(buffer + start, '\n');
	k = character ? 1 + (unsigned int)(character - buffer) : end;
	new_pointer = _realloc(pointer, chars_read, chars_read ? chars_read + k : k + 1);
	if (!new_pointer) /* MALLOC FAILURE! */
		return (pointer ? free(pointer), -1 : -1);

	if (chars_read)
		_strncat(new_pointer, buffer + start, k - start);
	else
		_strncpy(new_pointer, buffer + start, k - start + 1);

	chars_read += k - start;
	start = k;
	pointer = new_pointer;

	if (length)
		*length = chars_read;
	*ptr = pointer;
	return (chars_read);
}

/**
 * shell_sigintHandler - blocks ctrl-C
 * @sig_num: the signal number
 *
 * Return: void
 */
void shell_sigintHandler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}

