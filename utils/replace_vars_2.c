/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_vars_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 11:07:40 by asebban           #+#    #+#             */
/*   Updated: 2025/05/12 11:12:09 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static	int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' || \
			c == '\v' || c == '\f' || c == '\r');
}

/* Helper: expand ~ to HOME */
int	handle_expand_home(const char *in,
	char **res, int *i, t_shell *shell)
{
	char	*val;

	val = get_env_value2("HOME", shell);
	(void)in;
	*res = ft_strjoin(*res, val);
	(*i)++;
	return (1);
}

int	handle_skip_heredoc(const char *in, char **res, int *i)
{
	append_char(res, "<<", 2);
	*i += 2;
	while (in[*i] && ft_isspace(in[*i]))
		append_char(res, &in[(*i)++], 1);
	return (1);
}

int	should_toggle_squote(char c, int skip_word)
{
	return (c == '\'' && !skip_word);
}

void	process_skip_word(const char *segment,
	char **res, int *i, int *skip_word)
{
	if (ft_isspace(segment[*i]))
		*skip_word = 0;
	append_char(res, &segment[*i], 1);
	(*i)++;
}
