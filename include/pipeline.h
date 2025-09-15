#ifndef PIPELINE_H
# define PIPELINE_H

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
	int				hd_fd; 		
}	t_redir;

typedef struct s_command
{
	char	**argv;
	int		argc;
	t_list	*redirs;
}	t_command;

typedef struct s_pipeline
{
	int			n_cmds;
	t_command	*cmds;
}	t_pipeline;

typedef struct s_seg {
	t_list *args; 
	int     argc;
	t_list *redirs;
	int     start;
} t_seg;

// build_pipeline.c
int		build_pipeline(t_input *input, t_token *tokens, t_pipeline *pipeline);
int		bp_prepare(t_input *input, t_token *tokens, t_pipeline *pipeline);
void	bp_seg_init(t_seg *seg, int start);
int		bp_fill_segment(t_input *input, t_token *tokens, t_seg *seg, int *i);
int		bp_finalize_command(t_seg *seg, t_pipeline *pipeline, int cmd_i, t_token *tokens, int i);

// pipeline_utils.c
int		get_redir_type(TokType type, t_redir_type *t_type);
int		is_redir_tok(TokType type);
int		count_pipes(t_input *input);

// pipeline_args.c
int		arg_ll_to_arr(t_seg *seg, t_pipeline *pipeline, int cmd_i);
int		append_arg(t_token *tokens, int i, t_seg *seg);

// pipeline_redirs.c
int		build_and_append_redir(t_token *tokens, int i, t_seg *seg);
int		fill_redir_fields(t_redir *redir, t_token *tokens, int i);

// exit_build_pipeline.c
int		err_exit_build_pipeline(t_pipeline *pipeline, t_seg *seg, int built_count);
void	free_partial_seg(t_seg *seg);
void	del_redir(void *content);
void	free_pipeline(t_pipeline *pipeline, int	built_count);

// // printer - debugger
void		print_cmds(const t_pipeline *pipeline);
void		print_one_cmd(const t_command *cmd, int idx);
void		print_redirs(const t_list *lst);
const char	*redir_type_name(t_redir_type t);

#endif