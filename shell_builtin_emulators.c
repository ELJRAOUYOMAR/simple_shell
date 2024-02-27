#include "shell.h"

/**
 * shell_myexit - exits the shell
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: exits with a given exit status
 *         (0) if info->argv[0] != "exit"
 */
int shell_myexit(shell_info_t *info)
{
	int exitcheck;

	if (info->argv[1])  /* If there is an exit argument */
	{
		exitcheck = shell_erratoi(info->argv[1]);
		if (exitcheck == -1)
		{
			info->status = 2;
			shell_print_error(info, "Illegal number: ");
			shell_eputs(info->argv[1]);
			shell_eputchar('\n');
			return (1);
		}
		info->err_num = shell_erratoi(info->argv[1]);
		return (-2);
	}
	info->err_num = -1;
	return (-2);
}

/**
 * shell_mycd - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int shell_mycd(shell_info_t *info)
{
	char *current_dir, *new_dir, buffer[1024];
	int change_dir_ret;

	current_dir = getcwd(buffer, 1024);
	if (!current_dir)
		shell_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!info->argv[1])
	{
		new_dir = shell_getenv(info, "HOME=");
		if (!new_dir)
			change_dir_ret = /* TODO: handle this case properly */
				chdir((new_dir = shell_getenv(info, "PWD=")) ? new_dir : "/");
		else
			change_dir_ret = chdir(new_dir);
	}
	else if (shell_strcmp(info->argv[1], "-") == 0)
	{
		if (!shell_getenv(info, "OLDPWD="))
		{
			shell_puts(current_dir);
			shell_putchar('\n');
			return (1);
		}
		shell_puts(shell_getenv(info, "OLDPWD=")), shell_putchar('\n');
		change_dir_ret = /* TODO: handle this case properly */
			chdir((new_dir = shell_getenv(info, "OLDPWD=")) ? new_dir : "/");
	}
	else
		change_dir_ret = chdir(info->argv[1]);
	if (change_dir_ret == -1)
	{
		shell_print_error(info, "can't cd to ");
		shell_eputs(info->argv[1]), shell_eputchar('\n');
	}
	else
	{
		shell_setenv(info, "OLDPWD", shell_getenv(info, "PWD="));
		shell_setenv(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * shell_myhelp - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int shell_myhelp(shell_info_t *info)
{
	char **arguments;

	arguments = info->argv;
	shell_puts("help call works. Function not yet implemented \n");
	if (0)
		shell_puts(*arguments); /* temp att_unused workaround */
	return (0);
}

