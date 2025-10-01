#include "minishell.h"

int	builtin_echo(t_command *cmd)
{
{
	int i;
	int nflag;

	i = 1;
	nflag = 0;
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	while (cmd->argv[i] && cmd->argv[i][0] == '-' && cmd->argv[i][1] == 'n') //go through multiple n blocks
	{
		int j = 1;
		while (cmd->argv[i][j] == 'n')
			j++;
		if (cmd->argv[i][j] != '\0')
			break;
		nflag = 1;
		i++;
	}
	while (cmd->argv[i])
	{
		ft_putstr_fd(cmd->argv[i], 1);
		if (cmd->argv[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (!nflag)
		ft_putstr_fd("\n", 1);
	return (0);
}
}

int	exec_echo_in_parent(t_command *cmd, t_shell *shell)
{
	int	saved[2];

	if (!cmd || !shell)
		return (1);
	if (apply_redirs_in_parent(cmd, saved) != 0)
		return (1);
	shell->last_status = builtin_echo(cmd);
	restore_stdio(saved);
	return (shell->last_status);
}