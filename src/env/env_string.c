/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_string.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:35:00 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/04 15:36:17 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>

char	*create_env_string(const char *name, const char *value)
{
	char	*result;
	int		name_len;
	int		value_len;
	int		i;
	int		j;

	name_len = ft_strlen(name);
	value_len = ft_strlen(value);
	result = malloc(name_len + value_len + 2);
	if (!result)
		return (NULL);
	i = 0;
	while (i < name_len)
	{
		result[i] = name[i];
		i++;
	}
	result[i++] = '=';
	j = 0;
	while (j < value_len)
		result[i++] = value[j++];
	result[i] = '\0';
	return (result);
}
