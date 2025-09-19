#include "minishell.h"

//without arguments print env
//export KEY=VALUE or export KEY = modify the env

//Arrange in alphabetical order if not already CHECK THIS 
//KEY=VALUE, if key is already in the list -> update it, if not create a new node 

int	run_export(t_shell *shell)
{
	if (!is_builtin_valid(arg))
{
    print_invalid_identifier("export", arg);
    shell->last_status = 1;
    continue;
}


}

int		is_builtin_valid(const char *key) // this is used in both export and unset
{

	int	counter;

	counter = 0;
	if (ft_isalpha(key[0]) || key[0] == '_')
	{
		counter++;
		while (key[counter])
		{
			if (!ft_isalnum(key[counter]) && key[counter] != '_')
			{
				//error message
				return (0);
			}
			counter++;
		}
		return (1);
	}
	else
	{
		//Error message
		return (0);
	}

}

void	print_invalid_identifier(const char *builtin, const char *key)
{
	write(2, builtin, ft_strlen(builtin));
	write(2, ": `", 3);
	write(2, key, ft_strlen(key));
	write(2, "': not a valid identifier\n", 26);
}
