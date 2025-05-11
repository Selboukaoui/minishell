/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_del.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 12:14:41 by asebban           #+#    #+#             */
/*   Updated: 2025/05/11 12:16:48 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' ||
			c == '\v' || c == '\f' || c == '\r');
}

static char *get_last_heredoc_delimiter(const char *cmdline)
{
	char *last = NULL;
	for (size_t i = 0; cmdline[i]; ++i) {
		if (cmdline[i] == '<' && cmdline[i + 1] == '<') {
			i += 2;
			while (cmdline[i] && ft_isspace(cmdline[i]))
				++i;
			size_t start = i;
			while (cmdline[i] && !ft_isspace(cmdline[i])
				   && cmdline[i] != '<' && cmdline[i] != '>' && cmdline[i] != '|')
				++i;
			size_t len = i - start;
			if (len > 0) {
				// free(last);
				last = ft_strndup(cmdline + start, len); // check strndup
			}
		}
	}
	return last;
}

int is_last_delim_quoted(const char *cmdline)
{
	char *raw = get_last_heredoc_delimiter(cmdline);
	if (!raw)
		return 0;
	int quoted = (strchr(raw, '\'') != NULL || strchr(raw, '"') != NULL);
	// free(raw);
	return quoted;
}