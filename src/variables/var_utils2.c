#include "minishell.h"

int	copy_env_value(const char *src, int *si, char *dst, int *di)
{
	int	var_start;
	int	var_len;
	int	chars_written;

	if (src[*si] == '$' && src[*si + 1] == '?')
	{
		*si += 2;
		return (handle_exit_status(dst, di));
	}
	if (src[*si] == '$' && (ft_isalpha(src[*si + 1]) || src[*si + 1] == '_'))
	{
		(*si)++;
		var_start = *si;
		var_len = get_var_length(src, var_start);
		*si += var_len;
		chars_written = handle_env_var(src, var_start, var_len, dst + *di);
		*di += chars_written;
		return (1);
	}
	dst[(*di)++] = src[(*si)++];
	return (0);
}

int	process_char_in_expansion(const char *str, int *si, char *result,
	int *di)
{
	char	in_quotes;

	in_quotes = 0;
	if ((str[*si] == '\'' || str[*si] == '"') && !in_quotes)
		in_quotes = str[*si];
	else if (str[*si] == in_quotes)
		in_quotes = 0;
	if (in_quotes == '\'')
	{
		result[(*di)++] = str[(*si)++];
		return (1);
	}
	return (copy_env_value(str, si, result, di));
}

char	*expand_string(const char *str)
{
	char	*result;
	int		si;
	int		di;

	if (!str)
		return (NULL);
	result = malloc(4096);
	if (!result)
		return (NULL);
	si = 0;
	di = 0;
	while (str[si])
	{
		process_char_in_expansion(str, &si, result, &di);
	}
	result[di] = '\0';
	return (result);
}

int	should_expand(const char *str, int si, int single_q, int double_q)
{
	if (single_q)
		return (0);
	if (str[si] != '$')
		return (0);
	if (str[si + 1] == '\0')
		return (0);
	if (double_q || (!single_q && !double_q))
		return (1);
	return (0);
}

