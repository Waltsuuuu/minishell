#include "minishell.h"


int	builtin_cd(char **argv, t_shell *shell)
{
	char	oldbuf[PATH_MAX];
	char	newbuf[PATH_MAX];
	char	*path;

	if (argv[2])
		return (ft_putstr_fd("cd expects exactly one path.\n", 2), 1);

	path = argv[1];
	if (argv[0] && !argv[1])
	{
		path = env_get(shell, "HOME");
		printf("%s\n", path);
	}
	if (!getcwd(oldbuf, sizeof(oldbuf)) && !path)
		return (ft_putstr_fd("cd: getwcd\n", 2), 1);

	if (chdir(path) < 0)
		return (ft_putstr_fd("cd error\n", 2), 1);

	if (!getcwd(newbuf, sizeof(newbuf)))
		return (ft_putstr_fd("getwcd error\n",2), 1);
	
	if (env_set(&shell->env_head, "OLDPWD", oldbuf) == 1)
		return (ft_putstr_fd("oldpwd update error\n", 2), 1);
		
	if (env_set(&shell->env_head, "PWD", newbuf) == 1)
		return (ft_putstr_fd("pwd update error\n", 2), 1);

	return (0);

}

int	exec_cd_in_parent(t_command *cmd, t_shell *shell)
{
	int	saved[2];

	if (!cmd || !shell)
		return (1);
	if (apply_redirs_in_parent(cmd, saved) != 0)
		return (1);
	shell->last_status = builtin_cd(cmd->argv, shell);
	restore_stdio(saved);
	return (shell->last_status);
}