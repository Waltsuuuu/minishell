/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 12:53:59 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/06 12:55:01 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(t_shell *shell)
{
	t_env	*env_list;

	env_list = shell->env_head;
	while (env_list)
	{
		if (env_list->key && env_list->value)
			ft_printf("%s=%s\n", env_list->key, env_list->value);
		env_list = env_list->next;
	}
}

char	*env_get(t_shell *shell, const char *key)
{
	t_env	*env;

	env = shell->env_head;
	while (env)
	{
		if (env->key && key && !ft_strncmp(env->key, key, ft_strlen(key)))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
