/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 00:00:00 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/02 21:08:39 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <ctype.h>

int	handle_exit_status(char *dst, int *di)
{
	char	status[12];
	int		k;

	snprintf(status, sizeof(status), "%d", g_state.last_status);
	k = 0;
	while (status[k])
		dst[(*di)++] = status[k++];
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
	while (src[start + len] && (isalnum(src[start + len])
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

	strncpy(var, src + var_start, var_len);
	var[var_len] = 0;
	val = getenv(var);
	di = 0;
	if (val)
	{
		k = 0;
		while (val[k])
			dst[di++] = val[k++];
	}
	return (di);
}
