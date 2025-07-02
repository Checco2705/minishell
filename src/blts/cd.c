/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:32:10 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/01 23:02:59 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>   // Per chdir, getcwd
#include <stdio.h>    // Per perror, fprintf
#include <string.h>   // Per strcmp
#include <stdlib.h>   // Per getenv

// Cambia directory, gestisce cd, cd -, errori e stampa su stderr
int	ft_cd(char **args)
{
	char	*path;

	path = NULL;
	if (args[1] && args[2])
	{
		fprintf(stderr, "cd: troppi argomenti\n");
		return (1);
	}
	if (!args[1] || strcmp(args[1], "~") == 0)
		path = getenv("HOME");
	else if (strcmp(args[1], "-") == 0)
		path = getenv("OLDPWD");
	else
		path = args[1];
	if (!path)
	{
		fprintf(stderr, "cd: path not set\n");
		return (1);
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}
