#include "minishell.h"

/**
 * @brief Rebuilds token.text, expanding $VAR (KEY) to its value.
 * If KEY is invalid, it is appended as it is.
 * If KEY is valid but is not declared, KEY is expanded to
 * an empty string.
 * While not inside quotes, a '$' preceeding a quoted segment is skipped.
 * @return Newly allocated expanded string, or NULL on error.
 */
char	*hd_expand_variable(char *text, char **envp)
{
	char	*exp_text;

	if (!text)
		return (ft_strdup(""));
	exp_text = ft_strdup("");
	if (!exp_text)
		return (NULL);
	if (hd_create_exp_var_text(text, &exp_text, envp) == -1)
	{
		free(exp_text);
		return (NULL);
	}
	return (exp_text);
}

/**
 * @brief Iterates over token.text, rebuilds the string while searching for
 * a valid "$VAR" segment to expand.
 * Keeps track of quote state. Does not expand inside single quotes.
 * @return 0 on success, -1 on error.
 */
int	hd_create_exp_var_text(char *text, char **exp_text, char **envp)
{
	t_expand_state	st;

	init_expand_state(&st);
	while (text[st.i] != '\0')
	{
		st.expanded = handle_var_expansion(&st, text, exp_text, envp);
		if (st.expanded == -1)
			return (-1);
		if (st.expanded == 1)
			continue ;
		if (append_char(exp_text, text[st.i]) == -1)
			return (-1);
		st.i++;
	}
	return (0);
}
