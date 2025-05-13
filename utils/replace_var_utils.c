/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_var_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 21:47:45 by asebban           #+#    #+#             */
/*   Updated: 2025/05/13 15:18:51 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static	void	append_var(const char *seg, t_shell *shell,
		char *result, int ptrs[3])
{
	int		start;
	int		len;
	char	*name;
	char	*val;

	start = ptrs[0] + 1;
	len = var_name_len1(seg, start);
	name = ft_malloc(len + 1, 1);
	if (!name)
		return ;
	ft_strncpy(name, &seg[start], len);
	name[len] = '\0';
	val = get_env_value(shell->env, name);
	if (ptrs[2] == 1)
		app_result(&val);
	if (val)
	{
		ft_strcpy(&result[ptrs[1]], val);
		ptrs[1] += ft_strlen(val);
	}
	else
	{
		result[ptrs[1]++] = '$';
		ft_memcpy(&result[ptrs[1]], name, len);
		ptrs[1] += len;
	}
	ptrs[0] = start + len;
}

char	*ft_replace_var3(const char *seg, t_shell *shell, int flag)
{
	int		ptrs[3];
	int		total;
	char	*result;
	bool	in_sq;
	bool	in_dq;
	char	c;

	in_dq = false;
	1 && (ptrs[0] = 0, ptrs[1] = 0, in_sq = false, ptrs[2] = flag);
	if (!seg || !shell)
		return (NULL);
	total = ft_strlen(seg) + all_value((char *)seg, shell) + 1;
	result = ft_malloc(total, 1);
	if (!result)
		return (NULL);
	while (seg[ptrs[0]])
	{
		c = seg[ptrs[0]];

		// Detect start of heredoc: "<<"
		if (!in_sq && c == '<' && seg[ptrs[0] + 1] == '<')
		{
			// printf ("hey\n");
			// Enter skip mode: next variable is not expanded
			ptrs[2] = 1;
			// copy the two '<' chars
			result[ptrs[1]++] = seg[ptrs[0]++];
			result[ptrs[1]++] = seg[ptrs[0]++];
			continue;
		}

		if (c == '\'' && !in_dq)
		{
			// toggle single-quote state
			in_sq = !in_sq;
			result[ptrs[1]++] = seg[ptrs[0]++];
		}
		else if (c == '\"' && !in_sq)
		{
			in_dq = !in_dq;
			result[ptrs[1]++] = seg[ptrs[0]++];
		}
		else if (!in_sq && c == '$' && (ft_isalpha(seg[ptrs[0] + 1]) || seg[ptrs[0] + 1] == '_'))
		{
				append_var(seg, shell, result, ptrs);
		}
		else
		{
			result[ptrs[1]++] = seg[ptrs[0]++];
		}
		// printf ("%s",result);
	}
	return (result[ptrs[1]] = '\0', result);
}

static	char	*ft_check_segments(char *seg, t_shell *shell)
{
	int		i;
	bool	has_equal;

	i = 0;
	has_equal = false;
	if (!seg || !shell)
		return (seg);
	while (seg[i])
	{
		if (seg[i] == '=')
		{
			has_equal = true;
			break ;
		}
		if (seg[i] == '$')
			break ;
		i++;
	}
	if (has_equal && seg[i] != '$')
		return (seg);
	return (ft_replace_var3(seg, shell, 1));
}

static	char	*process_token(char *output,
				char *token, t_shell *shell, bool *skip_heredoc)
{
	char	*new_output;
	char	*tmp;

	if (*skip_heredoc)
	{
		new_output = ft_strjoin(output, token);
		*skip_heredoc = false;
	}
	else if (ft_strcmp(token, "<<") == 0)
	{
		new_output = ft_strjoin(output, "<<");
		*skip_heredoc = true;
	}
	else
	{
		tmp = ft_check_segments(token, shell);
		new_output = ft_strjoin(output, tmp);
	}
	return (new_output);
}

char	*process_tokens(char **tokens, t_shell *shell)
{
	size_t	i;
	char	*output;
	bool	skip_heredoc;
	char	*new_output;

	output = ft_strdup("");
	skip_heredoc = false;
	i = 0;
	while (tokens[i])
	{
		output = process_token(output, tokens[i], shell, &skip_heredoc);
		if (tokens[i + 1])
		{
			new_output = ft_strjoin(output, " ");
			output = new_output;
		}
		i++;
	}
	return (output);
}
