/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 21:30:37 by asebban           #+#    #+#             */
/*   Updated: 2025/05/13 13:58:54 by asebban          ###   ########.fr       */
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

size_t	process_double_redirect(char **out, size_t *out_len, int flag)
{
	if (flag == 1)
		append_chunk(out, out_len, "'>>'", 4);
	else
		append_chunk(out, out_len, "'<<'", 4);
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

// void	app_result(char **pres)
// {
// 	const char	*in;
// 	char		*out;
// 	size_t		out_len;
// 	size_t		i;

// 	if (!(*pres))
// 		return ;
// 	in = *pres;
// 	out = ft_strdup("");
// 	out_len = 0;
// 	i = 0;
// 	out[0] = '\0';
// 	while (in[i])
// 	{
// 		if ((in[i] == '>' && in[i + 1] == '>') || (in[i] == '<' && in[i + 1] == '<'))
// 		{
// 			if (in[i] == '>')
// 				i += process_double_redirect(&out, &out_len, 1);
// 			else
// 				i += process_double_redirect(&out, &out_len, 0);
// 		}
// 		else if (in[i] == '<' || in[i] == '>' || in[i] == '|')
// 			i += process_single_operator(&out, &out_len, in[i]);
// 		else
// 			i += process_regular_char(&out, &out_len, &in[i]);
// 	}
// 	*pres = out;
// }


/*
 * Revised app_result: Split input on spaces, wrap each token in double quotes,
 * then join them back into a single string and assign to *pres.
 * Uses ft_split, ft_strdup, ft_strjoin, and proper memory management.
 */
void	app_result(char **pres)
{
    char    **tokens;
    char    *result;
    char    *tmp;
    size_t  i;

    if (!pres || !*pres)
        return;
    
    /* Split original string by spaces */
    tokens = ft_split(*pres, ' ');

    /* Initialize empty result */
    result = ft_strdup("");

    /* Iterate tokens, wrap in quotes, and join */
    i = 0;
    while (tokens[i])
    {
        /* Wrap current token in double quotes */
        tmp = ft_strjoin("\"", tokens[i]);
        tokens[i] = ft_strjoin(tmp, "\"");

        /* Append space if not the first token */
        if (i > 0)
        {
            tmp = ft_strjoin(" ", tokens[i]);
            tokens[i] = tmp;
        }

        /* Append to result */
        tmp = ft_strjoin(result, tokens[i]);
        result = tmp;
        i++;
    }
    *pres = result;
}
