/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 09:43:48 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/07 09:47:49 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_unset_in_parent(t_command *cmd, t_shell *shell)
{
	int	saved[2];

	if (!cmd || !shell)
		return (1);
	if (apply_redirs_in_parent(cmd, saved) != 0)
	{
		return (1);
	}
	shell->last_status = builtin_unset(cmd->argv, shell);
	restore_stdio(saved);
	return (shell->last_status);
}

static int	is_valid_unset_name(const char *key)
{
	int	counter;

	if (!key || !key[0])
		return (0);
	if (!(ft_isalpha((unsigned char)key[0]) || key[0] == '_'))
		return (0);
	counter = 1;
	while (key[counter])
	{
		if (!ft_isalnum((unsigned char)key[counter]) && key[counter] != '_')
			return (0);
		counter++;
	}
	return (1);
}

int	builtin_unset(char **argv, t_shell *shell)
{
	int	counter;

	if (!argv[1] || *argv[1] == '\0')
		return (0);
	counter = 1;
	while (argv[counter])
	{
		if (!is_valid_unset_name(argv[counter]))
			return (0);
		else
			env_unset(shell, argv[counter]);
		counter++;
	}
	return (0);
}

int	env_unset(t_shell *shell, const char *key)
{
	t_env	**ptr;
	t_env	*cur;

	if (!shell || !key || !*key)
		return (0);
	ptr = &shell->env_head;
	while (*ptr)
	{
		cur = *ptr;
		if (ft_strcmp((cur->key), key) == 0)
		{
			*ptr = cur->next;
			free (cur->key);
			free (cur->value);
			free (cur);
			return (0);
		}
		ptr = &cur->next;
	}
	return (0);
}
