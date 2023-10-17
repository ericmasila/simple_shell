#ifndef __GET_LINE_C_H
#define __GET_LINE_C_H

#include "root.h"

#ifndef SSIZE_MAX
# define SSIZE_MAX ((size_t) (SIZE_MAX / 2))
#endif

# undef flockstream
# undef funlockstream
# define flockstream(x) ((void) 0)
# define funlockstream(x) ((void) 0)
# define read_stream(stream) getc(stream)


/**
 * __get_delim - Get user input until a delim is keyed
 * @lineptr: line pointer
 * @n: len
 * @delim: delim
 * @stream: filepointer
 * Return: size or EOF
 */
ssize_t __get_delim(char **lineptr, size_t *n, int delim, FILE *stream)
{	ssize_t result = 0;
	int i = 0;
	char *new_lineptr = NULL;
	size_t needed = 0, len = 0;

	if (!lineptr || !n || !stream)
	{	errno = EINVAL;
		return (EOF);
	} flockstream(stream);
	if (*lineptr == NULL || *n == 0)
	{	*n = 120;
		new_lineptr = (char *) realloc(*lineptr, *n);
		if (!new_lineptr)
		{	result = EOF;
			goto KILL;
		} *lineptr = new_lineptr;
	}
	for (; i != delim ; len++)
	{	i = read_stream(stream);
		if (i == EOF)
		{	result = EOF;
			break;
		}
		if (len + 1 >= *n)
		{	needed = (2 * *n + 1) > SSIZE_MAX ? SSIZE_MAX : (2 * *n + 1);
			if (len + 1 >= needed)
			{	result = EOF;
				errno = EOVERFLOW;
				goto KILL;
			}	new_lineptr = (char *) realloc(*lineptr, needed);
			if (!new_lineptr)
			{	result = EOF;
				goto KILL;
			}	*lineptr = new_lineptr;
			*n = needed;
		}	(*lineptr)[len] = i;
	} (*lineptr)[len] = END;
	result = len ? (ssize_t) len : result;
KILL:	funlockstream(stream);
	return (result);
}

#undef __get_line
/**
 * __get_line - local __get_line function
 * @s: String
 * @n: Len
 * @stream: input source
 * Return: size of text or EOF
 */
__home ssize_t __get_line(char **s, size_t *n, FILE *stream)
{
	return (__get_delim(s, n, '\n', stream));
}

/**
 * __str_tok_r - parse string into tokens
 * @s: String
 * @delim: delim
 * @save_ptr: save pointer
 * Return: string
 */
__home char *__str_tok_r(char *s, const char *delim, char **save_ptr)
{
	char *end;

	if (s == NULL)
		s = *save_ptr;

	if (*s == END)
	{
		*save_ptr = s;
		return (NULL);
	}

	s += strspn(s, delim);
	if (*s == END)
	{
		*save_ptr = s;
		return (NULL);
	}

	end = s + strcspn(s, delim);
	if (*end == END)
	{
		*save_ptr = end;
		return (s);
	}

	*end = END;
	*save_ptr = ++end;
	return (s);
}

#undef __str_tok
/**
 * __str_tok - parse string into tokens
 * @s: string
 * @delim: delimeter
 * Return: tokens
 */
__home char *__str_tok(char *s, const char *delim)
{
	static char *save;

	return (__str_tok_r(s, delim, &save));
}
#endif
