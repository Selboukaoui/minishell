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

char			*get_next_token(const char *s, int *index);
char			*parse_token(const char *s, int *i);
int				parse_literal(const char *s, int *i, char **res);
int				parse_quoted(const char *s, int *i, char **res);
char			*append_segment(char *res, const char *seg, int len);
int				count_tokens(const char *s);
int				advance_token(const char *s, int idx);
int				ft_operator_len(const char *s);
char			*ft_dup_token(const char *str, int len);
int				skip_spaces(const char *s, int i);
char			*build_full_str(const t_environ_node *node);
void			print_sorted_env(t_environ_list *env);
void			print_sorted_nodes(const t_environ_node *sorted);
t_environ_node	*create_sorted_copy(const t_environ_list *env_list);
void			insert_sorted(t_environ_node **sorted, \
t_environ_node *new_node);
t_export_data	parse_export_str(const char *str);
void			update_node(t_environ_node *node, t_export_data *data);
void			add_new_node(t_shell *shell, t_export_data *data);
void			exporting(t_shell *shell, char *str);
int				export_args(t_shell *shell, char **args);
void			handle_no_file_error(char *path);
void			replace_soh_with_dollar_in_env(t_environ_list *env_list);
void			replace_soh_with_dollar(char *str);
char			*ft_replace_var3(const char *seg, t_shell *shell, int flag);
void			append_var(const char *seg, t_shell *shell, \
char *result, int ptrs[3]);
void			close_all_fds(void);
void			fill_replaced_segment(const char *seg, \
t_shell *shell, char *result, int flag);

#endif