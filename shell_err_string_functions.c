#include "shell.h"

/**
 * _eputs - prints an input string to standard error
 * @err_str: the string to be printed
 *
 * Return: Nothing
 */
void shell__eputs(char *err_str)
{
	int index = 0;

	if (!err_str)
		return;
	while (err_str[index] != '\0')
	{
		shell_eputchar(err_str[index]);
		index++;
	}
}

/**
 * _eputchar - writes the character to standard error
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int shell__eputchar(char c)
{
	static int index;
	static char buffer[SHELL_WRITE_BUF_SIZE];

	if (c == SHELL_BUF_FLUSH || index >= SHELL_WRITE_BUF_SIZE)
	{
		write(2, buffer, index);
		index = 0;
	}
	if (c != SHELL_BUF_FLUSH)
		buffer[index++] = c;
	return (1);
}

/**
 * _putfd - writes the character to given file descriptor
 * @c: The character to print
 * @fd: The file descriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int shell__putfd(char c, int fd)
{
	static int index;
	static char buffer[SHELL_WRITE_BUF_SIZE];

	if (c == SHELL_BUF_FLUSH || index >= SHELL_WRITE_BUF_SIZE)
	{
		write(fd, buffer, index);
		index = 0;
	}
	if (c != SHELL_BUF_FLUSH)
		buffer[index++] = c;
	return (1);
}

/**
 * _putsfd - prints an input string to the specified file descriptor
 * @str: the string to be printed
 * @fd: the file descriptor to write to
 *
 * Return: the number of characters put
 */
int shell__putsfd(char *str, int fd)
{
	int count = 0;

	if (!str)
		return (0);
	while (*str)
	{
		count += shell_putfd(*str++, fd);
	}
	return (count);
}

