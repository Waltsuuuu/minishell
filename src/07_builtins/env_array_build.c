/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_array_build.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 08:58:45 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/09 10:49:11 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	pair_format_for_env(t_shell *shell, t_env *head)
{
	int		counter;
	t_env	*env_list;
	char	*pair;

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
 * Converts env list to a NULL-terminated array of "KEY=VALUE" strings.
 *
 * @param head env list head
 * @param size out param: number of entries (no NULL)
 * @return newly allocated array, or NULL on error
 */
int	env_list_to_array(t_env *head, t_shell *shell)
{
	int		counter;
	t_env	*env_list;

	env_list = head;
	counter = 0;
	while (env_list)
	{
		counter++;
		env_list = env_list->next;
	}
	free_split(&shell->env_arr);
	shell->env_arr = NULL;
	shell->env_size = 0;
	shell->env_arr = malloc((counter + 1) * (sizeof(*shell->env_arr)));
	if (!shell->env_arr)
		return (-1);
	shell->env_size = counter;
	if (pair_format_for_env(shell, head) != 0)
	{
		shell->env_arr = NULL;
		shell->env_size = 0;
		return (-1);
	}
	return (0);
}

/*
 * Builds: KEY="VALUE"
 * Uses your ft_strjoin + free pattern. Returns NULL on error.
 */
static char	*key_and_value_format(t_env *env_list)
{
	char		*tmp;
	char		*pair;
	const char	*val;

	if (!env_list || !env_list->key)
		return (NULL);
	val = env_list->value;
	if (!val)
		val = "";
	tmp = ft_strjoin(env_list->key, "=\"");
	if (!tmp)
		return (NULL);
	pair = ft_strjoin(tmp, val);
	free(tmp);
	if (!pair)
		return (NULL);
	tmp = ft_strjoin(pair, "\"");
	free(pair);
	if (!tmp)
		return (NULL);
	return (tmp);
}

static int	pair_format_for_export_display(t_shell *shell, t_env *head)
{
	int		counter;
	char	*pair;
	t_env	*env_list;

	env_list = head;
	counter = 0;
	while (env_list)
	{
		if (env_list->key == NULL)
			return (free_partial(shell->env_arr, counter), -1);
		if (env_list->assigned == 0)
			pair = ft_strdup(env_list->key);
		else if (env_list->value[0] == '\0')
			pair = ft_strjoin(env_list->key, "=\"\"");
		else
			pair = key_and_value_format(env_list);
		if (!pair)
			return (free_partial(shell->env_arr, counter), -1);
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
	t_env	*env_list;
	int		counter;

	env_list = head;
	counter = 0;
	while (env_list)
	{
		counter++;
		env_list = env_list->next;
	}
	free_split(&shell->env_arr);
	shell->env_arr = NULL;
	shell->env_size = 0;
	shell->env_arr = malloc((counter + 1) * (sizeof(*shell->env_arr)));
	if (!shell->env_arr)
		return (-1);
	shell->env_size = counter;
	if (pair_format_for_export_display(shell, head) != 0)
	{
		shell->env_arr = NULL;
		shell->env_size = 0;
		return (-1);
	}
	return (0);
}
