/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:14:24 by asebban           #+#    #+#             */
/*   Updated: 2025/05/11 14:56:53 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	env(t_shell *shell)
{
	t_environ_node	*current;

	if (shell->executor->execs[1])
		return (write (2, "To many arguments\n",19), 0);
	current = shell->env->head;
	while (current)
	{
		if (current->value)
		{
			ft_putstr_fd(current->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putstr_fd(current->value, STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		current = current->next;
	}
	return (EXIT_SUCCESS);
}
