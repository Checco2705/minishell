/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_esa.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 13:10:16 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/04 14:01:58 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_esalen(unsigned long long n)
{
	int	len;

	len = 0;
	while (n != 0)
	{
		n = n / 16;
		len++;
	}
	return (len);
}

static void	ft_print_hex_char(unsigned long long n, const char format)
{
	if (n <= 9)
		ft_putchar((n + '0'));
	else
	{
		if (format == 'x')
			ft_putchar((n - 10 + 'a'));
		if (format == 'X')
			ft_putchar((n - 10 + 'A'));
	}
}

void	ft_printesa(unsigned long long n, const char format)
{
	if (n >= 16)
	{
		ft_printesa(n / 16, format);
		ft_printesa(n % 16, format);
	}
	else
		ft_print_hex_char(n, format);
}

int	ft_putesa(unsigned long long n, const char type)
{
	if (n == 0)
		return (write(1, "0", 1));
	else
		ft_printesa(n, type);
	return (ft_esalen(n));
}
