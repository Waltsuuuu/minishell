/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 08:54:41 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/09 10:50:01 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		env_node->value = ft_strdup("");
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

	if (!s1)
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
