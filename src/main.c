/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 10:08:12 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/08/21 11:59:13 by mhirvasm         ###   ########.fr       */
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
	char	*input;
	char	**words;
	char	cwd[BUFSIZ];
	char	*buf;
	char	**paths;
	char	**absolute_paths;
	int		counter;
	__pid_t	pid;
	int		status;

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

	input = NULL;
	while (1337)
	{
		
		getworkindir(cwd, sizeof(cwd));
		buf = ft_strjoin(cwd, "~:$");
		if (!buf)
			break;

        input = readline(buf);
		if (!input)
		{
			free(buf);
			break;
		}
		add_history(input);
		words = ft_split(input, ' ');
		if (!words || !*words)
		{
			free_split(words);
			free(buf);
			free(input);
			continue;
		}
		paths = find_from_path(envp);
		absolute_paths = build_absolute_paths(paths, words[0]);
		
		counter = 0;
		while (absolute_paths && absolute_paths[counter])
		{
			if (access(absolute_paths[counter], X_OK) == 0)
			{
				pid = fork();
				if (pid == 0)
				{
					execve(absolute_paths[counter], words, envp);
					perror("execve");
					_exit(127);
				}
				waitpid(pid, &status, 0);
				break;
			}
			counter++;
		}
		if (!absolute_paths || !absolute_paths[counter])
			write(2, "minishell: command not found\n", 29);
		free_split(absolute_paths);
		free_split(paths);
		free_split(words);
		free(buf);
		free (input);
	}
	
	
	return (EXIT_SUCCESS);
}
