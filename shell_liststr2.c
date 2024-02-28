#include "shell.h"

/**
 * shell_list_len - determines length of linked list
 * @head: pointer to first node
 *
 * Return: size of list
 */
size_t shell_list_len(const shell_list_t *head)
{
	size_t count = 0;

	while (head)
	{
		head = head->next;
		count++;
	}
	return (count);
}

/**
 * shell_list_to_strings - returns an array of strings of the list->str
 * @head: pointer to first node
 *
 * Return: array of strings
 */
char **shell_list_to_strings(shell_list_t *head)
{
	shell_list_t *node = head;
	size_t size = shell_list_len(head), i;
	char **strings;
	char *str;

	if (!head || !size)
		return (NULL);
	strings = malloc(sizeof(char *) * (size + 1));
	if (!strings)
		return (NULL);
	for (i = 0; node; node = node->next, i++)
	{
		str = malloc(shell_strlen(node->str) + 1);
		if (!str)
		{
			for (size_t j = 0; j < i; j++)
				free(strings[j]);
			free(strings);
			return (NULL);
		}
		str = shell_strcpy(str, node->str);
		strings[i] = str;
	}
	strings[i] = NULL;
	return (strings);
}


/**
 * shell_print_list - prints all elements of a shell_list_t linked list
 * @head: pointer to first node
 *
 * Return: size of list
 */
size_t shell_print_list(const shell_list_t *head)
{
	size_t count = 0;

	while (head)
	{
		shell_puts(convert_number(head->num, 10, 0));
		shell_putchar(':');
		shell_putchar(' ');
		shell_puts(head->str ? head->str : "(nil)");
		shell_puts("\n");
		head = head->next;
		count++;
	}
	return (count);
}

/**
 * shell_node_starts_with - returns node whose string starts with prefix
 * @head: pointer to list head
 * @prefix: string to match
 * @c: the next character after prefix to match
 *
 * Return: match node or null
 */
shell_list_t *shell_node_starts_with(shell_list_t *head, char *prefix, char c)
{
	char *p = NULL;

	while (head)
	{
		p = shell_starts_with(head->str, prefix);
		if (p && ((c == -1) || (*p == c)))
			return (head);
		head = head->next;
	}
	return (NULL);
}

/**
 * shell_get_node_index - gets the index of a node
 * @head: pointer to list head
 * @node: pointer to the node
 *
 * Return: index of node or -1
 */
ssize_t get_node_index(shell_list_t *head, shell_list_t *node)
{
	size_t index = 0;

	while (head)
	{
		if (head == node)
			return (index);
		head = head->next;
		index++;
	}
	return (-1);
}

