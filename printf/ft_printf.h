/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 15:45:27 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/04 14:01:33 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>

int		ft_printf(const char *format, ...);
int		ft_fprintf(int fd, const char *format, ...);
int		ft_putchar(char c);
int		ft_putstr(char *str);
int		ft_putnumb(int n);
int		ft_putuns(unsigned int n);
int		ft_putpointer(unsigned long long ptr);
int		ft_putesa(unsigned long long n, const char type);
int		ft_putchar_printf_fd(char c, int fd);
int		ft_putstr_printf_fd(char *str, int fd);
int		ft_putnumb_printf_fd(int n, int fd);
int		ft_putuns_printf_fd(unsigned int n, int fd);
int		ft_putpointer_printf_fd(unsigned long long ptr, int fd);
int		ft_putesa_printf_fd(unsigned long long n, const char type, int fd);
size_t	ft_strlen(const char *s);
char	*ft_utoa(unsigned int n);

#endif