/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:53:00 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/04 14:02:44 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putpointer_printf_fd(unsigned long long ptr, int fd)
{
	int	count;

	count = 0;
	if (ptr == 0)
	{
		count += ft_putstr_printf_fd("(nil)", fd);
		return (count);
	}
	count += ft_putstr_printf_fd("0x", fd);
	count += ft_putesa_printf_fd(ptr, 'x', fd);
	return (count);
}

int	ft_putesa_printf_fd(unsigned long long n, const char type, int fd)
{
	int		count;
	char	*base;

	count = 0;
	if (type == 'x')
		base = "0123456789abcdef";
	else
		base = "0123456789ABCDEF";
	if (n >= 16)
		count += ft_putesa_printf_fd(n / 16, type, fd);
	count += ft_putchar_printf_fd(base[n % 16], fd);
	return (count);
}

static int	ft_handle_format_fd(va_list *args, const char format, int fd)
{
	if (format == 'c')
		return (ft_putchar_printf_fd(va_arg(*args, int), fd));
	else if (format == 's')
		return (ft_putstr_printf_fd(va_arg(*args, char *), fd));
	else if (format == 'p')
		return (ft_putpointer_printf_fd(va_arg(*args, unsigned long long), fd));
	else if (format == 'd' || format == 'i')
		return (ft_putnumb_printf_fd(va_arg(*args, int), fd));
	else if (format == 'u')
		return (ft_putuns_printf_fd(va_arg(*args, unsigned int), fd));
	else if (format == 'x' || format == 'X')
		return (ft_putesa_printf_fd(va_arg(*args, unsigned int), format, fd));
	else if (format == '%')
		return (ft_putchar_printf_fd('%', fd));
	return (0);
}

int	ft_fprintf(int fd, const char *format, ...)
{
	va_list		args;
	size_t		i;
	int			len;

	if (!format)
		return (-1);
	i = 0;
	len = 0;
	va_start(args, format);
	while (format[i])
	{
		if (format[i] == '%')
		{
			len += ft_handle_format_fd(&args, format[i + 1], fd);
			i++;
		}
		else
			len += ft_putchar_printf_fd(format[i], fd);
		i++;
	}
	va_end(args);
	return (len);
}
