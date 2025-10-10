/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 12:26:15 by wheino            #+#    #+#             */
/*   Updated: 2025/10/10 12:56:59 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <errno.h>
# include <fcntl.h>
# include <termios.h>
# include <limits.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "libft.h"
# include "tokenize.h"
# include "expand.h"
# include "pipeline.h"
# include "heredoc.h"
# include "quotes.h"
# include "execute.h"
# include "utils.h"
# include "printf/ft_printf.h"
# include "../libft/get_next_line/get_next_line.h"
# include "builtin.h"
# include "debug.h" // REMOVE

# define PIPE_BUF_MAX 63000

extern volatile sig_atomic_t	g_signal;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				assigned;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	int					status;
	char				*cwd;
	char				*buf;
	char				*line;
	t_input				input;
	t_pipeline			pipeline;
	int					last_status;
	int					interactive;
	t_env				*env_head;
	char				**env_arr;
	int					env_size;
	int					in_child;
}	t_shell;

void	init_shell(t_shell *shell, char *envp[]);
int		run_shell(t_shell *shell);
int		prompt(t_shell *shell);
int		parse(t_shell *shell);
int		parse_tokenize(t_shell *shell);
int		parse_rm_quotes_and_expand(t_shell *shell);
int		build_pipeline_collect_hd(t_shell *shell);

// 00_BALANCE_QUOTES
/*					See quotes.h 								*/

// 01_TOKENIZE
/*					See tokenizer.h 							*/

// 02_EXPAND_VARS_AND_STATUS
/*					See expand.h 								*/

// 03_REMOVE_QUOTES
/*					See quotes.h 								*/

// 04_BUILD_CMD_PIPELINE
/*					See pipeline.h								*/

// 05_COLLECT_HEREDOCS
/*					See heredoc.h								*/

// 06_EXECUTE_CMD_PIPELINE
/*					See execute.h								*/

// 07_BUILTINS
/*					See builtin.h								*/

// UTILS
/*					See utils.h									*/
void	kill_and_reap_children(pid_t *pids, int n);
void	execve_error_and_exit(t_shell *shell, char **argv,
			pid_t *child_pids, int saved_errno);
int		has_slash(char *input);
void	clean(char **directories, t_shell *shell, pid_t *child_pids);
int		is_interactive(t_shell *shell);

#endif