/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:14:24 by asebban           #+#    #+#             */
/*   Updated: 2025/05/15 17:37:46 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	env(t_shell *shell)
{
	t_environ_node	*current;

	if (shell->executor->execs[1])
		return (write (2, "env :To many arguments\n", 24), 0);
	current = shell->env->head;
	while (current)
	{
		if (current->value)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (EXIT_SUCCESS);
}
