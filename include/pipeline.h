/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 11:11:27 by wheino            #+#    #+#             */
/*   Updated: 2025/10/09 11:29:34 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPELINE_H
# define PIPELINE_H

typedef struct s_shell	t_shell;

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
	char			*target;
	int				no_expand;
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
	int			pipe_pair[2];
	pid_t		*child_pids;
}	t_pipeline;

typedef struct s_seg
{
	t_list	*args;
	int		argc;
	t_list	*redirs;
}	t_seg;

// build_pipeline.c
int		build_pipeline(t_shell *shell, t_input *input,
			t_token *tokens, t_pipeline *pipeline);
int		bp_prepare(t_shell *shell, t_input *input,
			t_token *tokens, t_pipeline *pipeline);
void	bp_seg_init(t_seg *seg);
int		bp_fill_segment(t_shell *shell, t_input *input, t_seg *seg, int *i);
int		bp_finalize_command(t_seg *seg, int cmd_i, t_shell *shell, int i);

// pipeline_utils.c
int		get_redir_type(e_tok_type type, t_redir_type *t_type);
int		is_redir_tok(e_tok_type type);
int		count_pipes(t_input *input);
void	tok_syntax_err(t_token *token, t_shell *shell);

// pipeline_args.c
int		arg_ll_to_arr(t_seg *seg, t_pipeline *pipeline, int cmd_i);
int		append_arg(t_token *tokens, int i, t_seg *seg);

// pipeline_redirs.c
int		build_and_append_redir(t_token *tokens, int i, t_seg *seg);
int		fill_redir_fields(t_redir *redir, t_token *tokens, int i);

// exit_build_pipeline.c
int		err_exit_build_pipeline(t_pipeline *pipeline, t_seg *seg,
			int built_count);
void	free_partial_seg(t_seg *seg);
void	del_redir(void *content);
void	free_pipeline(t_pipeline *pipeline, int built_count);

#endif
