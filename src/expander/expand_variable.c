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
	t_expand_var_state st;

	init_expand_state(&st);
	while (text[st.i] != '\0')
	{
		st.quote_handled = process_quote_char(text[st.i], &st.in_single, &st.in_double, exp_text);
		if (st.quote_handled == -1)
			return (-1);
		if (st.quote_handled == 1)
		{
			st.i++;
			continue ;
		}
		st.expanded = handle_var_expansion(text, exp_text, &st.i, envp, st.in_single);
		if (st.expanded == -1)
			return (-1);
		if (st.expanded == 1)
			continue ;
		if (process_char(exp_text, text[st.i]) == -1)
			return (-1);
		st.i++;
	}
	return (0);
}

void	init_expand_state(t_expand_var_state *st)
{
	st->i = 0;
	st->in_single = 0;
	st->in_double = 0;
}

int	handle_var_expansion(char *text, char **exp_text, int *i, char **envp, int in_single)
{
	int	status;

	status = 0;
	if (!in_single && text[*i] == '$' && valid_start_char(text[*i + 1]) == 1)
	{
		(*i)++;
		status = process_var_expansion(text, exp_text, i, envp);
		if (status == -1)
			return (-1);
		return (1);
	}
	return (0);
}

int	process_var_expansion(char *text, char **exp_text, int *i, char **envp)
{
 	char	*key;
 	int		key_len;
 	int		start_i;
 	int		env_i;

	key = NULL;
	if (extract_key(text, i, &key, &key_len, &start_i) == -1)
		return (-1);
	env_i = find_env_index(envp, key, key_len);
	if (env_i >= 0)
	{
		if (process_expanded_str(exp_text, &envp[env_i][key_len + 1]) == -1)
		{
			free(key);
			return (-1);
		}
		free(key);
		return (1);
	}
	free(key);
	return (0);
}

int	extract_key(char *text, int *i, char **key, int *key_len, int *start_i)
{
	*start_i = *i;
	*key_len = 0;
	while (valid_cont_char(text[*i]) == 1)
	{
		(*i)++;
		(*key_len)++;
	}
	*key = malloc(*key_len + 2); // +2 for '=' and '\0'
	copy_n_chars(*key, &text[*start_i], *key_len);
	(*key)[*key_len] = '=';
	(*key)[*key_len + 1] = '\0';
	return (0);
}

int	find_env_index(char **envp, char *key, int key_len)
{
	int	env_i;

	env_i = 0;
	while (envp[env_i])
	{
		if (ft_strncmp(envp[env_i], key, key_len + 1) == 0)
			return (env_i);
		env_i++;
	}
	return (-1);
}
