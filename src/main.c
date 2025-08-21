/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 10:08:12 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/08/21 15:15:42 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		if (!line) { //TODO signals not working properly, also check the child
            if (g_signal == SIGINT) {
                // Interrupted line editing by Ctrl-C -> just refresh prompt
                write(1, "\n", 1);
                g_signal = 0;
				free (buf);
				free(line);
				continue;
            }
            // Real EOF -> exit (subject requirement)
            printf("exit\n");
            break;
        }
		
    	else if (g_signal == SIGQUIT)  // Ctrl-\"
    	{
        g_signal = 0;
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

		free_split(absolute_paths);
		free_split(paths);
		free(buf);
		free (line);
		clear_struct_on_failure(&input);
	}
	
	
	return (EXIT_SUCCESS);
}
