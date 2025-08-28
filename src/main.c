
#include "minishell.h"

static void	getworkindir(char *buf, size_t size)
{
	if (NULL == getcwd(buf, size))
		perror("getwcd FAILED");
}


int	main(int argc, char *argv[], char *envp[])
{
	(void)	argc;
	(void) (argv);
	//int		status;
	char	*line;
	t_input	input;
	char	cwd[BUFSIZ];
	char	*buf;
	char	**paths;
	char	**absolute_paths;
	
	setup_signal_handlers_for_prompt();
	

	absolute_paths = NULL;

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
				write(STDOUT_FILENO, "\n", 1);
                g_signal = 0;
				free (buf);
				free(line);
				continue;
        }
		add_history(line);
		if (parse_input_line(line, &input) == -1)
			printf("Something went wrong in parsing, probably gotta add clean up here?\n");
		if (!input.words || input.count == 0)
		{
			clear_struct_on_failure(&input);
			free(buf);
			free(line);
			continue;
		}
		paths = find_from_path(envp);
		absolute_paths = build_absolute_paths(paths, input.words[0]);
		exec_ext_func(absolute_paths, input.words, envp);
		////////////////////////////////////////////////////////////////////////////////////
		//TODO we should be able to open a minishell on minishell. 
		//TODO I head there is environment variable called level, which should be increased!
		////////////////////////////////////////////////////////////////////////////////////
		free_split(absolute_paths);
		free_split(paths);
		free(buf);
		free (line);
		clear_struct_on_failure(&input);
	}
	
	
	return (EXIT_SUCCESS);
}
