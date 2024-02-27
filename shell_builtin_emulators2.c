#include "shell.h"

/**
 * my_history - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int my_history(shell_info_t *info)
{
    /* Print the history list */
    print_list(info->history);
    return (0);
}

/**
 * unset_alias - removes an alias from the alias list
 * @info: parameter struct
 * @str: the string representing the alias
 *
 * Return: Always 0 on success, 1 on error
 */
int unset_alias(shell_info_t *info, char *str)
{
    char *p, c;
    int ret;

    p = shell_strchr(str, '=');
    if (!p)
        return (1);
    c = *p;
    *p = 0;
    ret = delete_node_at_index(&(info->alias),
                               get_node_index(info->alias, shell_node_starts_with(info->alias, str, -1)));
    *p = c;
    return (ret);
}

/**
 * set_alias - sets an alias in the alias list
 * @info: parameter struct
 * @str: the string representing the alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias(shell_info_t *info, char *str)
{
    char *p;

    p = shell_strchr(str, '=');
    if (!p)
        return (1);
    if (!*++p)
        return (unset_alias(info, str));

    unset_alias(info, str);
    return (add_node_end(&(info->alias), str, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int printt_alias(shell_list_t *node)
{
    char *p = NULL, *a = NULL;

    if (node)
    {
        p = shell_strchr(node->str, '=');
        for (a = node->str; a <= p; a++)
            shell_putchar(*a);
        shell_putchar('\'');
        shell_puts(p + 1);
        shell_puts("'\n");
        return (0);
    }
    return (1);
}

/**
 * my_alias - mimics the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int shell_myalias(shell_info_t *);
{
    int i = 0;
    char *p = NULL;
    shell_list_t *node = NULL;

    if (info->argc == 1)
    {
        node = info->alias;
        while (node)
        {
            printt_alias(node);
            node = node->next;
        }
        return (0);
    }
    for (i = 1; info->argv[i]; i++)
    {
        p = shell_strchr(info->argv[i], '=');
        if (p)
            set_alias(info, info->argv[i]);
        else
            printt_alias(shell_node_starts_with(info->alias, info->argv[i], '='));
    }

    return (0);
}

