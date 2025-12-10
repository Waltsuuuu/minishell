/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:14:41 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/05/28 16:17:23 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_number_base(unsigned long nb, int base, int is_upper)
{
	int		count;
	char	*digits;
	char	c;

	count = 0;
	if (is_upper)
		digits = "0123456789ABCDEF";
	else
		digits = "0123456789abcdef";
	if (nb >= (unsigned long)base)
		count += print_number_base(nb / base, base, is_upper);
	c = digits[nb % base];
	if (write(1, &c, 1) == -1)
		return (-1);
	return (count + 1);
}

int	print_char(int c)
{
	return (write(1, &c, 1));
}

int	print_str(char *s)
{
	int	count;

	if (!s)
		return (write(1, "(null)", 6));
	count = 0;
	while (*s)
	{
		if (write(1, s++, 1) == -1)
			return (-1);
		count++;
	}
	return (count);
}

int	print_pointer(void *ptr)
{
	unsigned long	address;
	int				return_value;
	int				count;

	address = (unsigned long)ptr;
	count = 0;
	if (address == 0)
		return (write(1, "(nil)", 5));
	if (write(1, "0x", 2) == -1)
		return (-1);
	count += 2;
	return_value = print_number_base(address, 16, 0);
	if (return_value == -1)
		return (-1);
	return (count + return_value);
}

int	print_format(char fspecifier, va_list ap)
{
	int	count;

	count = 0;
	if (fspecifier == '%')
		count += write(1, "%", 1);
	else if (fspecifier == 'c')
		count += print_char(va_arg(ap, int));
	else if (fspecifier == 's')
		count += print_str(va_arg(ap, char *));
	else if (fspecifier == 'p')
		count += print_pointer(va_arg(ap, void *));
	else if (fspecifier == 'd' || fspecifier == 'i')
		count += print_signed_number(va_arg(ap, int));
	else if (fspecifier == 'u')
		count += print_number_base(va_arg(ap, unsigned int), 10, 0);
	else if (fspecifier == 'x')
		count += print_number_base(va_arg(ap, unsigned int), 16, 0);
	else if (fspecifier == 'X')
		count += print_number_base(va_arg(ap, unsigned int), 16, 1);
	else
		count += print_invalid_format(fspecifier);
	return (count);
}
