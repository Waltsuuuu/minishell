#include "minishell.h"

//without arguments print env
//export KEY=VALUE or export KEY = modify the env

//Arrange in alphabetical order if not already CHECK THIS 
//KEY=VALUE, if key is already in the list -> update it, if not create a new node 
/*
int	run_export(t_shell *shell)
{
	if (!is_builtin_valid(arg))
{
    print_invalid_identifier("export", arg);
    shell->last_status = 1;
    continue;
}


}
*/

int		is_builtin_valid(const char *key) // this is used in both export and unset
{

	int	counter;

	counter = 0;
	if (ft_isalpha(key[0]) || key[0] == '_')
	{
		counter++;
		while (key[counter])
		{
			if (!ft_isalnum(key[counter]) && key[counter] != '_')
			{
				//error message
				return (0);
			}
			counter++;
		}
		return (1);
	}
	else
	{
		//Error message
		return (0);
	}

}
/*
void	print_invalid_identifier(const char *builtin, const char *key)
{
	ft_printf("%s: `%s': not a valid identifier\n", builtin, key);

}
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
