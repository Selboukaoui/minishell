/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:35:04 by asebban           #+#    #+#             */
/*   Updated: 2025/05/13 12:53:20 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_lexer_list	*init_lexer_and_flags(char **input_array)
{
	t_lexer_list	*head;

	head = create_lexer_list(input_array);
	return (head);
}

int	assign_lexer_data(t_lexer_list *cur,
		char **input_array, const char *rl_copy)
{
	int		last_pos;
	int		idx;
	int		i;
	int		inside;
	size_t	len;

	1 && (i = 0, last_pos = 0, inside = 0);
	while (input_array[i] && cur)
	{
		cur->str = ft_strdup(input_array[i]);
		if (!cur->str)
			return (0);
		idx = find_token_pos(rl_copy, cur->str, last_pos);
		if (idx < 0)
			idx = last_pos;
		last_pos = idx + ft_strlen(cur->str);
		len = ft_strlen(cur->str);
		if (idx > 0 && ((rl_copy[idx - 1] == '\'' && rl_copy[idx + len] == '\'')
				|| (rl_copy[idx - 1] == '\"' && rl_copy[idx + len] == '\"')))
			inside = 1;
		cur->type = which_type(cur->str, inside);
		inside = 0;
		1 && (i++, cur = cur->next);
	}
	return (1);
}

t_lexer_list	*fill_lexer_list(char **input_array, const char *rl_copy)
{
	t_lexer_list	*head;

	head = init_lexer_and_flags(input_array);
	if (!head)
		return (NULL);
	if (!assign_lexer_data(head, input_array, rl_copy))
		return (NULL);
	return (head);
}

t_lexer_list	*tokenize(t_shell *shell)
{
	char			**tokens;
	t_lexer_list	*lex;

	if (!shell || !shell->rl_copy)
		return (NULL);
	tokens = ft_newsplit(shell->rl_copy);
	if (!tokens)
		return (NULL);
	lex = fill_lexer_list(tokens, shell->rl_copy);
	return (lex);
}
