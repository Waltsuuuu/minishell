#include "minishell.h"

int	builtin_pwd(void)
{
	char	buf[PATH_MAX];
	char	*cwd;

	cwd = getcwd(buf, sizeof(buf));
	if (!cwd)
		return(perror("pwd"), 1);
	ft_putstr_fd(cwd, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	// printf("%s\n", cwd);
	return (0);
}