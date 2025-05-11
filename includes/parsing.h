/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:31:21 by asebban           #+#    #+#             */
/*   Updated: 2025/05/10 19:28:27 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"

t_lexer_list	*tokenize(t_shell *shell);
t_lexer_list	*lexer(t_shell *shell);
char			*clean_rl_copy(char *rl_copy);
int				check_quote_syntax(const char *input);
bool			parser(t_shell *shell);
char			**ft_newsplit(const char *s);
int				ft_strcmp(const char *s1, const char *s2);
void			signal_setup(int mode);
int             check_redirect_out(char *input, int j);
int				check_redirect_in(char *input, int j);
void			int_to_str(int num, char *str);
char			*ft_strcpy(char *dest, const char *src);
char			*ft_strncpy(char *dest, const char *src, size_t n);
t_token			which_type(char *s, int inside);
// char			*replace_vars(char *input, t_shell *shell);
int				herdoc_status(int action, int value);
char			*handle_dollar_quotes(char *input);
char			*replace_var_equals_var(char *input, t_shell *shell);
int				all_value(char *seg, t_shell *shell);
char            *change_all_var(const char *str, t_shell *sh);
int             calculate_max_len(const char *str, t_shell *shell);
char            *process_tokens(char **tokens, t_shell *shell);
int             var_name_len1(const char *seg, int pos);
#endif