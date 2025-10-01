
#include "minishell.h"

static int	is_interactive(t_shell *shell)
{
	if (shell && shell->interactive)
		return (1);
	return (isatty(STDIN_FILENO));
}

int	builtin_exit(t_command *cmd, t_shell *shell)
{
	int			status;
	long long	value;

	if (!cmd || !shell)
		return (1);

	if (!cmd->argv[1])
	{
		status = (unsigned char)shell->last_status; // by casting to unsigned char we get the 0-255 value
		if (shell->in_child)
			_exit(status);
		if (is_interactive(shell))
			ft_putstr_fd("exit\n",1);
		//CLEANUP HERE
		exit(status);
	}
		//validate the first arg
	if (!ft_strtoll(cmd->argv[1], &value))
	{
		if (!shell->in_child && is_interactive(shell))
			ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd->argv[1] ,2);
		ft_putstr_fd(": numeric argument required\n", 2);
		//CLEANUP HEERE
		if (shell->in_child)
			_exit(2);
		exit(2);
	}
	if (cmd->argv[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		shell->last_status = 1;
		return(1); //do not exit, just return 1
		//we do not exit here
	}
	//if right amount arg, and arg validated to be ok
	status = (unsigned char)value;
	if (shell->in_child)
		_exit(status);
	if (is_interactive(shell))
		ft_putstr_fd("exit\n", 1);
	//CLEANUP!
	exit(status);
}

int exec_exit_in_parent(t_command *cmd, t_shell *shell)
{
    int saved[2];

    if (!cmd || !shell)
        return (1);

    shell->in_child = 0;
    if (apply_redirs_in_parent(cmd, saved) != 0)
        return (1);

    shell->last_status = builtin_exit(cmd, shell);

    restore_stdio(saved);
    return (shell->last_status);
}

