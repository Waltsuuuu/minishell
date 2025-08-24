/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 10:08:48 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/08/24 16:30:33 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "colors.h"
# include <sys/wait.h>
# include <sys/stat.h> // access

typedef enum {
	TOK_WORD,		// text
	TOK_PIPE,		// |
	TOK_REDIR_IN,	// <
	TOK_REDIT_OUT,	// >
	TOK_REDIT_APP,	// >>
	TOK_HEREDOC		// <<
}	TokType;

typedef struct s_token {
	TokType type;	// Token type
	char	*text;	// The "word"
	int		pos;	// Start index of token in *raw string.
}		t_token;

typedef struct s_input {
	char	*raw;		// Original line the user typed in.
	char	**words;	// Array of words (split from the original line).
	int		count;		// Number of words.
	t_token	*tokens;	// Array of tokens.
	int		n_tokens;	// Number of tokens.
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
int	parse_input_line(const char *line, t_input *input);
void	input_struct_zero(t_input *input);
int	clear_struct_on_failure(t_input *input);
void	free_tokens(t_input *input);
char	*normalize_tabs(const char *line);
int	count_words(char **words);

#endif