#include "minishell.h"

int  is_single_export(const t_pipeline *p);
int  save_stdio(int saved[2]);
void restore_stdio(int saved[2]);
int  apply_redirs_in_parent(t_command *cmd, int saved[2]);


int	exec_dispatch(char **envp, t_pipeline *pipeline, t_shell *shell)
{
	if (is_single_export(pipeline))
		return (exec_export_in_parent(&pipeline->cmds[0], shell));
	return (exec_pipeline(envp, pipeline, shell));
}


int	is_single_export(const t_pipeline *p)
{
	if (!p || p->n_cmds != 1)
		return (0);
	return (p->cmds && p->cmds[0].argv && p->cmds[0].argv[0]
		&& ft_strcmp(p->cmds[0].argv[0], "export") == 0);
}

int exec_export_in_parent(t_command *cmd, t_shell *shell)
{
    int saved[2];
    int status;

    if (!cmd || !shell)
        return (1);

    if (apply_redirs_in_parent(cmd, saved) != 0)
        return (1);
    printf("%d\n", cmd->argc);
    if (cmd->argc <= 1 || (cmd->argv[1] && cmd->argv[1][0] == '\0'))
    {
        env_sort_and_print(shell);
        restore_stdio(saved);
        shell->last_status = 0;
        return (0);
    }
    status = builtin_export(cmd->argv, shell);
    restore_stdio(saved);
    shell->last_status = status;
    return (status);
}

int	is_builtin_name(const char *name)
{
	if (!name)
		return (0);
	if (ft_strcmp(name, "export") == 0)
		return (1);
	if (ft_strcmp(name, "unset") == 0)
		return (1);
	if (ft_strcmp(name, "cd") == 0)
		return (1);
	if (ft_strcmp(name, "exit") == 0)
		return (1);
	if (ft_strcmp(name, "echo") == 0)
		return (1);
	if (ft_strcmp(name, "pwd") == 0)
		return (1);
	if (ft_strcmp(name, "env") == 0)
		return (1);
	return (0);
}

int	apply_redirs_in_parent(t_command *cmd, int saved[2])
{
	t_list	*node;
	t_redir	*r;
	int		in_fd;
	int		out_fd;

	if (save_stdio(saved) < 0)
		return (-1);
	in_fd = STDIN_FILENO;
	out_fd = STDOUT_FILENO;
	node = cmd->redirs;
	while (node)
	{
		r = (t_redir *)node->content;
		if (r && r->type == REDIR_OUT && apply_redir_out(r, &out_fd) < 0)
			return (restore_stdio(saved), -1);
		if (r && r->type == REDIR_APPEND && apply_redir_append(r, &out_fd) < 0)
			return (restore_stdio(saved), -1);
		if (r && r->type == REDIR_IN && apply_redir_in(r, &in_fd) < 0)
			return (restore_stdio(saved), -1);
		node = node->next;
	}
	if (in_fd != STDIN_FILENO && dup2(in_fd, STDIN_FILENO) < 0)
		return (restore_stdio(saved), -1);
	if (out_fd != STDOUT_FILENO && dup2(out_fd, STDOUT_FILENO) < 0)
		return (restore_stdio(saved), -1);
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	if (out_fd != STDOUT_FILENO)
		close(out_fd);
	return (0);
}

int	try_run_single_builtin(t_pipeline *pipeline, t_shell *shell)
{
	t_command	*cmd;
	int			saved[2];
	int			status;

	if (!pipeline || pipeline->n_cmds != 1)
		return (0);
	cmd = &pipeline->cmds[0];
	if (!cmd->argv || !cmd->argv[0])
		return (0);
	if (ft_strcmp(cmd->argv[0], "export") != 0)
		return (0);
	if (apply_redirs_in_parent(cmd, saved) < 0)
	{
		shell->last_status = 1;
		return (1);
	}
	shell->last_status = builtin_export(cmd->argv, shell);
	restore_stdio(saved);
	return (1);
}

int	save_stdio(int saved[2])
{
	saved[0] = dup(STDIN_FILENO);
	saved[1] = dup(STDOUT_FILENO);
	if (saved[0] < 0 || saved[1] < 0)
	{
		perror("dup");
		return (-1);
	}
	return (0);
}

void	restore_stdio(int saved[2])
{
	if (saved[0] >= 0)
	{
	dup2(saved[0], STDIN_FILENO);
	close(saved[0]);
	}
	if (saved[1] >= 0)
	{
	dup2(saved[1], STDOUT_FILENO);
	close(saved[1]);
	}
}