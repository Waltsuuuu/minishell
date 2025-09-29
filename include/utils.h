#ifndef UTILS_H
# define UTILS_H

#ifndef PATH_MAX
# define PATH_MAX 4096
#endif

/*					get_working_dir.c							*/
void	getworkindir(char *buf, size_t size);

/*					free_allocs.c								*/
void	free_allocs(t_shell *shell);
void	free_str_ptr(char **str_ptr);

/*					print_msh_banner.c							*/
void	print_msh_banner(void);

/*					free_split.c								*/
void	free_split(char ***arr_ptr);
void	free_partial(char **arr, size_t n);

// UTILS - signal_handling
/*					signal.c									*/
void	setup_signal_handlers_for_prompt();
void	setup_signal_handlers_for_child();
void	handle_sig(int signum);

#endif