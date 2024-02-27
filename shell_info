#include "shell.h"

/**
 * shell_clear_info - initializes shell_info_t struct
 * @info: struct address
 */
void shell_clear_info(shell_info_t *info)
{
	info->arguments = NULL;
	info->argument_vector = NULL;
	info->path = NULL;
	info->argument_count = 0;
}

/**
 * shell_set_info - initializes shell_info_t struct
 * @info: struct address
 * @av: argument vector
 */
void shell_set_info(shell_info_t *info, char **av)
{
	int i = 0;

	info->file_name = av[0];
	if (info->arguments)
	{
		info->argument_vector = strtow(info->arguments, " \t");
		if (!info->argument_vector)
		{

			info->argument_vector = malloc(sizeof(char *) * 2);
			if (info->argument_vector)
			{
				info->argument_vector[0] = _strdup(info->arguments);
				info->argument_vector[1] = NULL;
			}
		}
		for (i = 0; info->argument_vector && info->argument_vector[i]; i++)
			;
		info->argument_count = i;

		replace_alias(info);
		replace_vars(info);
	}
}

/**
 * shell_free_info - frees shell_info_t struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void shell_free_info(shell_info_t *info, int all)
{
	ffree(info->argument_vector);
	info->argument_vector = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->command_buffer)
			free(info->arguments);
		if (info->environment)
			free_list(&(info->environment));
		if (info->history)
			free_list(&(info->history));
		if (info->alias)
			free_list(&(info->alias));
		ffree(info->environ);
			info->environ = NULL;
		bfree((void **)info->command_buffer);
		if (info->read_file_descriptor > 2)
			close(info->read_file_descriptor);
		_putchar(BUF_FLUSH);
	}
}
