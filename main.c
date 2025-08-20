/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 10:08:12 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/08/20 13:00:59 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	getworkindir(char *buf, size_t size)
{
	if (NULL == getcwd(buf, size))
		perror("getwcd FAILED");
}

int	main(int argc, char *argv[])
{
	(void)	argc;
	(void) (argv);
	//int		status;
	char	*input;
	char	cwd[BUFSIZ];
	char	*buf;

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

	// Configure readline to auto-complete paths when the tab key is hit.
    rl_bind_key('\t', rl_complete);

    // Enable history
    using_history();
	input = NULL;
	while (1337)
	{
		
		//1) get line
		getworkindir(cwd, sizeof(cwd));
		buf = ft_strjoin(cwd, "~:$");
		 // Display prompt and read input
        char* input = readline(buf); // add ":~$" with strjoin or smthng
		printf("The input is: %s\n", input);

        // Check for EOF.
        if (!input)
            break;

        // Add input to readline history.
        add_history(input);

        // Do stuff...

        // Free buffer that was allocated by readline

		//2) get tokens

		//3) exec
		
		//Remember proper frees, consider memory arena?
		free(buf);
		free (input);
	}
	
	//if (fork() == 0)
	//	execvp(argv[1], argv + 1);
	//wait(&status);
	
	return (EXIT_SUCCESS);
}