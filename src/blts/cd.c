/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:32:10 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/04 14:49:38 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>   // Per chdir, getcwd
#include <stdio.h>    // Per perror
#include <stdlib.h>   // Per getenv

// Cambia directory, gestisce cd, cd -, errori e stampa su stderr
int	ft_cd(char **args)
{
	char	*path;

	path = NULL;
	if (args[1] && args[2])
	{
		ft_fprintf(2, "cd: troppi argomenti\n");
		return (1);
	}
	if (!args[1] || ft_strncmp(args[1], "~", 2) == 0)
		path = ft_getenv("HOME");
	else if (ft_strncmp(args[1], "-", 2) == 0)
		path = ft_getenv("OLDPWD");
	else
		path = args[1];
	if (!path)
	{
		ft_fprintf(2, "cd: path not set\n");
		return (1);
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}
