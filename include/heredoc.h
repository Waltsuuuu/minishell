/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 12:40:11 by wheino            #+#    #+#             */
/*   Updated: 2025/10/10 12:40:56 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

typedef struct s_shell		t_shell;
typedef struct s_pipeline	t_pipeline;
typedef struct s_command	t_command;
typedef struct s_redir		t_redir;

typedef struct s_hd_state
{
	int					fds[2];
	char				*line;
	int					status;
	pid_t				pid;
	struct termios		tty;
	struct sigaction	ign;
	struct sigaction	old_int;
	struct sigaction	old_quit;
	int					wait_result;
	int					pipe_bytes_written;
}	t_hd_state;

// COLLECT_HEREDOCS
int		collect_heredocs(t_pipeline *pipeline, t_shell *shell);
int		collect_cmd_heredocs(t_command *cmd, t_shell *shell);
int		collect_heredoc_body(t_redir *redir, t_shell *shell);
int		collect_hd(t_hd_state *state, t_shell *shell, t_redir *redir);

// HD_INPUT_READ_WRITE
int		handle_heredoc_line(t_hd_state *state, char *line,
			t_redir *redir, t_shell *shell);
int		fork_and_collect_hd(t_hd_state *state, t_shell *shell, t_redir *redir);
int		readline_and_check_eof(t_hd_state *state, t_redir *redir);
int		expand_write_line(t_hd_state *state, char *line, t_shell *shell);
void	write_line_nl(int fd, char *line);

// HD_SIGNAL_UTILS
void	heredoc_child_sighandler(void);
int		wait_child(t_hd_state *state);
int		handle_child_status(t_hd_state *state, t_shell *shell);
void	close_stdin_on_sigint(int sig_num);

// HD_UTILS
void	init_hd_state(t_hd_state *state);
int		close_pipe_err(t_hd_state *state);
void	free_line_close_fds(int fds[2], char *line);
void	restore_tty_and_sig(t_hd_state *state);

// HD_EXPAND_STATUS
char	*hd_expand_status(char *text, int last_status);
int		hd_create_exp_status_text(char *text, char **exp_text,
			char *status_str);

// HD_EXPAND_VARS
char	*hd_expand_variable(char *text, char **envp);
int		hd_create_exp_var_text(char *text, char **exp_text, char **envp);

#endif