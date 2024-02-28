#include "shell.h"

/**
 * shell_hsh - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int shell_hsh(shell_info_t *info, char **av)
{
    ssize_t bytes_read = 0;
    int builtin_ret = 0;

    while (bytes_read != -1 && builtin_ret != -2)
    {
        shell_clear_info(info);
        if (shell_interactive(info))
            shell_puts("$ ");
        shell_eputchar(SHELL_BUF_FLUSH);
        bytes_read = shell_get_input(info);
        if (bytes_read != -1)
        {
            shell_set_info(info, av);
            builtin_ret = shell_find_builtin(info);
            if (builtin_ret == -1)
                shell_find_cmd(info);
        }
        else if (shell_interactive(info))
            shell_putchar('\n');
        shell_free_info(info, 0);
    }
    shell_write_history(info);
    shell_free_info(info, 1);
    if (!shell_interactive(info) && info->status)
        exit(info->status);
    if (builtin_ret == -2)
    {
        if (info->err_num == -1)
            exit(info->status);
        exit(info->err_num);
    }
    return (builtin_ret);
}

/**
 * shell_find_builtin - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int shell_find_builtin(shell_info_t *info)
{
    int i, builtin_ret = -1;
    shell_builtin_table builtintbl[] = {
        {"exit", shell_myexit},
        {"env", shell_myenv},
        {"help", shell_myhelp},
        {"history", shell_myhistory},
        {"setenv", shell_mysetenv},
        {"unsetenv", shell_myunsetenv},
        {"cd", shell_mycd},
        {"alias", shell_myalias},
        {NULL, NULL}
    };

    for (i = 0; builtintbl[i].type; i++)
        if (shell_strcmp(info->argv[0], builtintbl[i].type) == 0)
        {
            info->line_count++;
            builtin_ret = builtintbl[i].func(info);
            break;
        }
    return (builtin_ret);
}

/**
 * shell_find_cmd - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void shell_find_cmd(shell_info_t *info)
{
    char *path = NULL;
    int i, k;

    info->path = info->argv[0];
    if (info->linecount_flag == 1)
    {
        info->line_count++;
        info->linecount_flag = 0;
    }
    for (i = 0, k = 0; info->arg[i]; i++)
        if (!shell_is_delim(info->arg[i], " \t\n"))
            k++;
    if (!k)
        return;

    path = shell_find_path(info, shell_getenv(info, "PATH="), info->argv[0]);
    if (path)
    {
        info->path = path;
        shell_fork_cmd(info);
    }
    else
    {
        if ((shell_interactive(info) || shell_getenv(info, "PATH=")
             || info->argv[0][0] == '/') && shell_is_cmd(info, info->argv[0]))
            shell_fork_cmd(info);
        else if (*(info->arg) != '\n')
        {
            info->status = 127;
            shell_print_error(info, "not found\n");
        }
    }
}

/**
 * shell_fork_cmd - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void shell_fork_cmd(shell_info_t *info)
{
    pid_t child_pid;

    child_pid = fork();
    if (child_pid == -1)
    {
        /* TODO: PUT ERROR FUNCTION */
        perror("Error:");
        return;
    }
    if (child_pid == 0)
    {
        if (execve(info->path, info->argv, shell_get_environ(info)) == -1)
        {
            shell_free_info(info, 1);
            if (errno == EACCES)
                exit(126);
            exit(1);
        }
        /* TODO: PUT ERROR FUNCTION */
    }
    else
    {
        wait(&(info->status));
        if (WIFEXITED(info->status))
        {
            info->status = WEXITSTATUS(info->status);
            if (info->status == 126)
                shell_print_error(info, "Permission denied\n");
        }
    }
}
