/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:52:41 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/05/22 14:45:26 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE
# endif
// Main function 
char			*get_next_line(int fd);

// Utility functions
char			*strjoin_gnl(char *s1, char *s2);
char			*strchr_gnl(const char *s, int c);
size_t			strlen_gnl(const char *s);
char			*extract_line(char *storage);
char			*save_storage(char *storage);
size_t			strlcpy_gnl(char *dst, const char *src, size_t size);
char			*strdup_gnl(const char *s);
int				is_storage_empty(char *storage);

#endif