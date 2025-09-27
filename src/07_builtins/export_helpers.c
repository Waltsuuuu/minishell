#include "minishell.h"
/**
 * Prints env, and frees it after.
 * @param env env array
 * @param size the size of the array
 */
static void	print_env_export(char **env, int size)
{
	int	i;

	i = 0;
	while (i < size)
		ft_printf("declare -x %s\n", env[i++]);
}

/**
 * Converts env list to array, sorts it, and prints "declare -x KEY=VALUE".
 *
 * @param shell shell state with env_head
 */
void	env_sort_and_print(t_shell *shell)
{
	// char	**env;
	char	*tmp;
	int		i;
	int		j;

	if (env_list_to_export_display_array(shell->env_head, shell) == -1) //Creating env in printable form
	{
		printf("ERROR creating a env_arr");
		return ; // Should probably return int aswell to inform of failure.
	}
	i = 0;
	while (i < shell->env_size)
	{
		j = i + 1;
		while (j < shell->env_size)
		{
			if (ft_strcmp(shell->env_arr[i], shell->env_arr[j]) > 0)
			{
				tmp = shell->env_arr[i];
				shell->env_arr[i] = shell->env_arr[j];
				shell->env_arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
	print_env_export(shell->env_arr, shell->env_size);
	if (env_list_to_array(shell->env_head, shell) == -1) //Recreating the right **env for execve usage
	{
		printf("ERROR creating a env_arr");
		return ; // Should probably return int aswell to inform of failure.
	}
}

/**
 * Finds an env node by key.
 *
 * @param head env list head
 * @param key  key to find
 * @return pointer to node, or NULL if not found
 */
t_env	*env_find(t_env *head, const char *key)
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
int	env_set(t_env **head, const char *key, const char *value)
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
		node->assigned = 1;
		return (0);
	}
	node = create_new_env_node(key, value);
	if (!node)
		return (1);
	append_env_node(head, node);
	return (0);
}
