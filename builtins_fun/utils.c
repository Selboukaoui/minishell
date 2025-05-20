/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 12:56:06 by selbouka          #+#    #+#             */
/*   Updated: 2025/05/20 15:18:36 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


void	ft_cat(char *str)
{
    int     len;
    int     i;

	printf ("%s\n", str);
    i = 0;
    len = ft_strlen (str) - 1;
    while (str[len] && str[len] != '/')
    {
        i++;
        len--;
    }
    ft_bzero(str + len, i);
	// printf ("%s\n", str);
}

int ft_chdir(char *arg)
{
	char	c[PATH_MAX];
	int		flag;

	flag = 0;
	
	if (chdir(arg) != 0)
	{
		if ((!ft_strcmp(arg, "..") || !ft_strcmp(arg, "/..") ) && !getcwd(c, PATH_MAX))
			(ft_cat(arg), flag = 1);
		else
			return (perror("minishell"), FAIL);
	}
	printf ("%s\n", arg);
	if (flag == 1 && chdir(arg) != 0)
		return (perror("minishell"), FAIL);
    return (OK);
}