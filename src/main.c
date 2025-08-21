/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 10:08:12 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/08/20 16:03:18 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	getworkindir(char *buf, size_t size)
{
	if (NULL == getcwd(buf, size))
		perror("getwcd FAILED");
}

char	**find_from_path(char *cmd, char *envp[])
{
	char	*path;
	char	**paths;
	size_t	counter;
	(void)cmd;
	
	path = NULL;
	counter = 0;
	while(envp[counter])
	{
	 if (strncmp(envp[counter], "PATH=", 5) == 0)
	 {
    	printf("%s\n", envp[counter]); //Printing the envpath
		path = &envp[counter][5]; //skipping the word "path="
		break;
	 }
	counter++;
	}
	printf("Printing path variable: %s\n", path); //printing the path
	
	//Now we need to split the path ---> **paths (ft_split':')
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	
	counter = 0; 
	while(paths[counter]) //testing paths
	{
		printf("path n%zu, %s\n", counter, paths[counter]);
		counter++;
	}
	return (paths);
	// workiing so far
	
}

int	main(int argc, char *argv[], char *envp[])
{
	(void)	argc;
	(void) (argv);
	//int		status;
	char	*input;
	char	cwd[BUFSIZ];
	char	*buf;
	char	**paths;

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
		paths = find_from_path(input, envp); //Now we have all the paths here in an array

		//TODO Now we need to find the "ABSOLUTEPATH", so we need to append to each
		//path "/+command_name". for example /usr/bin ---> /usr/bin/ls. 
		//Then we need to check with access that we have permission to execute it (as an example with ls)
		
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
	
	
	return (EXIT_SUCCESS);
}