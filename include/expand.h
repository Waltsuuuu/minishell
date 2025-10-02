#ifndef EXPAND_H
# define EXPAND_H

typedef struct s_expand_state
{
	int	i;
	int	in_single;
	int	in_double;
	int	quote_handled;
	int	expanded;
}	t_expand_state;

/* 					01_expansion.c								*/
int		expand_tokens(t_input *input, int last_status, char **envp);

/* 					02_expand_status.c							*/
char	*expand_status(char *text, int last_status);
int		create_exp_status_text(char *text, char **exp_text, char *status_str);

/* 					03_expand_variable.c						*/
char	*expand_variable(char *text, char **envp);
int		create_exp_var_text(char *text, char **exp_text, char **envp);
int		handle_var_expansion(char *text, char **exp_text, int *i, char **envp, int in_single, int in_double);
int		process_var_expansion(char *text, char **exp_text, int *i, char **envp);

/* 					04_expand_utils.c							*/
int		valid_cont_char(char c);
int		valid_start_char(char c);
int		process_expanded_str(char **exp_text, const char *str);
int		process_quote_char(char c, int *in_single, int *in_double, char **exp_text);
int		process_char(char **exp_text, char c);

/* 					05_expand_utils2.c							*/
void	init_expand_state(t_expand_state *st);
size_t	copy_n_chars(char *dst, const char *src, size_t size);
int		extract_key(char *text, int *i, char **key, int *key_len, int *start_i);
int		find_env_index(char **envp, char *key, int key_len);

#endif