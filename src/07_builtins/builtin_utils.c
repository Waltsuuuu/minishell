#include "minishell.h"

int	run_builtin(t_command *cmd, t_shell *shell)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (builtin_export(cmd->argv, shell));
	if (ft_strcmp(cmd->argv[0], "unset") == 0)
		return (builtin_unset(cmd->argv, shell));
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (builtin_cd(cmd->argv, shell));
	if (ft_strcmp(cmd->argv[0], "exit") == 0)
		return (builtin_exit(cmd, shell));
	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (builtin_echo(cmd));
	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (builtin_pwd());
	return (127);
}

int	is_builtin_name(const char *name)
{
	if (!name || !*name)
		return (0);
	if (ft_strcmp(name, "export") == 0)
		return (1);
	if (ft_strcmp(name, "unset") == 0)
		return (1);
	if (ft_strcmp(name, "cd") == 0)
		return (1);
	// if (ft_strcmp(name, "exit") == 0)
	// 	return (1);
	if (ft_strcmp(name, "echo") == 0)
		return (1);
	if (ft_strcmp(name, "pwd") == 0)
		return (1);
	return (0);

}

int	is_parent_builtin(const char *name)
{
	if (!name || !*name)
		return (0);
	if (ft_strcmp(name, "export") == 0)
		return (1);
	if (ft_strcmp(name, "unset") == 0)
		return (1);
	if (ft_strcmp(name, "cd") == 0)
		return (1);
	//if (ft_strcmp(name, "echo") == 0)
	 	//return (1);
	return (0);
}
