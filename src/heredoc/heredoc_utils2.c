#include "minishell.h"

static char	*create_base_filename(int counter)
{
	char	*num_str;
	char	*temp;

	num_str = ft_itoa(counter);
	if (!num_str)
		return (NULL);
	temp = ft_strjoin("/tmp/heredoc_", num_str);
	free(num_str);
	return (temp);
}

static char	*create_final_filename(char *base, int pid)
{
	char	*pid_str;
	char	*temp;
	char	*filename;

	pid_str = ft_itoa(pid);
	if (!pid_str)
	{
		free(base);
		return (NULL);
	}
	temp = ft_strjoin(base, "_");
	free(base);
	if (!temp)
	{
		free(pid_str);
		return (NULL);
	}
	filename = ft_strjoin(temp, pid_str);
	free(temp);
	free(pid_str);
	return (filename);
}

char	*create_temp_filename(void)
{
	static int	counter = 0;
	char		*base;

	base = create_base_filename(counter++);
	if (!base)
		return (NULL);
	return (create_final_filename(base, getpid()));
}
