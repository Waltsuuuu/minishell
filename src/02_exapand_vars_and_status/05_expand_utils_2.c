#include "minishell.h"

void	init_expand_state(t_expand_state *st)
{
	st->i = 0;
	st->in_single = 0;
	st->in_double = 0;
}

size_t	copy_n_chars(char *dst, const char *src, size_t size)
{
	size_t	counter;
	size_t	src_len;

	src_len = ft_strlen(src);
	if (size != 0)
	{
		counter = 0;
		while ((src[counter] != '\0') && (counter < size))
		{
			dst[counter] = src[counter];
			counter++;
		}
		dst[counter] = '\0';
	}
	return (src_len);
}

/**
 * @brief Extract variable name (KEY) at *i and return malloc'ed "KEY=".
 * @return 0 on success, -1 on allocation error.
 */
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
	if (!*key)
		return (-1);
	copy_n_chars(*key, &text[*start_i], *key_len);
	(*key)[*key_len] = '=';
	(*key)[*key_len + 1] = '\0';
	return (0);
}

/**
 * @brief Return index of env entry matching "KEY=", or -1 if absent.
 */
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