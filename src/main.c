/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 10:08:12 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/08/27 15:33:31 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	getworkindir(char *buf, size_t size)
{
	if (NULL == getcwd(buf, size))
		perror("getwcd FAILED");
}
// Tokenizer test. Prints tokens
static void print_tokens(const t_input *in)
{
	int i;

	if (!in || !in->tokens)
		return ;
	i = 0;
	while (i < in->n_tokens)
	{
		printf("[%d] kind=%d pos=%d text=\"%s\"\n",
			i, in->tokens[i].type, in->tokens[i].pos,
			in->tokens[i].text ? in->tokens[i].text : "(null)");
		i++;
	}
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
		if (!line)
		{
			free(buf);
			break;
		}
		add_history(line);
		if (parse_input_line(line, &input) == -1)
			printf("Something went wrong in parsing, probably gotta add clean up here?\n");
		print_tokens(&input);		// Testing tokenizer
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
