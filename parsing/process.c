/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 11:58:23 by asebban           #+#    #+#             */
/*   Updated: 2025/05/13 13:51:11 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	open_outputfile(t_executor *current, t_lexer_list *lexer)
{
	if (!lexer || !lexer->str || lexer->type != 1)
	{
		ft_putstr_fd("minishell: ambiguous redirect\n", STDERR_FILENO);
		exit_status(1, 1);
		return (false);
	}
	if (current->append)
		current->fd_out = open(lexer->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		current->fd_out = open(lexer->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (current->fd_out == -1)
	{
		perror(lexer->str);
		return (false);
	}
	return (true);
}

int	process_out_append(t_executor *current, t_lexer_list *lexer)
{	
	current->append = (lexer->type == APPEND);
	current->rederect_out = (lexer->type == rederect_out);
	if (!open_outputfile(current, lexer->next))
		return (FAILED);
	return (OK);
}

int	process_in_heredoc(t_executor *cur, t_lexer_list *lex, t_shell *sh)
{
	if (!lex->next || !lex->next->str)
	{
		ft_putstr_fd("minishell: ambiguous redirect\n", STDERR_FILENO);
		exit_status(1, 1);
		return (FAILED);
	}
	if (lex->type == HEREDOC)
	{
		cur->fd_in = create_heredoc(lex->next->str, sh);
		if (cur->fd_in == -1 || cur->fd_in == -2)
			return (exit_status(1, 130), FAILED);
	}
	else
	{
		cur->fd_in = open(lex->next->str, O_RDONLY);
		if (cur->fd_in == -1)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			perror(lex->next->str);
			return (FAILED);
		}
	}
	return (OK);
}

int	process_command(t_executor *current, t_lexer_list *lexer)
{
	int				size;
	t_lexer_list	*tmp;
	int				i;

	i = 0;
	size = 0;
	tmp = lexer;
	while (tmp && tmp->type == CMD)
	{
		size++;
		tmp = tmp->next;
	}
	current->execs = (char **)ft_calloc(size + 1, sizeof(char *));
	if (!current->execs)
		return (FAILED);
	while (i < size)
	{
		current->execs[i] = ft_strdup(lexer->str);
		if (!current->execs[i])
			return (FAILED);
		lexer = lexer->next;
		i++;
	}
	return (OK);
}
