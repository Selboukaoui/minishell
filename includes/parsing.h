/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:31:21 by asebban           #+#    #+#             */
/*   Updated: 2025/05/15 16:19:41 by selbouka         ###   ########.fr       */
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
int				check_redirect_out(char *input, int j);
int				check_redirect_in(char *input, int j);
void			int_to_str(int num, char *str);
char			*ft_strcpy(char *dest, const char *src);
char			*ft_strncpy(char *dest, const char *src, size_t n);
t_token			which_type(char *s, int inside);
int				herdoc_status(int action, int value);
char			*handle_dollar_quotes(char *input);
char			*replace_var_equals_var(char *input, t_shell *shell);
int				all_value(char *seg, t_shell *shell);
char			*change_all_var(const char *str, t_shell *sh);
int				calculate_max_len(const char *str, t_shell *shell);
char			*process_tokens(char **tokens, t_shell *shell);
int				var_name_len1(const char *seg, int pos);
int				create_heredoc(char *delimiter, t_shell *shell);
int				is_last_delim_quoted(const char *cmdline);
char			*replace_vars_heredoc(char *input, t_shell *shell);
t_executor		*process_lexemes(t_executor *list, \
t_executor *current, t_lexer_list **lexer, t_shell *shell);
int				process_command(t_executor *current, t_lexer_list *lexer);
int				process_in_heredoc(t_executor *cur, \
t_lexer_list *lex, t_shell *sh);
int				process_out_append(t_executor *current, t_lexer_list *lexer);
bool			open_outputfile(t_executor *current, t_lexer_list *lexer);
bool			is_delimiter(const char *line, const char *delimiter);
void			handle_eof(char *delimiter);
int				open_heredoc_pipe(int pipefd[2]);
size_t			buf_append(char *out, size_t o, const char *str);
size_t			append_exit_status(char *out, size_t o);
void			append_env_var(char *out, \
const char *start, t_shell *shell, size_t info[2]);
void			handle_variable_name(const char *input, \
size_t *idx, t_shell *shell, size_t *out_len);
void			calc_var_length(const char *input, \
size_t *idx, t_shell *shell, size_t *out_len);
size_t			handle_alpha_case(const char *input, \
char *out, t_replinfo *info, t_shell *shell);
void			lookup_and_append(const char *name_buf, \
char *out, t_replinfo *info, t_shell *shell);
size_t			extract_var_name(const char *input, \
t_replinfo *info, char *name_buf);
t_token			which_type(char *s, int inside);
int				find_token_pos(const char *rl, const char *s, int last_pos);
t_lexer_list	*create_lexer_list(char **input_array);
t_lexer_list	*allocate_lexer_nodes(int size);
int				count_input_array(char **input_array);
int				handle_expand_home(const char *in, \
char **res, int *i, t_shell *shell);
void			process_variable(const char *segment, \
char **res, int *i, t_shell *shell);
int				handle_expand_variable(const char *in, \
char **res, int *i, t_shell *shell);
void			process_home(const char *segment, \
char **res, int *i, t_shell *shell);
char			*segment_expand(const char *segment, t_shell *shell);
char			*process_export_segment(const char *seg, t_shell *shell);
char			*expand_variables(const char *input, t_shell *shell);
int				handle_skip_heredoc(const char *in, char **res, int *i);
int				should_toggle_squote(char c, int skip_word);
void			append_char(char **res, const char *src, int len);
void			process_skip_word(const char *segment, \
char **res, int *i, int *skip_word);
char			*get_env_value2(const char *key, t_shell *shell);
char			*ft_strtrim1(const char *s1);
int				is_cmdline_empty(const char *cmdline);
void			*ft_realloc(void *ptr, size_t old_size, size_t new_size);
size_t			parse_var_name(const char *str, char **out_var);
char			*expand_present_var(const char *trimmed, t_shell *shell);
char			*expand_absent_var(const char *trimmed, \
size_t var_len, t_shell *shell);
char			*expand_vars_in(const char *src, t_shell *shell);
char			*process_single_chunk(char *chunk, t_shell *shell);
void			append_chunk(char **dst, \
size_t *dst_len, const char *src, size_t len);
size_t			process_double_redirect(char **out, size_t *out_len, int flag);
size_t			process_single_operator(char **out, \
size_t *out_len, char sym);
size_t			process_regular_char(char **out, \
size_t *out_len, const char *in);
void			app_result(char **pres);
const char		*skip_leading_spaces(const char *s);
void			minishell_loop(t_shell *shell);
bool			check_tty(void);
int				skip_quates(char **input);

#endif