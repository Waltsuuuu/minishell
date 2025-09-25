#include "minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	(void)	argc;
	(void) (argv);
	char	*line;
	t_shell	shell = {0};
	char	cwd[BUFSIZ];
	
	setup_signal_handlers_for_prompt();
	print_msh_banner();
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
		shell.env_arr = env_list_to_array(shell.env_head, &shell); //Remember to clean in the end 
		getworkindir(cwd, sizeof(cwd));
		shell.buf = ft_strjoin(cwd, "~:$");
		if (!shell.buf)
			break ;
        line = readline(shell.buf);
		if (!line) // CTR:+D
		{   
			free(shell.buf);
			free (line);  
            printf("exit\n");
            break ;
        }
		if (g_signal == SIGINT)
		{
				shell.last_status = 130;
                g_signal = 0;
				free (shell.buf);
				free(line);
				continue ;
        }
		if (check_quote_balance(&line, &shell.last_status) == -1)
		{
			free(shell.buf);
			continue ;
		}
		add_history(line);
		if (parse_input_line(line, &shell.input) == -1)
		{
			free_allocs(&shell);
			free(line);
			continue ;
		}
		if (expand_tokens(&shell.input, shell.last_status, shell.env_arr) == -1)
		{
			free_allocs(&shell);
			free(line);
			continue ;
		}
		if (remove_quotes(&shell.input) == -1)
		{
			free_allocs(&shell);
			free(line);
			continue ;
		}
		// print_tokens(&shell.input);						// Token debug
		if (!shell.input.words || shell.input.count == 0)
		{
			free_allocs(&shell);
			free(line);
			continue;
		}
		// print_cmds(&shell.pipeline);						// Pipeline cmds debug
		// printf("Last status: %d\n", shell.last_status); 	// Last status debug
		if (build_pipeline(&shell.input, shell.input.tokens, &shell.pipeline) == -1)
		{
			free_allocs(&shell);
			free(line);
			continue;
		}
		exec_dispatch(shell.env_arr, &shell.pipeline, &shell);
		//exec_pipeline(envp, &shell.pipeline, &shell);
		free(line);
		
	}
	clean_env(&shell.env_head);
	return (EXIT_SUCCESS);
}

// Notes:
// TODO we should be able to open a minishell on minishell. 
// TODO I heard there is environment variable called level, which should be increased!
