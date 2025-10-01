
#include "minishell.h"

int	mini_exit(t_command *cmd, t_shell *shell)
{
	int			status;
	long long	value;

	if (!cmd->argv[1])
	{
		status = (unsigned char)shell->last_status; // by casting to unsigned char we get the 0-255 value
		//and somehow check if its parent or child
		//cleanup here
		exit(status);
	}
	//validate the first arg
	if (!ft_strtoll(cmd->argv[1], &value))
	{
		//make a wrapper out of this 
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd->argv[1] ,2);
		ft_putstr_fd(": numeric argument required", 2);
		exit(255);

	}
	//if argv1 is valid, but we have another arg
	if (cmd->argv[2])
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd("minishell: exit: too many arguments");
		return(1); //do not exit, just return 1
		//we do not exit here
	}
	//if right amount arg, and arg validated to be ok
	status = (unsigned char)value;
	//we neeed to check if child or parent. Parent prints exit, child doesnt.
	exit(status);

	 


}
