#include "minishell.h"

int	pwd(void)
{
	char	buf[PATH_MAX];
	char	*cwd;

	cwd = getcwd(buf, sizeof(buf));
	if (!cwd)
		return(perror("pwd"), 1);
	printf("%s\n", cwd);
	return (0);
}