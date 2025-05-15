/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_executor_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:18:37 by asebban           #+#    #+#             */
/*   Updated: 2025/05/15 15:24:16 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern int	g_signals;

t_executor	*fill_executor_list(t_shell *shell, t_executor *list)
{
	t_executor		*current;
	t_lexer_list	*lexer;

	current = list;
	lexer = shell->lex_head;
	while (current)
	{
		list = process_lexemes(list, current, &lexer, shell);
		if (!list)
		{
			// if (exit_status(0, 0) == 0)
			// 	exit_status(1, 1);
			return (NULL);
		}
		// exit_status(1, 0);
		current = current->next;
	}
	return (list);
}
