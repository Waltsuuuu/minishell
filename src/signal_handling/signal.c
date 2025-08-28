
#include "minishell.h"

volatile sig_atomic_t g_signal = 0; //global varable fr signal handling

/**
 * @brief Handle incoming signals during a prompt.
 * Resets the input line, and display fresh prompt.
 * 
 * @param signum, received signal number(e.g SIGINT)
 */
void	handle_sig(int signum)
{
	g_signal = signum;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	
}
/**
 * @brief Setup signal handlers for minishell prompt
 * Configures SIGINT to call handle_sig and ignores SIGQUIT
 */
void	setup_signal_handlers_for_prompt()
{
	struct	sigaction sa_prompt;
	
	ft_bzero(&sa_prompt, sizeof(sa_prompt)); // initaialize all values to 0
	rl_catch_signals = 0; //these 2 lines overwrites the readlines own handlers
	rl_catch_sigwinch = 0;

	sigemptyset(&sa_prompt.sa_mask); //masks off
	sa_prompt.sa_flags = SA_RESTART;
	sa_prompt.sa_handler = handle_sig;
	
	sigaction(SIGINT, &sa_prompt, NULL); //Ctrl + C
	signal(SIGQUIT, SIG_IGN); //Ctrl + \'
}
/**
 * @brief Setup default signal handlers for child processes.
 *
 * Restores default handling of SIGINT and SIGQUIT for child
 * processes so they behave like in a normal shell execution.
 */
void	setup_signal_handlers_for_child()
{
	struct sigaction sa_child;
					
	ft_bzero(&sa_child, sizeof(sa_child));
	sa_child.sa_handler = SIG_DFL;
	sigemptyset(&sa_child.sa_mask);
	sigaction(SIGINT,  &sa_child, NULL);
	sigaction(SIGQUIT, &sa_child, NULL);
					
}