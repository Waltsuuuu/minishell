#include "minishell.h"

/**
 * Builtin: export.
 * With no arguments, prints the environment sorted as declare -x lines.
 * With KEY=VALUE or KEY, sets or creates variables in the shell env.
 *
 * @param argv   command arguments (argv[0] == "export")
 * @param shell  shell state holding env list
 * @return 0 on success, 1 if any argument was invalid or failed to set
 */
int	builtin_export(char **argv, t_shell *shell)
{
	int	counter;
	int	status;

	if (!argv[1])
	{
		env_sort_and_print(shell);
		return (0);
	}
	counter = 1;
	status = 0;
	while (argv[counter])
	{
		if (process_export_arg(argv[counter], shell) != 0)
			status = 1;
		counter++;
	}
	return (status);
}

/**
 * Processes one export argument (KEY=VALUE or KEY).
 * Validates identifier and updates env accordingly.
 *
 * @param arg    argument string
 * @param shell  shell state
 * @return 0 on success, 1 on error
 */
int	process_export_arg(char *argv, t_shell *shell)
{
	char	*key;
	char	*value;
	int		equal;
	t_env	*node;

	if (!is_builtin_valid(argv))
	{
		print_invalid_identifier("export", argv);
		return (1);
	}
	equal = find_equal_sign(argv);
	if (equal > 0)
	{
		if (split_key_and_value(argv, &key, &value) != 0)
			return (1);
		shell->last_status = env_set(&shell->env_head, key, value);
		free(key);
		free(value);
		return (shell->last_status);
	}
	else
	{
		node = env_find(shell->env_head, argv);
		if (!node)
		{
			if (env_set(&shell->env_head, argv, "") != 0)
				return (1);
			node = env_find(shell->env_head, argv);
			if (node)
				node->assigned = 0;      // NEW: created by `export KEY`
		}
	}
	return (0);
}

/**
 * Validates an identifier for export/unset.
 * First char must be alphabetical or underscore; rest alnum or underscore.
 *
 * @param key  candidate identifier (possibly with '=')
 * @return 1 if valid up to '=' or end, else 0
 */
int	is_builtin_valid(const char *key)
{
	int	counter;

	if (!key || key[0] == '\0')
		return (0);
	counter = 0;
	if (ft_isalpha((unsigned char)key[0]) || key[0] == '_')
	{
		counter++;
		while (key[counter] && key[counter] != '=')
		{
			if (!ft_isalnum((unsigned char)key[counter]) && key[counter] != '_')
				return (0);
			counter++;
		}
		return (1);
	}
	else
		return (0);
}

/**
 * Prints "not a valid identifier" message for a builtin.
 *
 * @param builtin  builtin name ("export", etc.)
 * @param key      offending key string
 */
void	print_invalid_identifier(char *builtin, char *key)
{
	ft_putstr_fd(builtin, STDERR_FILENO);
	ft_putstr_fd(": `", STDERR_FILENO);
	ft_putstr_fd(key, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	// ft_printf("%s: `%s': not a valid identifier\n", builtin, key);
}

/**
 * Finds the index of '=' in a string.
 *
 * @param str string to scan
 * @return index of '=', or -1 if none
 */
int	find_equal_sign(char *str)
{
	int	counter;

	if (!str)
		return (-1);
	counter = 0;
	while (str[counter] && str[counter] != '=')
		counter++;
	if (str[counter] == '=')
		return (counter);
	return (-1);
}
