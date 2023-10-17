#ifndef _STR_TOOLS_H
#define _STR_TOOLS_H

#include "root.h"

/**
 * _strlen - len of str
 * @s: string
 *
 * Return: len of str
 */
__home int _strlen(char *s)
{
	int i = 0;

	while (*(s + i) != '\0')
	{
		i++;
	}

	return (i);
}

/**
 * _strncmp - Compare two strings with a specific length
 * @s1: String 1
 * @s2: String 2
 * @len: Length
 *
 * Return: Difference of characters (0 - Equal)
 */
__home int _strncmp(char *s1, char *s2, int len)
{
	int i = 1;

	while (i <= len)
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		s1++;
		s2++;
		i++;
	}
	return (0);
}

/**
 * _strcmp - Compare two strings
 * @s1: String 1
 * @s2: String 2
 *
 * Return: Difference of characters (0 - Equal)
 */
__home int _strcmp(char *s1, char *s2)
{
	while (*s1 == *s2)
	{
		if (*s1 == '\0')
			return (0);
		s1++;
		s2++;
	}

	return (*s1 - *s2);
}

/**
 * _strconcat - Copies a string and returns a pointer to the array.
 * @s1: String 1
 * @s2: String 2
 *
 * Return: Pointer to the array or NULL.
 */

__home char *_strconcat(char *s1, char *s2)
{
	int i = 0, j = 0, k = 0, n = 0;
	char *a;

	if (s1 == NULL)
		s1 = "";
	if (s2 == NULL)
		s2 = "";
	while (*(s1 + i))
		i++;

	while (*(s2 + j))
		j++;

	a = malloc(sizeof(char) * (i + j + 3));
	if (a == NULL)
		return (NULL);

	for (k = 0; k < i; k++)
	{
		*(a + k) = *(s1 + k);
	}

	for (n = 0; n <= j; n++)
	{
		*(a + n + k) = *(s2 + n);
	}

	return (a);
}

/**
 * _strdup - returns a pointer to a newly allocated space
 * in memory, which contains a copy of the string given
 * as a parameter.
 *
 * @str: string
 *
 * Return: pointer to space in memory or NULL
 */
__home char *_strdup(char *str)
{
	int len, i;
	char *s;

	if (!str)
		return (NULL);

	len = _strlen(str);
	s = malloc(sizeof(char) * len + 1);

	if (!s)
		return (NULL);

	for (i = 0; i < len; i++)
		s[i] = str[i];

	s[i] = END;

	return (s);
}

#endif
