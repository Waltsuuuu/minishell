#include "minishell.h"


/**
 * @brief Rebuilds token.text, expanding $? to the
 * last_status.
 * @return Newly allocated expanded string, or NULL on error.
 */
char	*hd_expand_status(char *text, int last_status)
{
	char	*exp_text;
	char	*status_str;

	if (!text)
		return (ft_strdup(""));
	exp_text = ft_strdup("");
	if (!exp_text)
		return (NULL);
	status_str = ft_itoa(last_status);
	if (!status_str)
	{
		free(exp_text);
		return (NULL);
	}
	if (hd_create_exp_status_text(text, &exp_text, status_str) == -1)
	{
		free(status_str);
		free(exp_text);
		return (NULL);
	}
	free(status_str);
	return (exp_text);
}

/**
 * @brief Iterates over token.text, rebuilds the string while
 * searching for a "$?" segment to expand.
 * DOES NOT CARE ABOUT QUOTES.
 * @return 0 on success, -1 on error.
 */
int	hd_create_exp_status_text(char *text, char **exp_text, char *status_str)
{
	t_expand_state	st;

	init_expand_state(&st);
	while (text[st.i] != '\0')
	{
		if (text[st.i] == '$' && text[st.i + 1] == '?')
		{
			if (append_expanded_str(exp_text, status_str) == -1)
				return (-1);
			st.i += 2;
			continue ;
		}
		if (append_char(exp_text, text[st.i]) == -1)
			return (-1);
		st.i++;
	}
	return (0);
}
