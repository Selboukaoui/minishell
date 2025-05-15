/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_var3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 11:08:06 by asebban           #+#    #+#             */
/*   Updated: 2025/05/15 15:56:04 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static	int	calculate_total_length(const char *seg, t_shell *shell)
{
	if (!seg || !shell)
		return (0);
	return (ft_strlen(seg) + all_value((char *)seg, shell) + 1);
}

// static	void	fill_replaced_segment(const char *seg,
// 	t_shell *shell, char *result, int flag)
// {
// 	int		ptrs[3];
// 	bool	q[2];
// 	char	c;

// 	1 && (ptrs[0] = 0, ptrs[1] = 0, ptrs[2] = flag, q[0] = false, q[1] = false);
// 	while (seg[ptrs[0]])
// 	{
// 		c = seg[ptrs[0]];
// 		if (!q[0] && c == '<' && seg[ptrs[0] + 1] == '<')
// 		{
// 			1 && (ptrs[2] = 1, result[ptrs[1]++] = seg[ptrs[0]++]);
// 			result[ptrs[1]++] = seg[ptrs[0]++];
// 			continue ;
// 		}
// 		if (c == '\'' && !q[1])
// 			1 && (q[0] = !q[0], result[ptrs[1]++] = seg[ptrs[0]++]);
// 		else if (c == '"' && !q[0])
// 			1 && (q[1] = !q[1], result[ptrs[1]++] = seg[ptrs[0]++]);
// 		else if (!q[0] && c == '$'
// 			&& (ft_isalpha(seg[ptrs[0] + 1]) || seg[ptrs[0] + 1] == '_'))
// 			append_var(seg, shell, result, ptrs);
// 		else
// 			result[ptrs[1]++] = seg[ptrs[0]++];
// 	}
// 	result[ptrs[1]] = '\0';
// }

char	*ft_replace_var3(const char *seg, t_shell *shell, int flag)
{
	int		total;
	char	*result;

	if (!seg || !shell)
		return (NULL);
	total = calculate_total_length(seg, shell);
	if (total <= 0)
		return (NULL);
	result = ft_malloc(total, 1);
	if (!result)
		return (NULL);
	fill_replaced_segment(seg, shell, result, flag);
	return (result);
}
