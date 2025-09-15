#include "minishell.h"

void	getworkindir(char *buf, size_t size)
{
	if (NULL == getcwd(buf, size))
		perror("getwcd FAILED");
}