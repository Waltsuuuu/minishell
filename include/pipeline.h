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
	t_list	*redirs; // Pointer to the redirct struct (If redirect)
}	t_command;

typedef struct s_pipeline
{
	int			n_cmds; // Number of commands
	t_command	*cmds;	// Array of structured commands
}	t_pipeline;

#endif