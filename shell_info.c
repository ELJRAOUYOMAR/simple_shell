#include "shell.h"

/**
 * shell_clear_info - initializes shell_info_t struct
 * @info: struct address
 */
void shell_clear_info(shell_info_t *info)
{
    info->argv = NULL;
    info->path = NULL;
    info->argc = 0;
}

/**
 * shell_set_info - initializes shell_info_t struct
 * @info: struct address
 * @av: argument vector
 */
void shell_set_info(shell_info_t *info, char **av)
{
    int i = 0;

    info->fname = av[0];
    if (info->arg)
    {
        info->argv = shell_strtow(info->arg, " \t");
        if (!info->argv)
        {
            info->argv = malloc(sizeof(char *) * 2);
            if (info->argv)
            {
                info->argv[0] = shell_strdup(info->arg);
                info->argv[1] = NULL;
            }
        }
        for (i = 0; info->argv && info->argv[i]; i++)
            ;
        info->argc = i;
    }
}

/**
 * shell_free_info - frees shell_info_t struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void shell_free_info(shell_info_t *info, int all)
{
    shell_ffree(info->argv);
    info->argv = NULL;
    info->path = NULL;
    if (all)
    {
        if (!info->cmd_buf)
            free(info->arg);
        shell_free_list(&(info->env));
        shell_free_list(&(info->history));
        shell_free_list(&(info->alias));
        shell_ffree(info->environ);
        info->environ = NULL;
        shell_bfree((void **)&info->cmd_buf);
        if (info->readfd > 2)
            close(info->readfd);
        shell_putchar(SHELL_BUF_FLUSH);
    }
}
