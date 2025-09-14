#ifndef TOKENIZER_H
# define TOKENIZER_H

typedef enum {
	TOK_WORD,		// text
	TOK_PIPE,		// |
	TOK_REDIR_IN,	// <
	TOK_REDIR_OUT,	// >
	TOK_REDIR_APP,	// >>
	TOK_HEREDOC		// <<
}	TokType;

typedef struct s_token {
	TokType type;	// Token type
	char	*text;	// The token as a string
	int		pos;	// Start index of token in *raw string.
	int		was_quoted; // Flag to signal if token had quotes (WORD TOKENS ONLY)  
}		t_token;

// Temporary linked list of TOKENS,
// once all tokens extracted, tok is moved to t_token array.
typedef struct s_toknode
{
	t_token             tok;
	struct s_toknode    *next;
}		t_toknode;

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

// Parser
int		parse_input_line(const char *line, t_input *input);
void	input_struct_zero(t_input *input);
int		fill_words_from_line(t_input *input, const char *line);
int		clear_struct_on_failure(t_input *input);
void	free_tokens(t_input *input);
char	*normalize_tabs(const char *line);
int		count_words(char **words);

// TOKENIZE.C
int		tokenize_line(const char *line, t_token **out, int *out_count);
int		run_tokenizer(t_tokenizer_state *state);
int		set_operator(t_tokenizer_state *state);
int		set_word(t_tokenizer_state *state);
int		convert_lst_to_arr(t_tokenizer_state *state);

// TOK_INIT_FREE.C
int		init_tokenizer_state(t_tokenizer_state *state, const char *line);
int		clear_tokenizer_state(t_tokenizer_state *state);
int		free_token_array(t_tokenizer_state *state);
int		free_token_list(t_tokenizer_state *state);

// TOK_UTILS.C
int		scan_word_end(const char *string, int i, int *in_single, int *in_double);
int		skip_blanks(const char *string, int i);
int		get_operator_len(const char *string);
TokType	get_operator_type(const char *string, int len);
int		append_token_node(t_tokenizer_state *state, TokType type, char *text, int pos);

// TOKEN_DEBUGGER.C
void print_tokens(const t_input *in);

#endif