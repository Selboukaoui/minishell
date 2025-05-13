/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 21:47:29 by asebban           #+#    #+#             */
/*   Updated: 2025/05/13 14:54:59 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern int	g_signals;

static	char	*read_input(char *input)
{
	char	*line;

	(void)input;
	signal_setup(PARENT);
	g_signals = 0;
	line = readline("\001\033[0;32m\002minishell-1.0$~ \001\033[0m\002");
	g_signals = 1;
	return (line);
}

static	bool	handle_eof1(char *input, t_shell *shell)
{
	(void)shell;
	if (!input)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		ft_malloc(0, 0);
		exit(exit_status(0, 0));
	}
	return (true);
}

static	char	*preprocess_input(char *raw, t_shell *shell)
{
	char	*processed;

	if (*raw)
		add_history(raw);
	if (!check_syntax(raw))
		return (NULL);
	processed = clean_rl_copy(raw);
	free(raw);
	processed = process_line_expand_first_var(processed, shell);
	printf("1-------------->[%s]\n", processed);
	if (!processed)
		return (NULL);
	processed = handle_dollar_quotes(processed);
	printf("2-------------->[%s]\n", processed);
	if (!processed)
		return (NULL);
	processed = replace_var_equals_var(processed, shell);
	printf("3-------------->[%s]\n", processed);
	if (!processed)
		return (NULL);
	processed = export_hard(processed, shell);
	printf("4-------------->[%s]\n", processed);
	return (processed);
}
void replace_soh_with_dollar_in_env(t_environ_list *env_list)
{
    t_environ_node *current = env_list->head;

    while (current != NULL)
    {
        char *value_ptr = current->value;
        while (value_ptr != NULL && *value_ptr != '\0')
        {
            if (*value_ptr == '\x01')
            {
                *value_ptr = '$';
            }
            value_ptr++;
        }
        current = current->next;
    }
}

void replace_soh_with_dollar(char *str)
{
    char *p = str;
    while (p && *p) {
        if (*p == '\x01') {
            *p = '$';
        }
        p++;
    }
}

static	void	parse_and_execute(t_shell *shell)
{
	shell->rl_copy = replace_vars1(shell->rl_input, shell);
	printf("5-------------->[%s]\n", shell->rl_copy);
	replace_soh_with_dollar(shell->rl_copy);
	replace_soh_with_dollar_in_env(shell->env);
	if (!shell->rl_copy)
		return ;
	
	if (!parser(shell))
		return ;
	executor(shell);
}

void	minishell_loop(t_shell *shell)
{
	char	*input;

	while (true)
	{
		if (!check_tty())
			return ;
		input = read_input(input);
		if (!handle_eof1(input, shell))
			continue ;
		shell->rl_input = preprocess_input(input, shell);
		if (!shell->rl_input)
			continue ;
		parse_and_execute(shell);
	}
}
