/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 11:53:45 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/09 08:59:44 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	int		equal;
	int		key_len;
	char	*val_start;

	equal = find_equal_sign(line);
	if (equal >= 0)
		key_len = equal;
	else
		key_len = (int)ft_strlen(line);
	*key_out = (char *)malloc(key_len + 1);
	if (!*key_out)
		return (1);
	ft_strlcpy(*key_out, line, key_len + 1);
	if (equal >= 0)
		val_start = &line[equal + 1];
	else
		val_start = "";
	*value_out = ft_strdup(val_start);
	if (!*value_out)
		return (free(*key_out), 1);
	return (0);
}

static void	free_env_node(t_env *node)
{
	if (!node)
		return ;
	free(node->key);
	free(node->value);
	free(node);
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
				free_env_node(node);
				clean_env(&env_head);
				return (free (key), free (value), NULL);
			}
			free (key);
			free (value);
		}
		i++;
	}
	return (env_head);
}
