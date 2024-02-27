#include "shell.h"

/**
 * shell_get_history_file - obtains the history file path
 * @info: parameter struct
 *
 * Return: allocated string containing history file path
 */
char *shell_get_history_file(shell_info_t *info)
{
	char *buffer, *home_dir;

	home_dir = _getenv(info, "HOME=");
	if (!home_dir)
		return (NULL);
	buffer = malloc(sizeof(char) * (_strlen(home_dir) + _strlen(HIST_FILE) + 2));
	if (!buffer)
		return (NULL);
	buffer[0] = 0;
	_strcpy(buffer, home_dir);
	_strcat(buffer, "/");
	_strcat(buffer, HIST_FILE);
	return (buffer);
}

/**
 * shell_write_history - creates a file and writes history to it, or appends to an existing file
 * @info: the parameter struct
 *
 * Return: 1 on success, -1 on failure
 */
int shell_write_history(shell_info_t *info)
{
	ssize_t file_descriptor;
	char *file_name = shell_get_history_file(info);
	list_t *node = NULL;

	if (!file_name)
		return (-1);

	file_descriptor = open(file_name, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(file_name);
	if (file_descriptor == -1)
		return (-1);
	for (node = info->history; node; node = node->next)
	{
		_puts_to_fd(node->str, file_descriptor);
		_put_to_fd('\n', file_descriptor);
	}
	_put_to_fd(BUF_FLUSH, file_descriptor);
	close(file_descriptor);
	return (1);
}

/**
 * shell_read_history - reads history from file
 * @info: the parameter struct
 *
 * Return: hist_count on success, 0 otherwise
 */
int shell_read_history(shell_info_t *info)
{
	int i, last = 0, line_count = 0;
	ssize_t file_descriptor, read_length, file_size = 0;
	struct stat file_stat;
	char *buffer = NULL, *file_name = shell_get_history_file(info);

	if (!file_name)
		return (0);

	file_descriptor = open(file_name, O_RDONLY);
	free(file_name);
	if (file_descriptor == -1)
		return (0);
	if (!fstat(file_descriptor, &file_stat))
		file_size = file_stat.st_size;
	if (file_size < 2)
		return (0);
	buffer = malloc(sizeof(char) * (file_size + 1));
	if (!buffer)
		return (0);
	read_length = read(file_descriptor, buffer, file_size);
	buffer[file_size] = 0;
	if (read_length <= 0)
		return (free(buffer), 0);
	close(file_descriptor);
	for (i = 0; i < file_size; i++)
		if (buffer[i] == '\n')
		{
			buffer[i] = 0;
			shell_build_history_list(info, buffer + last, line_count++);
			last = i + 1;
		}
	if (last != i)
		shell_build_history_list(info, buffer + last, line_count++);
	free(buffer);
	info->hist_count = line_count;
	while (info->hist_count-- >= HIST_MAX)
		delete_node_at_index(&(info->history), 0);
	shell_renumber_history(info);
	return (info->hist_count);
}

/**
 * shell_build_history_list - adds entry to a history linked list
 * @info: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @line_count: the history line count
 *
 * Return: Always 0
 */
int shell_build_history_list(shell_info_t *info, char *buf, int line_count)
{
	list_t *node = NULL;

	if (info->history)
		node = info->history;
	add_node_end(&node, buf, line_count);

	if (!info->history)
		info->history = node;
	return (0);
}

/**
 * shell_renumber_history - renumbers the history linked list after changes
 * @info: Structure containing potential arguments. Used to maintain
 *
 * Return: the new hist_count
 */
int shell_renumber_history(shell_info_t *info)
{
	list_t *node = info->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (info->hist_count = i);
}

