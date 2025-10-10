/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 12:18:43 by wheino            #+#    #+#             */
/*   Updated: 2025/10/10 12:36:01 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# define PATH_MAX 4096

/*					get_working_dir.c							*/
void	getworkindir(char *buf, size_t size);

/*					free_allocs.c								*/
void	free_allocs(t_shell *shell);
void	cleanup(t_shell *shell);
void	free_str_ptr(char **str_ptr);
void	free_on_failure(t_shell *shell);
void	free_shell(t_shell *shell);

/*					print_msh_banner.c							*/
void	print_msh_banner(void);

/*					free_split.c								*/
void	free_split(char ***arr_ptr);
void	free_partial(char **arr, size_t n);

// UTILS - signal_handling
/*					signal.c									*/
void	setup_signal_handlers_for_prompt(void);
void	setup_signal_handlers_for_child(void);
void	handle_sig(int signum);
void	ignore_parent_sig_handlers(struct sigaction *ign,
			struct sigaction *old_quit,
			struct sigaction *old_int);
void	restore_parent_sig_handlers(struct sigaction *old_quit,
			struct sigaction *old_int);

// Parsing for exit
int		ft_strtoll(const char *str, long long *out_value);

// Count heredocs
int		count_heredocs(t_input *input);

// Mode check
char	*read_with_mode_check(const char *buf);
void	remove_nl(char *str);

// Tty
void	save_terminal_state(struct termios *tty);
void	restore_terminal_state(struct termios *tty);

#endif