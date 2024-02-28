#include "shell.h"

/**
 * shell_strtow - splits a string into words. Repeat delimiters are ignored
 * @str: the input string
 * @delim: the delimiter string
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **shell_strtow(char *str, char *delim)
{
	int i, j, k, m, num_words = 0;
	char **words;

	if (str == NULL || str[0] == 0)
		return (NULL);
	if (!delim)
		delim = " ";
	for (i = 0; str[i] != '\0'; i++)
		if (!shell_is_delim(str[i], delim) && (shell_is_delim(str[i + 1], delim) || !str[i + 1]))
			num_words++;

	if (num_words == 0)
		return (NULL);
	words = malloc((1 + num_words) * sizeof(char *));
	if (!words)
		return (NULL);
	for (i = 0, j = 0; j < num_words; j++)
	{
		while (shell_is_delim(str[i], delim))
			i++;
		k = 0;
		while (!shell_is_delim(str[i + k], delim) && str[i + k])
			k++;
		words[j] = malloc((k + 1) * sizeof(char));
		if (!words[j])
		{
			for (k = 0; k < j; k++)
				free(words[k]);
			free(words);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			words[j][m] = str[i++];
		words[j][m] = 0;
	}
	words[j] = NULL;
	return (words);
}

/**
 * shell_strtow2 - splits a string into words
 * @str: the input string
 * @delim: the delimiter
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **shell_strtow2(char *str, char delim)
{
	int i, j, k, m, num_words = 0;
	char **words;

	if (str == NULL || str[0] == 0)
		return (NULL);
	for (i = 0; str[i] != '\0'; i++)
		if ((str[i] != delim && str[i + 1] == delim) ||
		    (str[i] != delim && !str[i + 1]) || str[i + 1] == delim)
			num_words++;
	if (num_words == 0)
		return (NULL);
	words = malloc((1 + num_words) * sizeof(char *));
	if (!words)
		return (NULL);
	for (i = 0, j = 0; j < num_words; j++)
	{
		while (str[i] == delim && str[i] != delim)
			i++;
		k = 0;
		while (str[i + k] != delim && str[i + k] && str[i + k] != delim)
			k++;
		words[j] = malloc((k + 1) * sizeof(char));
		if (!words[j])
		{
			for (k = 0; k < j; k++)
				free(words[k]);
			free(words);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			words[j][m] = str[i++];
		words[j][m] = 0;
	}
	words[j] = NULL;
	return (words);
}

