#include "minishell.h"
/**
 * Prints env, and frees it after.
 * @param env env array
 * @param size the size of the array
 */
static void	print_env_and_free(char **env, int size)
{
	int	i;

	i = 0;
	while (i < size)
		ft_printf("declare -x %s\n", env[i++]);
	free_split(env);
}

/**
 * Converts env list to array, sorts it, and prints "declare -x KEY=VALUE".
 *
 * @param shell shell state with env_head
 */
void	env_sort_and_print(t_shell *shell)
{
	char	**env;
	char	*tmp;
	int		i;
	int		j;

	env = env_list_to_array(shell->env_head, shell);
	i = 0;
	while (i < shell->env_size)
	{
		j = i + 1;
		while (j < shell->env_size)
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
	print_env_and_free(env, shell->env_size);
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
		return (0);
	}
	node = create_new_env_node(key, value);
	if (!node)
		return (1);
	append_env_node(head, node);
	return (0);
}
