#include "minishell.h"

int	init_tokenizer_state(t_tokenizer_state *state, const char *line)
{
	if (!state)
		return (-1);
	state->line = line;
	state->index = 0;
	state->in_double = 0;
	state->in_single = 0;
	state->word_start = -1;	// -1 means no active word yet.
	state->tokens = NULL;
	state->n_tokens = 0;
	state->head = NULL;
	state->tail = NULL;
	state->list_count = 0;
	return (0);
}

int	clear_tokenizer_state(t_tokenizer_state *state)
{
	if (!state)
		return (-1);
	if (state->tokens)
		free_token_array(state);
	if (state->head)
		free_token_list(state);
	state->tokens = NULL;
	state->n_tokens = 0;
	state->index = 0;
	state->in_double = 0;
	state->in_single = 0;
	state->word_start = -1;
	state->head = NULL;
	state->tail = NULL;
	state->list_count = 0;
	return (0);
}

int	free_token_array(t_tokenizer_state *state)
{
	int	i;

	if (!state)
		return (-1);
	i = 0;
	while (i < state->n_tokens)
	{
		if (state->tokens[i].text)
			free(state->tokens[i].text);
		i++;
	}
	free(state->tokens);
	return (0);
}

int	free_token_list(t_tokenizer_state *state)
{
	t_toknode	*current;
	t_toknode	*next;

	if (!state)
		return (-1);
	current = state->head;
	while (current)
	{
		next = current->next;
		if (current->tok.text)
			free(current->tok.text);
		free(current);
		current = next;
	}
	return (0);
}