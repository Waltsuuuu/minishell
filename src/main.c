#include "minishell.h"

static void	getworkindir(char *buf, size_t size)
{
	if (NULL == getcwd(buf, size))
		perror("getwcd FAILED");
}

// Tokenizer test. Prints tokens
// static void print_tokens(const t_input *in)
// {
// 	int i;

// 	if (!in || !in->tokens)
// 		return ;
// 	i = 0;
// 	printf("\n TOKENS \n");
// 	while (i < in->n_tokens)
// 	{
// 		printf("[%d] type=%d pos=%d text=\"%s\" was_quoted = \"%d\"\n",
// 			i, in->tokens[i].type, in->tokens[i].pos,
// 			in->tokens[i].text ? in->tokens[i].text : "(null)", in->tokens[i].was_quoted);
// 		i++;
// 	}
// }

int	main(int argc, char *argv[], char *envp[])
{
	(void)	argc;
	(void) (argv);
	//int		status;
	char	*line;
	t_shell	shell = {0};
	//ft_memset(shell.input, 0, sizeof(t_input));
	char	cwd[BUFSIZ];
	char	*buf;
	
	setup_signal_handlers_for_prompt();
	//shell = malloc(sizeof(shell));

	printf( 
"         #             #           #             ###    ###   \n"
"                                    #               #      #   \n"
"                                    #               #      #   \n"
" ## #   ###    #:##:  ###    :###:  #:##:   ###     #      #   \n"
" #:#:#    #    #  :#    #    #: .#  #  :#     :#    #      #   \n"
" # # #    #    #   #    #    #:.    #   #  #   #    #      #   \n"
" # # #    #    #   #    #    .###:  #   #  #####    #      #   \n"
" # # #    #    #   #    #       :#  #   #  #        #      #   \n"
" # # #    #    #   #    #    #. :#  #   #      #    #.     #.  \n"
" # # #  #####  #   #  #####  :###:  #   #   ###:    :##    :## \n"
);

	
	// REPL
	// READ->EVALUATE->PRINT/EXECUTE->LOOP
    rl_bind_key('\t', rl_complete);

	line = NULL;
	while (1337)
	{
		
		getworkindir(cwd, sizeof(cwd));
		buf = ft_strjoin(cwd, "~:$");
		if (!buf)
			break;
		
        line = readline(buf);
		if (!line) // CTR:+D
		{   
			free(buf);
			free (line);  
            printf("exit\n");
            break;
        }
		if (g_signal == SIGINT)
		{
				shell.last_status = 130;
                g_signal = 0;
				free (buf);
				free(line);
				continue;
        }
		if (check_quote_balance(&line) == -1) // Makes sure quotes are closed before sending input to tokenizer
		{
			free(buf);
			continue ;
		}
		add_history(line);
		if (parse_input_line(line, &shell.input) == -1)
		{
			clear_struct_on_failure(&shell.input);
			free(buf);
			free(line);
			continue;
		}
		if (expand_tokens(&shell.input, shell.last_status, envp) == -1)
		{
			clear_struct_on_failure(&shell.input);
			free(buf);
			free(line);
			continue;
		}
		if (remove_quotes(&shell.input) == -1)
		{
			clear_struct_on_failure(&shell.input);
			free(buf);
			free(line);
			continue;
		}
		// print_tokens(&shell.input);		// Testing tokenizer
		if (!shell.input.words || shell.input.count == 0)
		{
			clear_struct_on_failure(&shell.input);
			free(buf);
			free(line);
			continue;
		}
		build_pipeline(&shell.input, shell.input.tokens, &shell.pipeline);
		exec_pipeline(envp, &shell.pipeline);
		// print_cmds(&shell.pipeline);
		printf("Last status: %d\n", shell.last_status); //TESTING
		////////////////////////////////////////////////////////////////////////////////////
		//TODO we should be able to open a minishell on minishell. 
		//TODO I head there is environment variable called level, which should be increased!
		////////////////////////////////////////////////////////////////////////////////////
		free(buf);
		free (line);
		clear_struct_on_failure(&shell.input);
	}
	
	
	return (EXIT_SUCCESS);
}
