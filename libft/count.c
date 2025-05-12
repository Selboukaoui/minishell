/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 18:11:04 by asebban           #+#    #+#             */
/*   Updated: 2025/05/12 12:49:44 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	counter2(const char *s, char sep, int *i, char *quote)
{
	while (s[*i])
	{
		if ((s[*i] == '\'' || s[*i] == '\"'))
		{
			if (!(*quote))
				(*quote) = s[*i];
			else if ((*quote) == s[*i])
				(*quote) = 0;
		}
		if (s[*i] == sep && !(*quote))
			break ;
		(*i)++;
	}
}

int	counter1(const char *s, char sep)
{
	int		i;
	int		count;
	char	quote;

	1 && (i = -1, count = 0, quote = 0);
	while (s[++i])
	{
		while (s[i] == sep && !quote)
			i++;
		if (s[i])
			count++;
		counter2(s, sep, &i, &quote);
	}
	return (count);
}
