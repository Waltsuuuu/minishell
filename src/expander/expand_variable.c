#include "minishell.h"

char *expand_variable(char *text, char **envp)
{
	char	*exp_text;

	if (!text)
		return (ft_strdup(""));
	exp_text = ft_strdup("");
	if (!exp_text)
		return (NULL);
	if (create_exp_var_text(text, &exp_text, envp) == -1)
	{
		free(exp_text);
		return (NULL);
	} 
	return (exp_text);
}

int	create_exp_var_text(char *text, char **exp_text, char **envp)
{
	int	i;
	int	in_single;
	int	in_double;
	int	quote_handled;

	i = 0;
	in_single = 0;
	in_double = 0;
	while (text[i] != '\0')
	{
		quote_handled = process_quote_char(text[i], &in_single, &in_double, exp_text);
		if (quote_handled == -1)
			return (-1);
		if (quote_handled == 1)
		{
			i++;
			continue ;
		}
		if (!in_single && text[i] == '$' && valid_start_char(text[i + 1]) == 1)
		{
			i++;
			if (process_var_expansion(text, exp_text, &i, envp) == -1)
				return (-1);
			continue ;
		}
		if (process_char(exp_text, text[i]) == 1)
			return (-1);
		i++;
	}
	return (0);
}

int	process_var_expansion(char *text, char **exp_text, int *i, char **envp)
{
	char	*key;
	int		key_len;
	char	*value;
	int		value_len;
	int		start_i;
	int		env_i;

	start_i = *i;
	key_len = 0;
	while (valid_cont_char(text[*i]) == 1)
	{
		(*i)++;
		key_len++;
	}
	key = malloc(key_len + 2);
	if (!key)
		return (-1);
	copy_n_chars(key, &text[start_i], key_len);
	key[key_len] = '=';
	key[key_len + 1] = '\0';
	env_i = 0;
	while (envp[env_i])
	{
		if (ft_strncmp(envp[env_i], key, key_len + 1) == 0)           // Var is declared and gets expanded to var value
		{
			value_len = ft_strlen(envp[env_i]) - (key_len + 1);
			value = malloc(value_len + 1);
			if (!value)
			{
				free(key);
				return (-1);
			}
			copy_n_chars(value, &envp[env_i][key_len + 1], value_len);
			process_expanded_str(exp_text, value);
			free(key);
			free(value);
			return (1);
		}
		else
			env_i++;
	}
	free(key);
	return (0);
}

