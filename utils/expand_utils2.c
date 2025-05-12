/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 21:30:37 by asebban           #+#    #+#             */
/*   Updated: 2025/05/12 21:33:48 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	append_chunk(char **dst, size_t *dst_len, const char *src, size_t len)
{
	char	*new_dst;

	new_dst = ft_realloc(*dst, *dst_len, *dst_len + len + 1);
	if (!new_dst)
		return ;
	*dst = new_dst;
	ft_memcpy(*dst + *dst_len, src, len);
	*dst_len += len;
	(*dst)[*dst_len] = '\0';
}

size_t	process_double_redirect(char **out, size_t *out_len)
{
	append_chunk(out, out_len, "'>>'", 4);
	return (2);
}

size_t	process_single_operator(char **out,
		size_t *out_len, char sym)
{
	char	quoted[4];

	quoted[0] = '\'';
	quoted[1] = sym;
	quoted[2] = '\'';
	quoted[3] = '\0';
	append_chunk(out, out_len, quoted, 3);
	return (1);
}

size_t	process_regular_char(char **out,
		size_t *out_len, const char *in)
{
	append_chunk(out, out_len, in, 1);
	return (1);
}

void	app_result(char **pres)
{
	const char	*in;
	char		*out;
	size_t		out_len;
	size_t		i;

	in = *pres;
	out = malloc(1);
	out_len = 0;
	i = 0;
	out[0] = '\0';
	while (in[i])
	{
		if (in[i] == '>' && in[i + 1] == '>')
			i += process_double_redirect(&out, &out_len);
		else if (in[i] == '<' || in[i] == '>' || in[i] == '|')
			i += process_single_operator(&out, &out_len, in[i]);
		else
			i += process_regular_char(&out, &out_len, &in[i]);
	}
	*pres = out;
}
