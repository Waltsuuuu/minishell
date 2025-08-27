/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 10:08:48 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/08/27 14:46:45 by wheino           ###   ########.fr       */
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
	TOK_REDIR_OUT,	// >
	TOK_REDIR_APP,	// >>
	TOK_HEREDOC		// <<
}	TokType;

/* Temporary linked list of TOKENS, once all tokens extracted, allocate space for t_token *tokens array */
typedef struct s_toknode
{
	t_token             tok;
	struct s_toknode    *next;
}		t_toknode;

typedef struct s_token {
	TokType type;	// Token type
	char	*text;	// The token as a string
	int		pos;	// Start index of token in *raw string.
}		t_token;

typedef struct s_input {
	char	*raw;		// Original line the user typed in.
	char	**words;	// Array of words (split from the original line).
	int		count;		// Number of words.
	t_token	*tokens;	// Array of tokens.
	int		n_tokens;	// Number of tokens.
}		t_input;

typedef struct s_tokenizer_state
{
	const char	*line;		// Raw input string
	int			index;		// Current index
	int			in_single;	// Bool flag to declare if token is inside single quotes
	int			in_double;	// Bool flag to declate if token is inside double quotes
	int			word_start;	// Start index of the current WORD
	t_token		*tokens;	// Array of tokens
	int			n_tokens;	// Number of tokens
	t_toknode	*head;		// Head of token linked list
	t_toknode	*tail;		// Tail of token linked list
	int			list_count;	// Number of nodes (tokens);
}		t_tokenizer_state;

//TODO move to executor.h
char	*join_cmd_to_path(const char *path, const char *cmd);
char	**find_from_path(char *envp[]);
char	**build_absolute_paths(char **paths, const char *cmd);
void	exec_ext_func(char **absolute_paths, char **words, char *envp[]);
//TODO move to utils.h
void	free_split(char **arr);
void	free_partial(char **arr, size_t n);


// Parser
int		parse_input_line(const char *line, t_input *input);
void	input_struct_zero(t_input *input);
int	fill_words_from_line(t_input *input, const char *line);
int		clear_struct_on_failure(t_input *input);
void	free_tokens(t_input *input);
char	*normalize_tabs(const char *line);
int		count_words(char **words);

// Tokenizer
int		tokenize_line(const char *line, t_token **out, int *out_count);
int		init_tokenizer_state(t_tokenizer_state *state, const char *line);
int		run_tokenizer(t_tokenizer_state *state);
int		skip_blanks(const char *string, int i);
int		get_operator_len(const char *string);
int		set_operator(t_tokenizer_state *state);
TokType	get_operator_type(const char *string, int len);
int		set_word(t_tokenizer_state *state);
int		scan_word_end(const char *string, int i, int *in_single, int *in_double);
int		append_token_node(t_tokenizer_state *state, TokType type, char *text, int pos);
int		convert_lst_to_arr(t_tokenizer_state *state);
int		clear_tokenizer_state(t_tokenizer_state *state);
int		free_token_array(t_tokenizer_state *state);
int		free_token_list(t_tokenizer_state *state);


#endif