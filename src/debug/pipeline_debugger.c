#include "minishell.h"

/* ---------- debug printers for pipeline ---------- */

const char	*redir_type_name(t_redir_type t)
{
	if (t == REDIR_IN)
		return ("REDIR_IN <");
	if (t == REDIR_OUT)
		return ("REDIR_OUT >");
	if (t == REDIR_APPEND)
		return ("REDIR_APPEND >>");
	if (t == REDIR_HEREDOC)
		return ("REDIR_HEREDOC <<");
	return ("UNKNOWN");
}

void	print_redirs(const t_list *lst)
{
	const t_list	*cur;
	t_redir			*r;

	cur = lst;
	if (!cur)
	{
		printf("    redirs: (none)\n");
		return ;
	}
	printf("    redirs:\n");
	while (cur)
	{
		r = (t_redir *)cur->content;
		if (r)
			printf("      - %s  target=\"%s\"  no_expand=%d  hd_fd=%d\n",
				redir_type_name(r->type),
				r->target ? r->target : "(null)",
				r->no_expand,
				r->hd_fd);
		cur = cur->next;
	}
}

void	print_one_cmd(const t_command *cmd, int idx)
{
	int	i;

	printf("[cmd %d]\n", idx);
	printf("    argc: %d\n", cmd->argc);
	printf("    argv:");
	if (!cmd->argv || cmd->argc == 0)
		printf(" (none)\n");
	else
	{
		printf("\n");
		i = 0;
		while (cmd->argv[i])
		{
			printf("      argv[%d] = \"%s\"\n", i,
				cmd->argv[i] ? cmd->argv[i] : "(null)");
			i++;
		}
	}
	print_redirs(cmd->redirs);
}

void	print_cmds(const t_pipeline *pipeline)
{
	int	i;

	if (!pipeline || !pipeline->cmds || pipeline->n_cmds <= 0)
	{
		printf("(pipeline empty)\n");
		return ;
	}
	printf("\n\n=== pipeline: %d command(s) ===\n\n", pipeline->n_cmds);
	i = 0;
	while (i < pipeline->n_cmds)
	{
		print_one_cmd(&pipeline->cmds[i], i);
		i++;
	}
	printf("\n\n");
}