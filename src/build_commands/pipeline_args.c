#include "minishell.h"

int	arg_ll_to_arr(t_seg *seg, t_pipeline *pipeline, int cmd_i)
{
	t_list	*current;
	t_list	*next;
	int		i;

	pipeline->cmds[cmd_i].argv = malloc(sizeof(char *) * (seg->argc + 1));
	if (!pipeline->cmds[cmd_i].argv)
		return (-1);
	current = seg->args;
	i = 0;
	while (current)
	{
		pipeline->cmds[cmd_i].argv[i] = current->content;
		next = current->next;
		free(current);
		current = next;
		i++;
	}
	seg->args = NULL;
	pipeline->cmds[cmd_i].argv[i] = NULL;
	return (0);
}

int	append_arg(t_token *tokens, int i, t_seg *seg)
{
	t_list	*node;
	char 	*arg;

	arg = ft_strdup(tokens[i].text);
	if (!arg)
		return (-1);
	node = ft_lstnew(arg);
	if (!node)
	{
		free(arg);
		return (-1);
	}
	ft_lstadd_back(&seg->args, node);
	return (0);
}