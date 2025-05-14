
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
	{
		free(raw);
		// ft_malloc(0, 0);
		return (NULL);
	}
	processed = clean_rl_copy(raw);
	free(raw);
	processed = process_line_expand_first_var(processed, shell);
	printf("---> str1 : %s\n", processed);
	if (!processed)
		return (NULL);
	processed = handle_dollar_quotes(processed);
	printf("---> str2 : %s\n", processed);
	if (!processed)
		return (NULL);
	processed = replace_var_equals_var(processed, shell);
	printf("---> str3 : %s\n", processed);
	if (!processed)
		return (NULL);
	processed = export_hard(processed, shell);
	printf("---> str4 : %s\n", processed);
	return (processed);
}

static	void	parse_and_execute(t_shell *shell)
{
	shell->rl_copy = replace_vars1(shell->rl_input, shell);
	printf("---> str5 : %s\n", shell->rl_copy);
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
