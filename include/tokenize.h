/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 11:23:32 by wheino            #+#    #+#             */
/*   Updated: 2025/10/09 11:29:54 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZE_H
# define TOKENIZE_H

typedef enum e_tok_type
{
	TOK_WORD,
	TOK_PIPE,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_REDIR_APP,
	TOK_HEREDOC,
	TOK_VOID
}	e_tok_type;

typedef struct s_token
{
	e_tok_type	type;
	char		*text;
	int			pos;
	int			was_quoted; 
}		t_token;

typedef struct s_toknode
{
	t_token				tok;
	struct s_toknode	*next;
}		t_toknode;

typedef struct s_input
{
	char	*raw;
	t_token	*tokens;
	int		n_tokens;
}		t_input;

typedef struct s_tokenizer_state
{
	const char	*line;
	int			index;
	int			in_single;
	int			in_double;
	int			word_start;
	t_token		*tokens;
	int			n_tokens;
	t_toknode	*head;
	t_toknode	*tail;
	int			list_count;
}		t_tokenizer_state;

// 00_TOKENIZE
/*					parse_input.c					*/
int		parse_input_line(const char *line, t_input *input);
void	input_struct_zero(t_input *input);
int		clear_struct_on_failure(t_input *input);
void	free_tokens(t_input *input);

/*					tokenize.c						*/
int		tokenize_line(const char *line, t_input *input);
int		run_tokenizer(t_tokenizer_state *state);
int		set_operator(t_tokenizer_state *state);
int		set_word(t_tokenizer_state *state);
int		convert_lst_to_arr(t_tokenizer_state *state);

/*					tok_init_free.c					*/
int		init_tokenizer_state(t_tokenizer_state *state, const char *line);
int		clear_tokenizer_state(t_tokenizer_state *state);
int		free_token_array(t_tokenizer_state *state);
int		free_token_list(t_tokenizer_state *state);

/*					tok_utils.c						*/
int			scan_word_end(const char *string, int i, int *in_single, int *in_double);
int			skip_blanks(const char *string, int i);
int			get_operator_len(const char *string);
e_tok_type	get_operator_type(const char *string, int len);
int			append_token_node(t_tokenizer_state *state, e_tok_type type, char *text, int pos);

#endif