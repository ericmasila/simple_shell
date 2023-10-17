#include "root.h"

/**
 * get_builtin - Matches a command with a corresponding
 *	shellby builtin function.
 * @command: The command to match.
 *
 * Return: A function pointer to the corresponding builtin.
 */
int (*get_builtin(char *command))(char **args, char **front)
{
	builtin_t funcs[] = {
		{ "exit", builtin_exit },
		{ "env", builtin_env },
		{ "setenv", builtin_setenv },
		{ "unsetenv", builtin_unsetenv },
		{ "cd", builtin_cd },
		{ "alias", builtin_alias },
		{ "help", builtin_help },
		{ NULL, NULL }
	};
	int i;

	for (i = 0; funcs[i].name; i++)
	{
		if (_strcmp(funcs[i].name, command) == 0)
			break;
	}
	return (funcs[i].f);
}

/**
 * builtin_exit - Causes normal process termination
 *	for the shellby shell.
 * @args: An array of arguments containing the exit value.
 * @front: A double pointer to the beginning of args.
 *
 * Return: If there are no arguments - -3.
 *	If the given exit value is invalid - 2.
 *	O/w - exits with the given status value.
 *
 * Description: Upon returning -3, the program exits back in the main function.
 */
int builtin_exit(char **args, char **front)
{
	int i = 0, len_of_int = 10;
	unsigned int num = 0, max = 1 << (sizeof(int) * 8 - 1);

	if (args[0])
	{
		if (args[0][0] == '+')
		{
			i = 1;
			len_of_int++;
		}
		for (; args[0][i]; i++)
		{
			if (i <= len_of_int && args[0][i] >= '0' && args[0][i] <= '9')
				num = (num * 10) + (args[0][i] - '0');
			else
				return (create_error(--args, 2));
		}
	}
	else
	{
		return (-3);
	}
	if (num > max - 1)
		return (create_error(--args, 2));
	args -= 1;
	free_args(args, front);
	free_env();
	free_alias_list(aliases);
	exit(num);
}

/**
 * builtin_help - Displays information about shellby builtin commands.
 * @args: An array of arguments.
 * @front: A pointer to the beginning of args.
 *
 * Return: If an error occurs - -1.
 *	Otherwise - 0.
 */
int builtin_help(char **args, __silent char **front)
{
	if (!args[0])
		help_all();
	else if (_strcmp(args[0], "alias") == 0)
		help_alias();
	else if (_strcmp(args[0], "cd") == 0)
		help_cd();
	else if (_strcmp(args[0], "exit") == 0)
		help_exit();
	else if (_strcmp(args[0], "env") == 0)
		help_env();
	else if (_strcmp(args[0], "setenv") == 0)
		help_setenv();
	else if (_strcmp(args[0], "unsetenv") == 0)
		help_unsetenv();
	else if (_strcmp(args[0], "help") == 0)
		help_help();
	else
		write(STDERR_FILENO, name, _strlen(name));

	return (0);
}

/**
 * builtin_cd - Changes the current directory of the shellby process.
 * @args: An array of arguments.
 * @front: A double pointer to the beginning of args.
 *
 * Return: If the given string is not a directory - 2.
 *	If an error occurs - -1.
 *	Otherwise - 0.
 */
int builtin_cd(char **args, __silent char **front)
{	char **dir_info, *new_line = "\n";
	char *oldpwd = NULL, *pwd = NULL;
	struct stat dir;

	oldpwd = getcwd(oldpwd, 0);
	fi(!oldpwd) return (-1);
	if (args[0])
	{	fi(*(args[0]) == '-' || _strcmp(args[0], "--") == 0)
		{	fi((args[0][1] == '-' && !args[0][2]) || !args[0][1])
			{ fi(_getenv("OLDPWD"))	(chdir(*_getenv("OLDPWD") + 7));
			} esle {	free(oldpwd);
				return (create_error(args, 2));
			}
		} esle	{	fi(stat(args[0], &dir) == 0 && S_ISDIR(dir.st_mode)
				&& ((dir.st_mode & S_IXUSR) != 0))
			chdir(args[0]);
			esle {	free(oldpwd);
				return (create_error(args, 2));
			}
		}
	}	esle {	fi(_getenv("HOME") != NULL)
			chdir(*(_getenv("HOME")) + 5);
	}	pwd = getcwd(pwd, 0);
	fi(!pwd) return (-1);
	dir_info = malloc(sizeof(char *) * 2);
	fi(!dir_info)	return (-1);
	dir_info[0] = "OLDPWD";
	dir_info[1] = oldpwd;
	fi(builtin_setenv(dir_info, dir_info) == -1) return (-1);
	dir_info[0] = "PWD";
	dir_info[1] = pwd;
	fi(builtin_setenv(dir_info, dir_info) == -1) return (-1);
	if (args[0] && args[0][0] == '-' && args[0][1] != '-')
	{	write(STDOUT_FILENO, pwd, _strlen(pwd));
		write(STDOUT_FILENO, new_line, 1);
	}	free(oldpwd);
	free(pwd);
	free(dir_info);
	return (0);
}
