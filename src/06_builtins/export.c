#include "minishell.h"

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

void	print_invalid_identifier(const char *builtin, const char *key)
{
	ft_printf("%s: `%s': not a valid identifier\n", builtin, key);

}

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
