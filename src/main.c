#include "minishell.h"

static void	getworkindir(char *buf, size_t size)
{
	if (NULL == getcwd(buf, size))
		perror("getwcd FAILED");
}

static char **make_argv_simple(const char *segment)
{
    char *trimmed;
    char **argv;

    if (!segment)
        return (NULL);
    trimmed = ft_strtrim(segment, " \t");
    if (!trimmed)
        return (NULL);
    argv = ft_split(trimmed, ' ');
    free(trimmed);
    return (argv);
}

static char ***build_argvv_from_line(const char *line, int *out_stage_count)
{
    char **segments;
    char ***argvv;
    int count;
    int idx;

    if (!line || !out_stage_count)
        return (NULL);
    segments = ft_split(line, '|');
    if (!segments)
        return (NULL);
    count = 0;
    while (segments[count])
        count++;
    if (count == 0)
        return (free_split(segments), NULL);
    argvv = (char ***)malloc(sizeof(char **) * count);
    if (!argvv)
        return (free_split(segments), NULL);
    idx = 0;
    while (idx < count)
    {
        argvv[idx] = make_argv_simple(segments[idx]);
        if (!argvv[idx] || !argvv[idx][0])
        {
            while (idx > 0)
                free_split(argvv[--idx]);
            free(argvv);
            free_split(segments);
            return (NULL);
        }
        idx++;
    }
    free_split(segments);
    *out_stage_count = count;
    return (argvv);
}
/*
static void free_argvv(char ***argvv, int stage_count)
{
    int i = 0;
    if (!argvv)
        return ;
    while (i < stage_count)
        free_split(argvv[i++]);
    free(argvv);
}
*/
/*// Tokenizer test. Prints tokens
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
}*/


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
	char	**paths;
	char	**absolute_paths;
	char ***argvv;//testing pipe
	int stage_count;

	shell_init(&shell, envp); //TODO shell init

	//setup_signal_handlers_for_prompt(); //TODO delete after shell init works

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

		argvv = build_argvv_from_line(line, &stage_count);
		exec_pipeline(argvv, stage_count, envp);
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
			printf("Something went wrong in parsing, probably gotta add clean up here?\n");
		if (expand_tokens(&shell.input, shell.last_status) == -1)
		{
			clear_struct_on_failure(&shell.input);
			free(buf);
			free(line);
			continue;
		}
		//print_tokens(&shell.input);		// Testing tokenizer
		if (!shell.input.words || shell.input.count == 0)
		{
			clear_struct_on_failure(&shell.input);
			free(buf);
			free(line);
			continue;
		}
		paths = find_from_path(envp);
		absolute_paths = build_absolute_paths(paths, shell.input.words[0]);
		//exec_ext_func(absolute_paths, &shell, envp);
		printf("Last status: %d\n", shell.last_status); //TESTING
		////////////////////////////////////////////////////////////////////////////////////
		//TODO we should be able to open a minishell on minishell. 
		//TODO I head there is environment variable called level, which should be increased!
		////////////////////////////////////////////////////////////////////////////////////
		free_split(absolute_paths);
		free_split(paths);
		free(buf);
		free (line);
		clear_struct_on_failure(&shell.input);
	}
	
	
	return (EXIT_SUCCESS);
}
