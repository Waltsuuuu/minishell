#include "minishell.h"

/**
 * Splits "KEY=VALUE" into newly allocated key/value.
 * If '=' missing, value becomes empty string.
 *
 * @param line      input line
 * @param key_out   result key (malloc'd)
 * @param value_out result value (malloc'd)
 * @return 0 on success, 1 on allocation error
 */
int	split_key_and_value(char *line, char **key_out, char **value_out)
{
	int		counter;
	char	*start;

	start = line;
	counter = 0;
	while (*line && *line != '=')
	{
		counter++;
		line++;
	}
	*key_out = malloc(counter + 1);
	if (!*key_out)
		return (1);
	ft_strlcpy(*key_out, start, counter +1);
	if (*line == '=')
	{
		*value_out = ft_strdup(line + 1);
		if (!*value_out)
			return (free (*key_out),1);
	}
	else
	{
		*value_out = ft_strdup("");
		if (!*value_out)
			return (free (*key_out), 1);
	}
	return (0);
}

/**
 * Builds env list from envp.
 *
 * @param envp  environment vector
 * @return head of env list, or NULL on error
 */
t_env	*env_init_from_envp(char **envp)
{
	int		i;
	char	*key;
	char	*value;
	t_env	*node;
	t_env	*env_head;

	env_head = NULL;
	i = 0;
	while (envp && envp[i])
	{
		if (split_key_and_value(envp[i], &key, &value) == 0)
		{
			node = create_new_env_node(key, value);
			if (!node || append_env_node(&env_head, node) != 0)
			{
				free (key);
				free (value);
				free (node->key);
				free(node->value);
				free (node);
				return (NULL);
			}
			free (key);
			free (value);
		}
		i++;
	}
	return (env_head);
}

/**
 * Allocates a new env node (key/value are duplicated).
 *
 * @param key   key string (required)
 * @param value value string (NULL treated as "")
 * @return new node, or NULL on error
 */
t_env	*create_new_env_node(const char *key, const char *value)
{
	t_env	*env_node;

	if (!key)
		return (NULL);
	env_node = malloc(sizeof(*env_node));
	if (!env_node)
		return (NULL);
	env_node->key = ft_strdup(key);
	if (value)
		env_node->value = ft_strdup(value);
	else if (!value)
		env_node->value =ft_strdup("");
	env_node->next = NULL;
	if (!env_node->key || !env_node->value)
	{
		free (env_node->key);
		free (env_node->value);
		free (env_node);
		return (NULL);
	}
	return (env_node);
}

/**
 * Appends an env node to the end of list.
 *
 * @param head pointer to list head
 * @param new_env_node node to append
 * @return 0 on success
 */
int	append_env_node(t_env **head, t_env *new_env_node)
{
	t_env	*temp_node;

	if (!*head)
	{
		*head = new_env_node;
		return (0);
	}
	temp_node = *head;
	while (temp_node->next != NULL)
		temp_node = temp_node->next;
	temp_node->next = new_env_node;
	return (0);
}

/**
 * Converts env list to a NULL-terminated array of "KEY=VALUE" strings.
 *
 * @param head env list head
 * @param size out param: number of entries (no NULL)
 * @return newly allocated array, or NULL on error
 */
char	**env_list_to_array(t_env *head, t_shell *shell)
{
	char	**env_arr;
	int		counter;
	t_env	*env_list;
	char	*test;

	env_list = head;
	counter = 0;

	while (env_list)
	{
		counter++;
		env_list = env_list->next;
	}
	env_arr = malloc((counter + 1) * (sizeof(*env_arr)));
	shell->env_size = counter;
	if (!env_arr)
		return (NULL);

	env_list = head;
	counter = 0;
	while (env_list)
	{
		if (env_list->key == NULL)
		{
			free_partial(env_arr, counter);
			return (NULL);
		}
		test = ft_strjoin_with_equal_sign(env_list->key, env_list->value);
		if (!test)
		{
			free_partial(env_arr, counter);
			return (NULL);
		}
		env_arr[counter] = test;
		env_list = env_list->next;
		counter++;
	}
	env_arr[counter] = NULL;

	return (env_arr);
}

/**
 * Joins key and value into "KEY=VALUE".
 *
 * @param s1 key (NULL treated as "")
 * @param s2 value (NULL treated as "")
 * @return newly allocated string, or NULL on error
 */
char	*ft_strjoin_with_equal_sign(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*result_str;
	size_t	counter;

	if (!s1) //if null treat as an empty string
		s1 = "";
	if (!s2)
		s2 = "";
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	result_str = malloc(s1_len + s2_len + 2);
	if (result_str == NULL)
		return (NULL);
	counter = 0;
	while (*s1)
		result_str[counter++] = *s1++;
	result_str[counter++] = '=';
	while (*s2)
		result_str[counter++] = *s2++;
	result_str[counter] = '\0';
	return (result_str);
}

/**
 * Joins key and value into "KEY=VALUE".
 *
 * @param s1 key (NULL treated as "")
 * @param s2 value (NULL treated as "")
 * @return newly allocated string, or NULL on error
 */
void	clean_env(t_env **head)
{
	t_env	*current;
	t_env	*next;

	if (!head || !*head)
		return ;
	current = *head;
	while (current)
	{
		next = current->next;
		free (current->key);
		free (current->value);
		free (current);
		current = next;
	}
	*head = NULL;
}
