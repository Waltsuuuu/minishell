#include "minishell.h"

int	get_redir_type(TokType type, t_redir_type *t_type)
{
	if (type == TOK_REDIR_IN)
		*t_type = REDIR_IN;
	else if (type == TOK_REDIR_OUT)
		*t_type = REDIR_OUT;
	else if (type == TOK_REDIR_APP)
		*t_type = REDIR_APPEND;
	else if (type == TOK_HEREDOC)
		*t_type = REDIR_HEREDOC;
	else
		return (-1);
	return (0);
}

int	is_redir_tok(TokType type)
{
	if (type == TOK_HEREDOC || type == TOK_REDIR_IN
			|| type == TOK_REDIR_OUT || type == TOK_REDIR_APP)
		return (1);
	return (0);
}

int	count_pipes(t_input *input)
{
	int count;
	int	i;

	if (!input->tokens)
		return (0);
	count = 0;
	i = 0;
	while (i < input->n_tokens)
	{
		if (input->tokens[i].type == TOK_PIPE)
			count++;
		i++;
	}
	return (count);
}

void	tok_syntax_err(t_token *token, t_shell *shell)
{
	ft_putstr_fd("Syntax error near '", STDERR_FILENO);
	if (token->text)
		ft_putstr_fd(token->text, STDERR_FILENO);
	else
		ft_putstr_fd("(null)", STDERR_FILENO);
	ft_putstr_fd("' at position ", STDERR_FILENO);
	ft_putnbr_fd(token->pos, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	shell->last_status = 2;
}
