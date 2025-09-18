#include "minishell.h"

/*
	split key and value:

	-First we loop through from start of the envp till '=', now we have our key.
	-Then we do loop til end of the string '\0', and now we have our VALUE.

	then in our init env function we can check if the key and value split is succesfull,
	if it is, we create a node with key and value in it.
	!!!guard against if node creation fails!!!
	remember to properly free the allocated key and value, before extracting another key and value. 

	



*/


static	int	split_key_and_value(char *line, char **key_out, char **value_out)
{
	int	counter;

	counter = 0;
	//first get the key out.
	while (*line && *line != '=')
	{
		counter++;
		line++;
	} //loop til '='
		
	key_out = malloc(counter + 1);
	ft_strlcpy(*key_out, line, counter +1); // now we should have the key value, next go for value!

	if (line[counter] == '=')
	{
		*value_out = ft_strdup(line[counter+1]);
		if (!value_out)
			return (-1);
	}
	else
	{
		//if there is no value inside return empty string
		*value_out = ft_strdup("");
		if (!value_out)
			return (-1);
	}
	return (0);
}

t_env *env_init_from_envp(char **envp)
{
	//in here we do the key splitting, node creation and the appending
	int		i;
	char	*key;
	char	*value;
	t_env	*node;
	t_env	*env_head;

	i = 0;
	while (envp && envp[i])
	{
		//initalize keys every round to null, and free the old keys after everyround.
		if (split_key_and_value(envp[i], &key, &value) == 0) 
		{
			//so if the split is succesfull, then we do create the node with the parsed key and value.
			//so here we need a node creation function
			node = create_new_env_node(key, value); //TODO create this function
			// then we need to append the node to the list
			if (!node || append_env_node(&env_head, node) != 0 ) //if node creation fails, or append node fails, we clean and exit.
			{
				free (key);
				free (value);
				return (NULL);
			}
			free (key);
			free (value);
		}
		i++;
	}
}

t_env	*create_new_env_node(const char *key, const char *value)
{
	t_env	*env_node;

	env_node = malloc(sizeof(*env_node));
	if (!env_node)
		return (NULL);
	if (!key || !value)
		return (NULL);
	env_node->key = ft_strdup(key);
	env_node->value = ft_strdup(value);
	env_node->next = NULL;
	if (!env_node->key || !env_node->value)
	{
		free (env_node->key);
		free (env_node->value);
		free (env_node);
	}
	return (env_node);

}

int	append_env_node(t_env **head, t_env *new_env_node)
{
	t_env	*temp_node;

	if (!*head)
	{
		*head = new_env_node;
		return ;
	}
	temp_node = *head;
	while (temp_node->next != NULL)
		temp_node = temp_node->next;
	temp_node->next = new_env_node;
	return (0); //check if succeeds in env_init_from envp
}

//need free function too for the env
