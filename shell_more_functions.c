#include "shell.h"

/**
 * shell_interactive - returns true if shell is in interactive mode
 * @info: pointer to the shell_info_t struct
 *
 * Return: 1 if in interactive mode, 0 otherwise
 */
int shell_interactive(shell_info_t *info)
{
	return (isatty(STDIN_FILENO) && info->readfd <= 2);
}

/**
 * shell_is_delim - checks if character is a delimiter
 * @c: the character to check
 * @delim: the delimiter string
 * Return: 1 if true, 0 if false
 */
int shell_is_delim(char c, char *delim)
{
	while (*delim)
		if (*delim++ == c)
			return (1);
	return (0);
}

/**
 * shell_isalpha - checks for alphabetic character
 * @c: The character to check
 * Return: 1 if c is alphabetic, 0 otherwise
 */
int shell_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	else
		return (0);
}

/**
 * shell_atoi - converts a string to an integer
 * @s: the string to be converted
 * Return: 0 if no numbers in string, converted number otherwise
 */
int shell_atoi(char *s)
{
	int i, sign = 1, flag = 0, output;
	unsigned int result = 0;

	for (i = 0;  s[i] != '\0' && flag != 2; i++)
	{
		if (s[i] == '-')
			sign *= -1;

		if (s[i] >= '0' && s[i] <= '9')
		{
			flag = 1;
			result *= 10;
			result += (s[i] - '0');
		}
		else if (flag == 1)
			flag = 2;
	}

	if (sign == -1)
		output = -result;
	else
		output = result;

	return (output);
}

