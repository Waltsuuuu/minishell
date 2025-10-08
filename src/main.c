/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 12:39:10 by wheino            #+#    #+#             */
/*   Updated: 2025/10/08 14:40:12 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

int	main(int argc, char *argv[], char *envp[])
{
	(void)	argc;
	(void) (argv);
	t_shell	shell;

	init_shell(&shell, envp);
	run_shell(&shell);
	free_shell(&shell);
	return (shell.last_status);
}

int	run_shell(t_shell *shell)
{
	while (1337)
	{
		shell->status = prompt(shell);
		if (shell->status == -1)
			continue;
		else if (shell->status == 1)
			return (0);
		if (g_signal == SIGINT)
		{
				shell->last_status = 130;
                g_signal = 0;
				free_str_ptr(&shell->line);
				continue ;
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
	shell->buf = ft_strjoin(cwd,"~:$");
	if (!shell->buf)
		return (1) ;
	shell->line = read_with_mode_check(shell->buf);
	free_str_ptr(&shell->buf);
	if (!shell->line)
	{
		clean_env(&shell->env_head);
		free_split(&shell->env_arr);
		clear_history();
		if(isatty(STDIN_FILENO))
			printf("exit\n");
		return (1);
	}
	return (0);
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

int	parse_tokenize(t_shell *shell)
{
	if (check_quote_balance(&shell->line, &shell->last_status) == -1)
	{
		free_str_ptr(&shell->line);
		return (-1);
	}
	add_history(shell->line);
	if (parse_input_line(shell->line, &shell->input) == -1)
	{
		free_on_failure(shell);
		return (-1) ;
	}
	if (count_heredocs(&shell->input) >= 16)
	{
		ft_putstr_fd("minishell: maximum here-document count exceeded\n", STDERR_FILENO);
		return (1) ;
	}
	return (0);
}

int	parse_rm_quotes_and_expand(t_shell *shell)
{
	if (expand_tokens(&shell->input, shell->last_status, shell->env_arr) == -1)
	{
		free_on_failure(shell);
		return (-1) ;
	}
	if (remove_quotes(&shell->input) == -1)
	{
		free_on_failure(shell);
		return (-1) ;
	}
	if (shell->input.n_tokens <= 0)
	{
		free_on_failure(shell);
		return (-1) ;
	}
	return (0);
}

int	build_pipeline_collect_hd(t_shell *shell)
{
	if (build_pipeline(shell, &shell->input, shell->input.tokens, &shell->pipeline) == -1)
	{
		free_on_failure(shell);
		return (-1) ;
	}
	if (collect_heredocs(&shell->pipeline, shell, shell->env_arr) == -1)
	{
		free_on_failure(shell);
		return (-1) ;
	}
	return (0);
}

		// printf("Last status: %d\n", shell.last_status); 	// Last status debug
		// print_tokens(&shell.input);						// Token debug
		// print_cmds(&shell.pipeline);						// Pipeline cmds debug