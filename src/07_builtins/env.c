/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 12:53:59 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/08 09:24:48 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_env(t_command *cmd, t_shell *shell)
{
	t_env	*env_list;

	if (!shell)
		return (-1);
	env_list = shell->env_head;
	if (cmd->argc > 1)
	{
		ft_putstr_fd("env: ", 2);
		ft_putstr_fd(cmd->argv[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (127);
	}
	while (env_list)
	{
		if (env_list->key && env_list->value && env_list->assigned == 1)
			ft_printf("%s=%s\n", env_list->key, env_list->value);
		env_list = env_list->next;
	}
	return (0);
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
