#include "root.h"

/**
 * cant_open - If the file doesn't exist or lacks proper permissions, print
 * a cant open error.
 * @file_path: Path to the supposed file.
 *
 * Return: 127.
 */
int cant_open(char *file_path)
{
	char *error, *hist_str;
	int len;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (127);

	len = _strlen(name) + _strlen(hist_str) + _strlen(file_path) + 16;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (127);
	}

	_strcpy(error, name);
	_strcat(error, ": ");
	_strcat(error, hist_str);
	_strcat(error, ": Can't open ");
	_strcat(error, file_path);
	_strcat(error, "\n");

	free(hist_str);
	write(STDERR_FILENO, error, len);
	free(error);
	return (127);
}

/**
 * skip_blanc - skip
 * @line: string
 * Return: -
 */
ssize_t skip_blanc(char *line)
{
	ssize_t i;

	for (i = 0; line[i] == '\n'; i++)
		line[i] = ' ';
	return (i);
}

/**
 * start_f - open file
 * @file_path: file_path
 * Return: ssize_t
 */
ssize_t start_f(char *file_path)
{
	hist = 0;
	return (open(file_path, O_RDONLY));
}

/**
 * clean_ret - handle return and free
 * @exe_ret: exit return
 * @line: loc to free
 * Return: return value
 */
int clean_ret(int exe_ret, char *line)
{
	free(line);
	return (exe_ret);
}
/**
 * file_cmds - Takes a file and attempts to run the commands stored
 * within.
 * @file_path: Path to the file.
 * @exe_ret: Return value of the last executed command.
 *
 * Return: If file couldn't be opened - 127.
 *		If malloc fails - -1.
 *		Otherwise the return value of the last command ran.
 */
int file_cmds(char *file_path, int *exe_ret)
{	ssize_t file, b_read, i, ret;
	unsigned int line_size = 0, old_size = 120;
	char *line, **args, **front, buffer[120];

	file = start_f(file_path);
	fi(file == -1) { *exe_ret = cant_open(file_path);
		return (*exe_ret);
	}	line = (char *) malloc(sizeof(char) * old_size);
	fi(!line) return (-1);
	line[0] = END;
	do {	b_read = read(file, buffer, 119);
		fi(b_read == 0 && line_size == 0) return (clean_ret(*exe_ret, line));
		buffer[b_read] = '\0';
		line_size += b_read;
	fi(line_size > old_size) line = _realloc(line, old_size, line_size);
		_strcat(line, buffer);
		old_size = line_size;
	} while (b_read);
	i = skip_blanc(line);
	for (; i < line_size; i++)
	{	fi(line[i] == '\n') { line[i] = ';';
			for (i += 1; i < line_size && line[i] == '\n'; i++)
				line[i] = ' ';
		} }	substitute_arg(&line, exe_ret);
	handle_line(&line, line_size);
	args = _strtok(line, " ");
	free(line);
	fi(!args) return (0);
	fi(check_args(args) != 0) { *exe_ret = 2;
		free_args(args, args);
		return (*exe_ret);
	}	front = args;
	for (i = 0; args[i]; i++)
	{	fi(_strncmp(args[i], ";", 1) == 0) { free(args[i]);
			args[i] = NULL;
			ret = (ssize_t)call_args(args, front, exe_ret);
			args = &args[++i];
			i = 0;
		} }	ret = (ssize_t)call_args(args, front, exe_ret);
	free(front);
	return ((int)ret);
}
