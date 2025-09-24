#ifndef HEREDOC_H
# define HEREDOC_H

#include <termios.h>

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

int		collect_heredocs(t_pipeline *pipeline, t_shell *shell, char **envp);
int		collect_cmd_heredocs(t_command *cmd, t_shell *shell, char **envp);
int		collect_heredoc_body(t_redir *redir, t_shell *shell, char **envp);
int		handle_heredoc_line(int	fd, char *line, t_redir *redir, int last_status, char **envp);
int		expand_write_line(int fd, char *line, int last_status, char **envp);
void	write_line_nl(int fd, char *line);
void	free_line_close_fds(int fds[2], char *line);
void	heredoc_sigint(int signum);
void	setup_sig_handlers_for_heredoc(void);

#endif