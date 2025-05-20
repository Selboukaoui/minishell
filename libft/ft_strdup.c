/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 20:44:41 by selbouka          #+#    #+#             */
/*   Updated: 2025/05/20 17:07:26 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	char	*p;
	int		len;
	int		i;

	len = ft_strlen(src);
	p = ft_malloc(len + 1, 1);
	if (p == NULL)
		return (NULL);
	i = 0;
	while (src[i])
	{
		p[i] = src[i];
		i++;
	}
	p[i] = '\0';
	return (p);
}

// char	*ft_strdup1(const char *src)
// {
// 	char	*p;
// 	int		i;

// 	i = ft_strlen(src);
// 	p = ft_malloc(i + 1, 1);
// 	i = 0;
// 	if (p == NULL)
// 		return (NULL);
// 	while (src[i])
// 	{
// 		p[i] = src[i];
// 		i++;
// 	}
// 	p[i] = '\0';
// 	return (p);
// }
