/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:15:39 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/05/27 16:48:28 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdarg.h>

int		ft_printf(const char *format, ...);
int		print_format(char fspecifier, va_list ap);
int		print_char(int c);
int		print_str(char *s);
int		print_pointer(void *ptr);
int		print_number_base(unsigned long nb, int base, int is_upper);
int		print_signed_number(int nb);
int		print_invalid_format(char fspecifier);
int		parse_format(const char *format, va_list ap);
#endif
