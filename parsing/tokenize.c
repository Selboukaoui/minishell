/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:35:04 by asebban           #+#    #+#             */
/*   Updated: 2025/05/20 19:42:57 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_lexer_list	*init_lexer_and_flags(char **input_array)
{
	t_lexer_list	*head;

	head = create_lexer_list(input_array);
	return (head);
}

static	int	process_lexer_node(t_lexer_list *node, const char *token,
		const char *rl_copy, int *last_pos)
{
	int		idx;
	size_t	len;
	int		inside;

	node->str = ft_strdup(token);
	if (!node->str)
		return (count_herdoc(2), 0);
	idx = find_token_pos(rl_copy, node->str, *last_pos);
	if (idx < 0)
		idx = *last_pos;
	len = ft_strlen(node->str);
	*last_pos = idx + len;
	inside = 0;
	if (idx > 0 && ((rl_copy[idx - 1] == '\'' && rl_copy[idx + len] == '\'')
			|| (rl_copy[idx - 1] == '"' && rl_copy[idx + len] == '"')))
		inside = 1;
	node->type = which_type(node->str, inside);
	if (node->type == HEREDOC)
		count_herdoc(1);
	if (count_herdoc(0) == 16)
	{
		ft_putstr_fd("maximum here-document count exceeded\n", 2);
		(ft_malloc(0, 0), exit(2));
	}
	return (1);
}

int	assign_lexer_data(t_lexer_list *cur, char **input_array,
		const char *rl_copy)
{
	int	i;
	int	last_pos;

	1 && (i = 0, last_pos = 0);
	while (input_array[i] && cur)
	{
		if (!process_lexer_node(cur, input_array[i], rl_copy, &last_pos))
			return (0);
		i++;
		cur = cur->next;
	}
	return (count_herdoc(2), 1);
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
