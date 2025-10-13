/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 12:51:02 by wheino            #+#    #+#             */
/*   Updated: 2025/10/13 11:30:00 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

typedef struct s_shell		t_shell;
typedef struct s_command	t_command;
typedef struct s_env		t_env;

int		is_builtin_name(const char *name);
int		builtin_export(char **argv, t_shell *shell);
int		split_key_and_value(char *line, char **key_out, char **value_out);
int		exec_export_in_parent(t_command *cmd, t_shell *shell);
int		process_export_arg(char *arg, t_shell *shell);
int		env_list_to_export_display_array(t_env *head, t_shell *shell);
int		is_builtin_valid(const char *key);
void	print_invalid_identifier(char *builtin, char *key);
int		env_set(t_env **head, const char *key, const char *value);
t_env	*env_find(t_env *head, const char *key);
int		find_equal_sign(char *str);
int		save_stdio(int saved[2]);
int		apply_redirs_in_parent(t_command *cmd, int saved[2]);
int		is_parent_builtin(const char *name);
int		exec_unset_in_parent(t_command *cmd, t_shell *shell);
int		builtin_unset(char **argv, t_shell *shell);
int		env_unset(t_shell *shell, const char *key);
int		builtin_cd(char **argv, t_shell *shell);
int		exec_cd_in_parent(t_command *cmd, t_shell *shell);
int		builtin_pwd(void);
int		builtin_echo(t_command *cmd);
int		exec_exit_in_parent(t_command *cmd, t_shell *shell);
int		builtin_exit(t_command *cmd, t_shell *shell);
int		exec_echo_in_parent(t_command *cmd, t_shell *shell);
int		restore_stdio(int saved[2]);
void	init_parent_fds(int *in_fd, int *out_fd);
int		replug_stdio_pair(int in_fd, int out_fd, int saved[2]);
int		is_interactive(t_shell *shell);

/*					ENV											*/
t_env	*env_init_from_envp(char **envp);
t_env	*create_new_env_node(const char *key, const char *value);
int		append_env_node(t_env **head, t_env *new_env_node);
void	clean_env(t_env **head);
int		env_list_to_array(t_env *head, t_shell *shell);
char	*ft_strjoin_with_equal_sign(char const *s1, char const *s2);
int		print_env(t_command *cmd, t_shell *shell);
int		env_sort_and_print(t_shell *shell);
int		find_equal_sign(char *str);
char	*env_get(t_shell *shell, const char *key);

#endif