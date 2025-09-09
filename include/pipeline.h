#ifndef PIPELINE_H
# define PIPELINE_H

#include "libft.h"

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char 			*target;	// Filename OR heredoc delimiter
	int				no_expand;	// heredoc: 1 if delimiter was quoted.
}	t_redir;

typedef struct s_command
{
	char	**argv; // NULL-terminated array of tokens.
	int		argc;
	t_list	*redirs; // Pointer to the redirct struct (If redirect)
}	t_command;

typedef struct s_pipeline
{
	int			n_cmds; // Number of commands
	t_command	*cmds;	// Array of structured commands
}	t_pipeline;

typedef struct s_seg {
	t_list *args;     /* list of char* */
	int     argc;     /* count of args */
	t_list *redirs;   /* list of t_redir* */
	int     start;    /* token index at segment start (for “empty” check) */
} t_seg;

int	build_pipeline(t_input *input, t_token *tokens, t_pipeline *pipeline);
int	arg_ll_to_arr(t_seg *seg, t_pipeline *pipeline, int cmd_i);
int	append_arg(t_token *tokens, int i, t_seg *seg);
int	build_and_append_redir(t_token *tokens, int i, t_seg *seg);
int	get_redir_type(TokType type, t_redir_type *t_type);
int	is_redir_tok(TokType type);
int	count_pipes(t_input *input);

// printer - debugger
void	print_cmds(const t_pipeline *pipeline);
void	print_one_cmd(const t_command *cmd, int idx);
void	print_redirs(const t_list *lst);
const char	*redir_type_name(t_redir_type t);


#endif