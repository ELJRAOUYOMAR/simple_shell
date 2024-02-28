#include "shell.h"

/**
 * get_environ - returns the string array copy of our environ
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: The string array copy of our environ
 */
char **shell_get_environ(shell_info_t *info)
{
	if (!info->environ || info->env_changed)
	{
		info->environ = shell_list_to_strings(info->env);
		info->env_changed = 0;
	}

	return (info->environ);
}

/**
 * _unsetenv - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: The string env var property
 * Return: 1 on delete, 0 otherwise
 */
int shell_unsetenv(shell_info_t *info, char *var)
{
	shell_list_t *node = info->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = shell_starts_with(node->str, var);
		if (p && *p == '=')
		{
			info->env_changed = delete_node_at_index(&(info->env), i);
			i = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (info->env_changed);
}

/**
 * _setenv - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: The string env var property
 * @value: The string env var value
 * Return: Always 0
 */
int shell_setenv(shell_info_t *info, char *var, char *value)
{
	char *buf = NULL;
	shell_list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(shell_strlen(var) + shell_strlen(value) + 2);
	if (!buf)
		return (1);
	shell_strcpy(buf, var);
	shell_strcat(buf, "=");
	shell_strcat(buf, value);
	node = info->env;
	while (node)
	{
		p = shell_starts_with(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			info->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	shell_add_node(&(info->env), buf, 0);
	free(buf);
	info->env_changed = 1;
	return (0);
}

