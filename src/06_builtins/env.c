#include "minishell.h"

	//with env builtin we just print the env
	// but there is a rule, that we only print keys which has values
	//so if value = NULL, then we skip printing

void	print_env(t_shell *shell)
{
	t_env	*env_list;

	env_list = shell->env_head;
	
	while (env_list)
	{
		if (env_list->key && env_list->value)
			ft_printf("%s=%s\n", env_list->key, env_list->value);
		env_list = env_list->next;
	}
}