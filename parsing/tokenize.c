/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:35:04 by asebban           #+#    #+#             */
/*   Updated: 2025/05/10 20:51:28 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static	int	count_input_array(char **input_array)
{
	int	size;

	size = 0;
	if (!input_array)
		return (0);
	while (input_array[size])
		size++;
	return (size);
}

static t_lexer_list	*allocate_lexer_nodes(int size)
{
	t_lexer_list	*head;
	t_lexer_list	*current;
	t_lexer_list	*prev;
	int				i;

	if (size == 0)
		return (NULL);
	head = (t_lexer_list *)ft_malloc(sizeof(t_lexer_list), 1);
	if (!head)
		return (NULL);
	1 && (head->prev = NULL, prev = head, i = 1);
	while (i < size)
	{
		current = (t_lexer_list *)ft_malloc(sizeof(t_lexer_list), 1);
		if (!current)
		{
			while (head)
				1 && (current = head->next, head = current);
			return (NULL);
		}
		1 && (current->prev = prev, prev->next = current, prev = current, i++);
	}
	if (size > 0)
		prev->next = NULL;
	return (head);
}

static	t_lexer_list	*create_lexer_list(char **input_array)
{
	int	size;

	size = count_input_array(input_array);
	if (size == 0)
		return (NULL);
	return (allocate_lexer_nodes(size));
}

static	int	find_token_pos(const char *rl, const char *s, int last_pos)
{
	size_t	rl_len;
	size_t	rem_len;
	char	*p;

	if (!rl || !s)
		return (-1);
	rl_len = ft_strlen(rl);
	if ((size_t)last_pos >= rl_len)
		return (-1);
	rem_len = rl_len - last_pos;
	p = ft_strnstr(rl + last_pos, s, rem_len);
	if (!p)
		return (-1);
	return ((int)(p - rl));
}

t_token	which_type(char *s, int inside)
{
	if (!s)
		return (EMPTY);
	if (inside)
		return (CMD);
	if (ft_strcmp(s, "|") == 0)
		return (PIPE);
	if (ft_strcmp(s, "<") == 0)
		return (REDERECT_IN);
	if (ft_strcmp(s, ">") == 0)
		return (rederect_out);
	if (ft_strcmp(s, "<<") == 0)
		return (HEREDOC);
	if (ft_strcmp(s, ">>") == 0)
		return (APPEND);
	return (CMD);
}

t_lexer_list	*init_lexer_and_flags(char **input_array)
{
	t_lexer_list	*head;

	head = create_lexer_list(input_array);
	return (head);
}


int	assign_lexer_data(t_lexer_list *cur, char **input_array, const char *rl_copy)
{
	int		last_pos;
	int 	idx;
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
	
	if (!shell || !shell->rl_copy )
		return (NULL);
	tokens = ft_newsplit(shell->rl_copy);
	if (!tokens)
		return (NULL);
	lex = fill_lexer_list(tokens, shell->rl_copy);
	return (lex);
}
