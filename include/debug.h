#ifndef DEBUG_H
# define DEBUG_H

#include "minishell.h"

// TOKEN DEBUGGER
void print_tokens(const t_input *in);

// PIPELINE CMDS DEBUGGER
void			print_cmds(const t_pipeline *pipeline);
void			print_one_cmd(const t_command *cmd, int idx);
void			print_redirs(const t_list *lst);
const char	*	redir_type_name(t_redir_type t);

#endif