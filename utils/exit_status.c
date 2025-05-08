/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 20:00:30 by asebban           #+#    #+#             */
/*   Updated: 2025/05/08 17:34:33 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	exit_status(int action, int value)
{
	static int	status;

	status = 0;
	if (action == 0)
		return (status);
	else if (action == 1)
		status = value;
	return (status);
}
int	herdoc_status(int action, int value)
{
	static int	status;

	// status = 0;
	if (action == 0)
		return (status);
	else if (action == 1)
		status = value;
	return (status);
}

void err_for_norm(char *cmd)
{
	ft_putstr_fd("minishell: command not found: ", STDERR_FILENO);
	(ft_putstr_fd(cmd, STDERR_FILENO), ft_putstr_fd("\n", STDERR_FILENO));
	(ft_malloc(0, 0), exit(127));
}
void	get_path_error(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
}