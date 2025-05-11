/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 12:35:55 by asebban           #+#    #+#             */
/*   Updated: 2025/05/11 14:59:35 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static	size_t	buf_append(char *out, size_t o, const char *str)
{
	size_t	k;

	k = 0;
	while (str && str[k])
		out[o++] = str[k++];
	return (o);
}
static	size_t	append_exit_status(char *out, size_t o)
{
	char	buf[12];
	int		status;

	status = exit_status(EXIT_GET, 0);
	int_to_str(status, buf);
	return (buf_append(out, o, buf));
}

static	size_t	append_env_var(char *out, size_t o,
		const char *start, size_t vlen, t_shell *shell)
{
	char	name[256];
	char	*val;

	ft_strncpy(name, start, vlen);
	name[vlen] = '\0';

	val = get_env_value(shell->env, name);
	if (val)
		return (buf_append(out, o, val));
	return (o);
}

static	void	calc_var_length(const char *input,
		size_t *idx, t_shell *shell, size_t *out_len)
{
	if (input[*idx + 1] == '?') {
		int status = exit_status(EXIT_GET, 0);
		char buf[12];
		int_to_str(status, buf);
		*out_len = ft_strlen(buf);
		*idx += 2;
	} else if (ft_isalpha(input[*idx + 1]) || input[*idx + 1] == '_') {
		size_t start = *idx + 1;
		size_t vlen = 0;
		while (ft_isalnum(input[start + vlen]) || input[start + vlen] == '_')
			vlen++;
		char name[256];
		ft_strncpy(name, input + start, vlen);
		name[vlen] = '\0';
		char *val = get_env_value(shell->env, name);
		*out_len = val ? ft_strlen(val) : 0;
		*idx = start + vlen;
	} else {
		/* literal $ followed by non-variable char */
		*out_len = 1;
		(*idx)++;
	}
}

static	size_t	calc_replace_size(const char *input, t_shell *shell)
{
	size_t	i;
	size_t	total;
	size_t	var_len;

	i = 0;
	total = 0;
	while (input[i])
	{
		if (input[i] == '$')
		{
			var_len = 0;
			calc_var_length(input, &i, shell, &var_len);
			total += var_len;
		}
		else
		{
			total++;
			i++;
		}
	}
	return (total);
}

static	size_t	fill_replaced_vars(char *out, const char *input, t_shell *shell)
{
	size_t	i;
	size_t	o;
	size_t	len;

	1 && (i = 0, o = 0,len = ft_strlen(input));
	while (i < len)
	{
		if (input[i] == '$')
		{
			if (input[i + 1] == '?')
			{
				o = append_exit_status(out, o);
				i += 2;
			}
			else if (ft_isalpha(input[i + 1]) || input[i + 1] == '_') {
				size_t start = i + 1;
				size_t vlen = 0;
				while (ft_isalnum(input[start + vlen]) || input[start + vlen] == '_')
					vlen++;
				o = append_env_var(out, o, input + start, vlen, shell);
				i = start + vlen;
			} else {
				out[o++] = '$';
				i++;
			}
		} else {
			out[o++] = input[i++];
		}
	}

	out[o] = '\0';
	return o;
}

char *replace_vars_heredoc(char *input, t_shell *shell)
{
	if (!input)
		return (NULL);

	size_t needed = calc_replace_size(input, shell);
	char *out = ft_malloc(needed + 1, 1);
	if (!out)
		return NULL;
	fill_replaced_vars(out, input, shell);
	return out;
}
