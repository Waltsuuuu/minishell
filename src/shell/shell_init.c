#include "minishell.h"

int	shell_init(t_shell *shell, char *envp[])
{
	if (!shell)
		return (-1);
	ft_memset(shell, 0, sizeof(*shell));
	shell->last_status = 0;
	//if envar setup doesnt work, return  -1
	setup_signal_handlers_for_prompt();
	shell->env = envp; //TODO make linked list of this 
	
	return (0);
	
}