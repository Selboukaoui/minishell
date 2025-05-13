/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:32:07 by asebban           #+#    #+#             */
/*   Updated: 2025/05/13 13:50:58 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include "minishell.h"

typedef struct s_shell
{
	char					*rl_input;
	char					*rl_copy;
	struct s_environ_list	*env;
	struct s_lexer_list		*lex_head;
	struct s_executor		*executor;
}	t_shell;

typedef struct s_environ_list
{
	struct s_environ_node	*head;
	int						size;
}	t_environ_list;

typedef struct s_environ_node
{
	char					*key;
	char					*value;
	struct s_environ_node	*next;
}	t_environ_node;

typedef enum s_token
{
	EMPTY = 0,
	CMD = 1,
	REDERECT_IN = 2,
	rederect_out = 3,
	APPEND = 4,
	HEREDOC = 5,
	PIPE	= 6,
}	t_token;

typedef struct s_lexer_list
{
	char				*str;
	t_token				type;
	struct s_lexer_list	*prev;
	struct s_lexer_list	*next;
	// int					no_split;
}	t_lexer_list;

typedef struct s_executor
{
	int					id;
	int					size;
	int					fd_in;
	int					fd_out;
	bool				rederect_out;
	bool				append;
	bool				redirect_input;
	bool				heredoc;
	char				**path;
	char				**execs;
	struct s_executor	*prev;
	struct s_executor	*next;
}	t_executor;

typedef struct s_info
{
	int		wstatus;
	t_shell	*shell;
	int		stdin_copy;
	pid_t	*pids;
}	t_info;

typedef struct garbage_c
{
	void				*data;
	struct garbage_c	*next;
}	t_collect;

typedef struct s_varctx
{
	const char	*in;
	char		*out;
	int			max;
	size_t		pos[2];
	bool		single;
	bool		double_s;
	t_shell		*sh;
}	t_varctx;

typedef struct s_export_data
{
	char	*key;
	char	*value;
	bool	append;
}	t_export_data;

typedef struct s_replinfo
{
	size_t	i;
	size_t	o;
}	t_replinfo;

#endif