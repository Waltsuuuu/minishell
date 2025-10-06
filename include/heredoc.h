#ifndef HEREDOC_H
# define HEREDOC_H

typedef struct s_shell    t_shell;
typedef struct s_pipeline t_pipeline;
typedef struct s_command  t_command;
typedef struct s_redir    t_redir;

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
}	t_hd_state;

// COLLECT_HEREDOCS
int		collect_heredocs(t_pipeline *pipeline, t_shell *shell, char **envp);
int		collect_cmd_heredocs(t_command *cmd, t_shell *shell, char **envp);
int		collect_heredoc_body(t_redir *redir, t_shell *shell, char **envp);

// HD_INPUT_READ_WRITE
int		handle_heredoc_line(int	fd, char *line, t_redir *redir, int last_status, char **envp);
int		fork_and_collect_hd(t_hd_state *state, t_shell *shell, t_redir *redir, char **envp);
int		readline_and_check_eof(t_hd_state *state, t_redir *redir);
int		expand_write_line(int fd, char *line, int last_status, char **envp);
void	write_line_nl(int fd, char *line);

// HD_SIGNAL_UTILS
void	ignore_parent_sig_handlers(t_hd_state *state);
void	heredoc_child_sighandler(void);
int		wait_child(t_hd_state *state);
void	restore_parent_sig_handlers(t_hd_state *state);
int		handle_child_status(t_hd_state *state, t_shell *shell);
void	close_stdin_on_sigint(int sig_num);

// HD_TTY_UTILS
void	save_terminal_state(struct termios *tty);
void	restore_terminal_state(t_hd_state *state);

// HD_UTILS
void	init_hd_state(t_hd_state *state);
int		close_pipe_err(t_hd_state *state);
void	free_line_close_fds(int fds[2], char *line);
void	restore_tty_and_sig(t_hd_state *state);

#endif