#include "minishell.h"


int	exec_pipeline(t_pipeline *pipeline, t_shell *shell)
{
	int		cmd_index;
	int		previous_read;
	int		next_read;
	int		next_write;
	struct sigaction ign;
	struct sigaction old_quit;
	struct sigaction old_int;
	struct termios tty;

	pipeline->child_pids = (pid_t *)malloc(sizeof(pid_t) * pipeline->n_cmds);
	if (!pipeline->child_pids)
	{
		return (1);
	}
	ft_bzero(pipeline->child_pids, sizeof(pid_t) * pipeline->n_cmds);
	previous_read = -1;
	cmd_index = 0;
	shell->pipeline.pipe_pair[0] = -1; //initialize the pipes
	shell->pipeline.pipe_pair[1] = -1;
	save_terminal_state(&tty);
	ignore_parent_sig_handlers(&ign, &old_quit, &old_int);
	while (cmd_index < pipeline->n_cmds)
	{
		if (open_next_pipe_if_needed(cmd_index, shell, &next_read, &next_write) < 0)
		{
			if (previous_read >= 0)
				close(previous_read);
			kill_and_reap_children(pipeline->child_pids, cmd_index);
			free (pipeline->child_pids);
			restore_parent_sig_handlers(&old_quit, &old_int);
			return (1); //TODO ERROR MSG
		}
		pipeline->child_pids[cmd_index] = spawn_cmd(&pipeline->cmds[cmd_index],					
				previous_read, next_write, shell);
		//close parent pipes
		if (pipeline->child_pids[cmd_index] < 0)
		{
			if (next_write >= 0)
				close(next_write);
			if (next_read  >= 0)
				close(next_read);
			if (previous_read  >= 0)
				close(previous_read);
			kill_and_reap_children(pipeline->child_pids, cmd_index);
			free (pipeline->child_pids);
			restore_parent_sig_handlers(&old_quit, &old_int);
			return (1); //TODO ERROR MSG
		}
		if (previous_read >= 0)
			close(previous_read);
		if (next_write >= 0)
			close (next_write);
		previous_read = next_read;
		cmd_index++;
	}
	if (previous_read >= 0)
		close(previous_read);
	shell->last_status = wait_all_and_last_status(pipeline->n_cmds,
			pipeline->child_pids[pipeline->n_cmds - 1]);
	restore_parent_sig_handlers(&old_quit, &old_int);
	restore_terminal_state(&tty);
	if (shell->last_status == 130)
		write(1, "\n", 1);
	free(pipeline->child_pids);
	free_allocs(shell);
	return (shell->last_status);
}


