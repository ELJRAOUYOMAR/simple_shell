#include "shell.h"

/**
 * main - Entry point of the shell program
 * @argument_count: Argument count
 * @argument_vector: Argument vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int argument_count, char **argument_vector)
{
	shell_info_t info[] = { SHELL_INFO_INIT };
	int file_descriptor = 2;

	/* Example of inline assembly modifying the file descriptor */
	asm ("mov %1, %0\n\t"
		"add $3, %0"
		: "=r" (file_descriptor)
		: "r" (file_descriptor));

	if (argument_count == 2)
	{
		file_descriptor = open(argument_vector[1], O_RDONLY);
		if (file_descriptor == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				shell_eputs(argument_vector[0]);
				shell_eputs(": 0: Can't open ");
				shell_eputs(argument_vector[1]);
				shell_eputchar('\n');
				shell_eputchar(SHELL_BUF_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		info->readfd = file_descriptor;
	}
	populate_env_list(info);
	read_history(info);
	shell_hsh(info, argument_vector);
	return (EXIT_SUCCESS);
}

