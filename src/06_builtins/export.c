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
		if (env_set(&shell->env_head, key, value) != 0)
			return (1);
		free (key);
		free (value);
	}
	else
	{
		if (!env_find(shell->env_head, argv))
			if (env_set(&shell->env_head, argv, "") != 0)
				return (1);
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
int		is_builtin_valid(const char *key)
{

	int	counter;

	counter = 0;
	if (!key || ft_isalpha(key[0]) || key[0] == '_')
	{
		counter++;
		while (key[counter] && key[counter] != '=')
		{
			if (!ft_isalnum(key[counter]) && key[counter] != '_')
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
void	print_invalid_identifier(const char *builtin, const char *key)
{
	ft_printf("%s: `%s': not a valid identifier\n", builtin, key);

}

/**
 * Finds an env node by key.
 *
 * @param head env list head
 * @param key  key to find
 * @return pointer to node, or NULL if not found
 */
t_env *env_find(t_env *head, const char *key)
{
	t_env	*envlist;

	if (!key)
		return (NULL);
	envlist = head;
	while (envlist)
	{
		if (!ft_strcmp(envlist->key, key))
		{
			return (envlist);
		}
		envlist = envlist->next;
	}
	return (NULL);
}

/**
 * Sets or creates an env key to value.
 *
 * @param head  pointer to env head pointer
 * @param key   key string (non-empty)
 * @param value value string (NULL treated as "")
 * @return 0 on success, 1 on error
 */
int env_set(t_env **head, const char *key, const char *value)
{
	t_env	*node;
	char	*new_value;

	if (!head || !key || !*key)
		return (1);
	node = NULL;
	node = env_find(*head, key);
	if (node)
	{
		if (value)
			new_value = ft_strdup(value);
		else
			new_value = ft_strdup("");
		if (!new_value)
			return (1);
		free (node->value);
		node->value = new_value;
		return (0);
	}
		node = create_new_env_node(key, value);
		if (!node)
			return (1);
		append_env_node(head, node);
		return (0);
}

/**
 * Converts env list to array, sorts it, and prints "declare -x KEY=VALUE".
 *
 * @param shell shell state with env_head
 */
void env_sort_and_print(t_shell *shell)
{
	char	**env;
	char	*tmp;
	int		size;
	int		i;
	int		j;

	env = env_list_to_array(shell->env_head, &size);
	i = 0;
	while (i < size)
	{
		j = i + 1;
		while (j < size)
		{
			if (ft_strcmp(env[i], env[j]) > 0)

			{
				tmp = env[i];
				env[i] = env[j];
				env[j] = tmp;
			}
			j++;
		}
		i++;
	}
	i = 0;
	while (i < size)
		ft_printf("declare -x %s\n", env[i++]);
	free_split(env);
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
