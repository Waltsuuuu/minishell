/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 12:39:10 by wheino            #+#    #+#             */
/*   Updated: 2025/10/10 11:17:49 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

int	main(int argc, char *argv[], char *envp[])
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	run_shell(&shell);
	free_shell(&shell);
	return (shell.last_status);
}

void	init_shell(t_shell *shell, char *envp[])
{
	ft_bzero(shell, sizeof(*shell));
	setup_signal_handlers_for_prompt();
	if (isatty(STDIN_FILENO))
		print_msh_banner();
	shell->line = NULL;
	shell->env_head = env_init_from_envp(envp);
}

int	run_shell(t_shell *shell)
{
	while (1337)
	{
		shell->status = prompt(shell);
		if (shell->status == -1)
			continue ;
		else if (shell->status == 1)
			return (0);
		if (g_signal == SIGINT)
		{
			shell->last_status = 130;
			g_signal = 0;
		}
		shell->status = parse(shell);
		if (shell->status == -1)
			continue ;
		else if (shell->status == 1)
			return (0);
		shell->last_status = exec_dispatch(&shell->pipeline, shell);
		free_str_ptr(&shell->line);
		free_allocs(shell);
	}
	return (0);
}

int	prompt(t_shell *shell)
{
	char	cwd[PATH_MAX];

	if (env_list_to_array(shell->env_head, shell) == -1)
		return (-1);
	getworkindir(cwd, sizeof(cwd));
	shell->buf = ft_strjoin(cwd, "~:$");
	if (!shell->buf)
		return (1);
	shell->line = read_with_mode_check(shell->buf);
	free_str_ptr(&shell->buf);
	if (!shell->line)
	{
		clean_env(&shell->env_head);
		free_split(&shell->env_arr);
		clear_history();
		if (isatty(STDIN_FILENO))
			ft_printf("exit\n");
		return (1);
	}
	return (0);
}

int	parse(t_shell *shell)
{
	int	status;

	status = parse_tokenize(shell);
	if (status != 0)
		return (status);
	status = parse_rm_quotes_and_expand(shell);
	if (status != 0)
		return (status);
	status = build_pipeline_collect_hd(shell);
	if (status != 0)
		return (status);
	return (status);
}
