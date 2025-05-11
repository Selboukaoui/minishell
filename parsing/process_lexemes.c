/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_lexemes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 11:55:21 by asebban           #+#    #+#             */
/*   Updated: 2025/05/11 12:09:08 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int handle_out_append_node(t_executor *current, t_lexer_list **lexer)
{
    int ret;

    ret = process_out_append(current, *lexer);
    if (ret == FAILED)
        return (FAILED);
    /* skip operator and filename */
    *lexer = (*lexer)->next->next;
    return (0);
}

/* Handle '<' and '<<' (heredoc) tokens */
static int handle_in_heredoc_node(t_executor *current, t_lexer_list **lexer, t_shell *shell)
{
    int ret;

    ret = process_in_heredoc(current, *lexer, shell);
    if (ret == FAILED)
        return (FAILED);
    /* skip operator and delimiter */
    *lexer = (*lexer)->next->next;
    return (0);
}

/* Handle command tokens */
static int handle_cmd_node(t_executor *current, t_lexer_list **lexer)
{
    int ret;

    ret = process_command(current, *lexer);
    if (ret == FAILED)
        return (FAILED);
    /* skip all consecutive CMD tokens */
    while (*lexer && (*lexer)->type == CMD)
        *lexer = (*lexer)->next;
    return (0);
}

/* Main segment processor: loops until PIPE or end */
static int process_lexeme_segment(t_executor *current,
                                  t_lexer_list **lexer,
                                  t_shell *shell)
{
    while (*lexer && (*lexer)->type != PIPE)
    {
        if ((*lexer)->type == rederect_out || (*lexer)->type == APPEND)
        {
            if (handle_out_append_node(current, lexer) == FAILED)
                return (FAILED);
        }
        else if ((*lexer)->type == REDERECT_IN || (*lexer)->type == HEREDOC)
        {
            if (handle_in_heredoc_node(current, lexer, shell) == FAILED)
                return (FAILED);
        }
        else if ((*lexer)->type == CMD)
        {
            if (handle_cmd_node(current, lexer) == FAILED)
                return (FAILED);
        }
        else
        {
            /* skip any other token */
            *lexer = (*lexer)->next;
        }
    }
    return (0);
}

/* Original API: process lexemes and skip PIPE */
t_executor *process_lexemes(t_executor *list,t_executor *current, t_lexer_list **lexer, t_shell *shell)
{
    if (process_lexeme_segment(current, lexer, shell) == FAILED)
        return (NULL);

    /* skip over PIPE token if present */
    if (*lexer && (*lexer)->type == PIPE)
        *lexer = (*lexer)->next;

    return (list);
}
