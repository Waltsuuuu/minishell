#include "minishell.h"

int	count_heredocs(t_input *input)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < input->n_tokens)
	{
		if (input->tokens[i].type == TOK_HEREDOC)
			count++;
		i++;
	}
	return (count);
}
