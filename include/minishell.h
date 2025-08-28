/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 10:08:48 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/08/28 15:07:17 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
//#include <editline/readline.h>
# include <readline/history.h>
# include "colors.h"
# include <sys/wait.h>
# include <sys/stat.h> // access
# include "tokenizer.h"

//TODO move to executor.h
char	*join_cmd_to_path(const char *path, const char *cmd);
char	**find_from_path(char *envp[]);
char	**build_absolute_paths(char **paths, const char *cmd);
void	exec_ext_func(char **absolute_paths, char **words, char *envp[]);
//TODO move to utils.h
void	free_split(char **arr);
void	free_partial(char **arr, size_t n);

#endif