/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 12:15:09 by selbouka          #+#    #+#             */
/*   Updated: 2025/05/09 21:24:58 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	counter(const char *s, char sep)
{
	int	i = 0;
	int	count = 0;
	char	quote = 0;

	if (!s)
		return (0);
	while (s[i])
	{
		while (s[i] == sep && !quote)
			i++;
		if (s[i])
			count++;
		while (s[i])
		{
			if ((s[i] == '\'' || s[i] == '\"'))
			{
				if (!quote)
					quote = s[i];
				else if (quote == s[i])
					quote = 0;
			}
			if (s[i] == sep && !quote)
				break;
			i++;
		}
	}
	return (count);
}

static char	**loop(const char *s, char **array, char sep)
{
	int		i = 0;
	int		j = 0;
	int		start;
	char	quote = 0;
	char	*tmp;

	while (s[j])
	{
		while (s[j] == sep && !quote)
			j++;
		start = j;
		while (s[j])
		{
			if ((s[j] == '\'' || s[j] == '\"'))
			{
				if (!quote)
					quote = s[j];
				else if (quote == s[j])
					quote = 0;
			}
			if (s[j] == sep && !quote)
				break;
			j++;
		}
		if (j > start)
		{
			tmp = ft_strndup(s + start, s[j]);
			if (!tmp)
				return (NULL);
			array[i++] = tmp;
		}
	}
	array[i] = NULL;
	return (array);
}


char	**ft_split1(char const *s, char c)
{
	char	**array;

	if (!s)
		return (NULL);
	array = (char **)ft_malloc(sizeof(char *) * (counter(s, c) + 1), 1);
	if (!array)
		return (NULL);
	return (loop(s, array, c));
}

