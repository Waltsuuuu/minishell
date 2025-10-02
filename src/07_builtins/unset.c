#include "minishell.h"

int	exec_unset_in_parent(t_command *cmd, t_shell *shell)
{
	int	saved[2];

	if (!cmd || !shell)
		return (1);
	if (apply_redirs_in_parent(cmd, saved) != 0)
		return (1);
	shell->last_status = builtin_unset(cmd->argv, shell);
	restore_stdio(saved);
	return (shell->last_status);
}

static int	is_valid_unset_name(const char *key)
{
    int counter;

    if (!key || !key[0])
        return (0);
    if (!(ft_isalpha((unsigned char)key[0]) || key[0] == '_'))
        return (0);
    counter = 1;
    while (key[counter])
    {
        if (!ft_isalnum((unsigned char)key[counter]) && key[counter] != '_')
            return (0);
        counter++;
    }
    return (1);
}

int	builtin_unset(char **argv, t_shell *shell)
{
	int	counter;

	if (!argv[1] || *argv[1] == '\0') //if no arguments
		return (0);
	counter = 1; // first argument
	while (argv[counter])
	{
	if (!is_valid_unset_name(argv[counter]))
	{
		print_invalid_identifier("unset", argv[counter]);
		return (1);
	}
	else 
	{
		env_unset(shell, argv[counter]);
	}
	counter++;
	}
	return (0);

}

int	env_unset(t_shell *shell, const char *key)
{
	//loop through the linked list, check if we find the key, lets use pointer to pointer, so we can directly modify the list
	t_env	**ptr;
	t_env	*cur;

	if (!shell || !key || !*key)
    	return (0);
	
	ptr = &shell->env_head;  // start at the address of the head pointer
	while (*ptr)
	{
		cur = *ptr; // read the current node
		if (ft_strcmp((cur->key), key) == 0)
		{
			*ptr = cur->next; //We link here before free
			free (cur->key);
			free (cur->value);
			free (cur);
			return (0);
		}
		ptr = &cur->next; //if we didnt get a match we go forward in a loop
	}
	return (0);
}