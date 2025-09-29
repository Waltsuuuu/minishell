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
	env_node->assigned = 1;
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
int	env_list_to_array(t_env *head, t_shell *shell)
{
	// char	**env_arr;
	int		counter;
	t_env	*env_list;
	char	*pair;


	env_list = head;
	counter = 0;
	while (env_list)
	{
		counter++;
		env_list = env_list->next;
	}
	free_split(&shell->env_arr); //Clean before building
	shell->env_arr = NULL;
	shell->env_size = 0;
	shell->env_arr = malloc((counter + 1) * (sizeof(*shell->env_arr)));
	if (!shell->env_arr)
		return (-1);
	shell->env_size = counter;
	env_list = head;
	counter = 0;
	while (env_list)
	{
		if (env_list->key == NULL)
		{
			free_partial(shell->env_arr, counter);
			return (-1);
		}
		pair = ft_strjoin_with_equal_sign(env_list->key, env_list->value);
		if (!pair)
		{
			free_partial(shell->env_arr, counter);
			return (-1);
		}
		shell->env_arr[counter] = pair;
		env_list = env_list->next;
		counter++;
	}
	shell->env_arr[counter] = NULL;

	return (0);
}

/**
 * @brief Build a printable array for `export` output.
 *
 * Creates a NULL-terminated array of display lines from the env list and
 * stores it in shell->env_arr, setting shell->env_size. Each entry is
 * formatted like Bash `declare -x`:
 *   - unassigned:          "KEY"
 *   - assigned & empty:    "KEY=\"\""
 *   - assigned & nonempty: "KEY=\"VALUE\""
 *
 * @param head   Head of the linked-list of environment variables.
 * @param shell  Shell state; receives the allocated array and its size.
 * @return 0 on success, -1 on allocation error (partial allocations are freed).
 *
 * @note This is for display only. Use a separate builder for execve envp.
 */
int	env_list_to_export_display_array(t_env *head, t_shell *shell)
{

	int		counter;
	t_env	*env_list;
	char	*pair;
	char	*tmp;


	env_list = head;
	counter = 0;
	while (env_list)
	{
		counter++;
		env_list = env_list->next;
	}
	free_split(&shell->env_arr); //Clean before building
	shell->env_arr = NULL;
	shell->env_size = 0;
	shell->env_arr = malloc((counter + 1) * (sizeof(*shell->env_arr)));
	if (!shell->env_arr)
		return (-1);
	shell->env_size = counter;
	env_list = head;
	counter = 0;
	while (env_list)
	{
		if (env_list->key == NULL)
		{
			free_partial(shell->env_arr, counter);
			return (-1);
		}
		if (env_list->assigned == 0)
		{
			pair = ft_strdup(env_list->key);                      /* KEY */
		}
		else if (env_list->value[0] == '\0')
		{
			pair = ft_strjoin(env_list->key, "=\"\"");            /* KEY="" */
		}
		else
		{
			/* KEY="VALUE" */
			tmp = ft_strjoin(env_list->key, "=\"");
			if (!tmp)
			{
				free_partial(shell->env_arr, counter);
				return (-1);
			}
			pair = ft_strjoin(tmp, env_list->value);
			free(tmp);
			if (!pair)
			{
				free_partial(shell->env_arr, counter);
				return (-1);
			}
			tmp = ft_strjoin(pair, "\"");
			free(pair);
			if (!tmp)
			{
				free_partial(shell->env_arr, counter);
				return (-1);
			}
			pair = tmp;
		}

		if (!pair)
		{
			free_partial(shell->env_arr, counter);
			return (-1);
		}
		shell->env_arr[counter] = pair;
		env_list = env_list->next;
		counter++;
	}
	shell->env_arr[counter] = NULL;
	return (0);
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
