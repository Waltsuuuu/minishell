#ifndef QUOTES_H
#define QUOTES_H

typedef struct s_input	t_input;

// 00_CHECK_QUOTE_BALANCE
/* 					quote_check.c								*/
int check_quote_balance(char **line, int *last_status);
int	quotes_unbalanced(const char *string);

// 03_REMOVE_QUOTES
/*					quote_removal.c								*/
int		remove_quotes(t_input *input);
char	*handle_quote_removal(char *text, int *was_quoted);
int		create_unquoted_text(char *text, char **unquoted_text, int *was_quoted);
int		detect_outer_quote(char c, int *in_single, int *in_double);

#endif