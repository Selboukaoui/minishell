/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 20:07:22 by selbouka          #+#    #+#             */
/*   Updated: 2025/04/17 20:07:22 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <linux/limits.h>
# include <stdbool.h>
# include <limits.h>
# include <sys/stat.h>
# include <stdio.h>
# include <fcntl.h>
# include <signal.h>
# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include "struct.h"
# include "execution.h"
# include "parsing.h"
# include "builtin.h"

# define FAILED 102
# define OK 100
# define EXIT_GET 0
# define EXIT_SET 1

# define FAIL_SYSCALL       -1
# define OKAY               0
# define FAIL_SYSCALL_PARENT    1
# define FAIL_SYSCALL_CHILD  2

# define FAIL -1
# define PARENT 1
# define CHILD  3


char    *get_next_token(const char *s, int *index);
char    *parse_token(const char *s, int *i);
int     parse_literal(const char *s, int *i, char **res);
int     parse_quoted(const char *s, int *i, char **res);
char    *append_segment(char *res, const char *seg, int len);
int	count_tokens(const char *s);
int	advance_token(const char *s, int idx);
int	ft_operator_len(const char *s);
char	*ft_dup_token(const char *str, int len);
int	skip_spaces(const char *s, int i);

#endif