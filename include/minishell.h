
#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "colors.h"
# include <sys/wait.h>
# include <sys/stat.h> // access and macros
# include <signal.h>

typedef struct s_shell
{
    int     last_status;   // for $?
    char  **env;           // environment variables (array)
    // or: t_env *env_list; if we manage as a linked list
    //t_input	input; //TODO open after conflicts
    char 	**argv;          // arguments for current command
    // parsing / tokens
    char   *cwd;
}   t_shell;

//TODO move signals.h
extern volatile sig_atomic_t g_signal;
void	setup_signal_handlers_for_prompt();
void	setup_signal_handlers_for_child();
void	handle_sig(int signum);

typedef struct s_input {
	char	*raw;		// Original line the user typed in.
	char	**words;		// Array of words (split from the original line).
	int		count;		// Number of words.
}		t_input;

//TODO move to executor.h
char	*join_cmd_to_path(const char *path, const char *cmd);
char	**find_from_path(char *envp[]);
char	**build_absolute_paths(char **paths, const char *cmd);
void	exec_ext_func(char **absolute_paths, char **words, char *envp[]);
//TODO move to utils.h
void	free_split(char **arr);
void	free_partial(char **arr, size_t n);

//Parser
int		parse_input_line(const char *line, t_input *input);
void	input_struct_init(t_input *input);
int		clear_struct_on_failure(t_input *input);
char	*normalize_tabs(const char *line);
int		count_words(char **words);

#endif