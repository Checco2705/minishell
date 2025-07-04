/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:52:00 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/04 14:02:35 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar_printf_fd(char c, int fd)
{
	write(fd, &c, 1);
	return (1);
}

int	ft_putstr_printf_fd(char *str, int fd)
{
	int	len;

	if (!str)
	{
		write(fd, "(null)", 6);
		return (6);
	}
	len = ft_strlen(str);
	write(fd, str, len);
	return (len);
}

static int	ft_handle_min_int_fd(int fd)
{
	ft_putchar_printf_fd('-', fd);
	ft_putchar_printf_fd('2', fd);
	ft_putnumb_printf_fd(147483648, fd);
	return (11);
}

int	ft_putnumb_printf_fd(int n, int fd)
{
	int	i;

	i = 0;
	if (n == -2147483648)
		return (ft_handle_min_int_fd(fd));
	else if (n < 0)
	{
		i += ft_putchar_printf_fd('-', fd);
		i += ft_putnumb_printf_fd(-n, fd);
	}
	else if (n >= 10)
	{
		i += ft_putnumb_printf_fd(n / 10, fd);
		i += ft_putnumb_printf_fd(n % 10, fd);
	}
	else
		i += ft_putchar_printf_fd(n + 48, fd);
	return (i);
}

int	ft_putuns_printf_fd(unsigned int n, int fd)
{
	int	count;

	count = 0;
	if (n >= 10)
		count += ft_putuns_printf_fd(n / 10, fd);
	count += ft_putchar_printf_fd((n % 10) + '0', fd);
	return (count);
}
