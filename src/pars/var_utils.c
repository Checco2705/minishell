/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 00:00:00 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/04 14:42:27 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>

int	handle_exit_status(char *dst, int *di)
{
	char	*status;
	int		k;

	status = ft_itoa(g_state.last_status);
	if (!status)
		return (0);
	k = 0;
	while (status[k])
		dst[(*di)++] = status[k++];
	free(status);
	return (1);
}

void	copy_env_value_to_dst(char *val, char *dst, int *di)
{
	int	k;

	k = 0;
	while (val[k])
	{
		dst[(*di)++] = val[k];
		k++;
	}
}

int	get_var_length(const char *src, int start)
{
	int	len;

	len = 0;
	while (src[start + len] && (ft_isalnum(src[start + len])
			|| src[start + len] == '_'))
		len++;
	return (len);
}

int	handle_env_var(const char *src, int var_start, int var_len, char *dst)
{
	char	var[256];
	char	*val;
	int		di;
	int		k;

	k = 0;
	while (k < var_len && k < 255)
	{
		var[k] = src[var_start + k];
		k++;
	}
	var[k] = '\0';
	val = ft_getenv(var);
	di = 0;
	if (val)
	{
		k = 0;
		while (val[k])
			dst[di++] = val[k++];
	}
	return (di);
}
