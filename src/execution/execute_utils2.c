#include "minishell.h"

void	cleanup_resources(int **pipes, pid_t *pids, int num_cmds)
{
	int	j;

	if (num_cmds > 1)
	{
		j = 0;
		while (j < num_cmds - 1)
		{
			free(pipes[j]);
			j++;
		}
		free(pipes);
	}
	free(pids);
}
