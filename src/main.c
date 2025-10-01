#include "minishell.h"

// REAL
// line = readline(shell.buf);
// free_str_ptr(&shell.buf);
// if (!line) // CTR:+D
// {  
// 	clean_env(&shell.env_head);
// 	free_split(&shell.env_arr);
// 	clear_history();
// 	printf("exit\n");
// 	break ;
// }

// MSTEST M
// if (isatty(fileno(stdin)))
// line = readline(shell.buf);
// else
// {
// char *line;
// line = get_next_line(fileno(stdin));
// line = ft_strtrim(line, "\n");
// free(line);
// }
// // line = readline(shell.buf);
// free_str_ptr(&shell.buf);
// if (!line) // CTR:+D
// {  
// clean_env(&shell.env_head);
// free_split(&shell.env_arr);
// clear_history();
// // printf("exit\n");
// break ;
// }

int	main(int argc, char *argv[], char *envp[])
{
	(void)	argc;
	(void) (argv);
	char	*line;
	t_shell	shell = {0};
	char	cwd[BUFSIZ];
	
	setup_signal_handlers_for_prompt();
	// print_msh_banner();
    rl_bind_key('\t', rl_complete);
	line = NULL;

	//t_env *envlist;
	shell.env_head = env_init_from_envp(envp); //Initialize envp
	//envlist = shell.env_head;
	//if (shell.env_head == NULL)
	//	printf("init FAIL");
	//print_env(&shell);
	//env_sort_and_print(&shell);
	while (1337)
	{
		if (env_list_to_array(shell.env_head, &shell) == -1)
			continue ;
		getworkindir(cwd, sizeof(cwd));
		shell.buf = ft_strjoin(cwd, "~:$");
		if (!shell.buf)
			break ;
		// ADD TESTER BLOCK HERE vvv
		line = readline(shell.buf);
		free_str_ptr(&shell.buf);
		if (!line) // CTR:+D
		{  
			clean_env(&shell.env_head);
			free_split(&shell.env_arr);
			clear_history();
			// printf("exit\n");
			break ;
		}
		// ADD TESTER BLOCK HERE ^^^
		if (g_signal == SIGINT)
		{
				shell.last_status = 130;
                g_signal = 0;
				free_str_ptr(&line);
				continue ;
        }
		if (check_quote_balance(&line, &shell.last_status) == -1)
		{
			free_str_ptr(&line);
			continue ;
		}
		add_history(line);
		if (parse_input_line(line, &shell.input) == -1)
		{
			free_str_ptr(&line);
			free_allocs(&shell);
			free_split(&shell.env_arr);
			continue ;
		}
		if (count_heredocs(&shell.input) >= 16)
		{
			ft_putstr_fd("minishell: maximum here-document count exceeded\n", STDERR_FILENO);
			free_str_ptr(&line);
			free_allocs(&shell);
			free_split(&shell.env_arr);
			continue ;
		}
		if (expand_tokens(&shell.input, shell.last_status, shell.env_arr) == -1)
		{
			free_str_ptr(&line);
			free_allocs(&shell);
			free_split(&shell.env_arr);
			continue ;
		}
		if (remove_quotes(&shell.input) == -1)
		{
			free_str_ptr(&line);
			free_allocs(&shell);
			free_split(&shell.env_arr);
			continue ;
		}
		// print_tokens(&shell.input);						// Token debug
		if (shell.input.n_tokens <= 0)
		{
			free_str_ptr(&line);
			free_allocs(&shell);
			free_split(&shell.env_arr);
			continue ;
		}
		// printf("Last status: %d\n", shell.last_status); 	// Last status debug
		if (build_pipeline(&shell, &shell.input, shell.input.tokens, &shell.pipeline) == -1)
		{
			free_str_ptr(&line);
			free_allocs(&shell);
			free_split(&shell.env_arr);
			continue ;
		}
		if (collect_heredocs(&shell.pipeline, &shell, shell.env_arr) == -1)
		{
			free_str_ptr(&line);
			free_allocs(&shell);
			free_split(&shell.env_arr);
			continue ;
		}
		// print_tokens(&shell.input);						// Token debug
		// print_cmds(&shell.pipeline);						// Pipeline cmds debug
		shell.last_status = exec_dispatch(shell.env_arr, &shell.pipeline, &shell);
		free_str_ptr(&line);
		free_allocs(&shell);
	}
	free_str_ptr(&shell.buf);
	free_str_ptr(&line);
	free_allocs(&shell);
	clean_env(&shell.env_head);
	free_split(&shell.env_arr);
	clear_history();
	return (shell.last_status);
}


// Notes:
// TODO we should be able to open a minishell on minishell. 
// TODO I heard there is environment variable called level, which should be increased!
