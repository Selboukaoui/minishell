/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_executor_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:18:37 by asebban           #+#    #+#             */
/*   Updated: 2025/05/10 19:26:39 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


extern int  g_signals;

static bool open_outputfile(t_executor *current, t_lexer_list *lexer)
{
	if (current->append)
		current->fd_out = open(lexer->str,
			O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		current->fd_out = open(lexer->str,
			O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (current->fd_out == -1)
	{
		perror(lexer->str);
		return false;
	}
	return true;
}


static int process_out_append(t_executor *current, t_lexer_list *lexer)
{
	// if (!lexer->next || lexer->next->type != EMPTY)
	// {
	//     // ft_putstr_fd("minishell: syntax error near unexpected token\n", STDERR_FILENO);
	//     return (FAILED);
	// }
	
	current->append = (lexer->type == APPEND);
	current->rederect_out = (lexer->type == rederect_out);
	if (!open_outputfile(current, lexer->next))
		return (FAILED);
	
	return (OK);
}



int ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' ||
			c == '\v' || c == '\f' || c == '\r');
}



static char *get_last_heredoc_delimiter(const char *cmdline)
{
	char *last = NULL;
	for (size_t i = 0; cmdline[i]; ++i) {
		if (cmdline[i] == '<' && cmdline[i + 1] == '<') {
			i += 2;
			while (cmdline[i] && ft_isspace(cmdline[i]))
				++i;
			size_t start = i;
			while (cmdline[i] && !ft_isspace(cmdline[i])
				   && cmdline[i] != '<' && cmdline[i] != '>' && cmdline[i] != '|')
				++i;
			size_t len = i - start;
			if (len > 0) {
				// free(last);
				last = ft_strndup(cmdline + start, len); // check strndup
			}
		}
	}
	return last;
}

static int is_last_delim_quoted(const char *cmdline)
{
	char *raw = get_last_heredoc_delimiter(cmdline);
	if (!raw)
		return 0;
	int quoted = (strchr(raw, '\'') != NULL || strchr(raw, '"') != NULL);
	// free(raw);
	return quoted;
}


static char *replace_vars_heredoc(char *input, t_shell *shell)
{
	size_t  i = 0, j = 0, len = ft_strlen(input);
	char    *output;
	int     in_single_quote   = 0;
	int     in_double_quote   = 0;
	int     seen_export       = 0;
	int     in_export_assign  = 0;
	int     k;

	{
		size_t p = 0;
		while (p < len && ft_isspace(input[p]))
			p++;
		if (p + 6 <= len
			&& ft_strncmp(&input[p], "export", 6) == 0
			&& (p + 6 == len || ft_isspace(input[p + 6])))
		{
			seen_export = 1;
		}
	}

	output = ft_malloc(PATH_MAX + 1, 1);
	if (!output)
		return NULL;

	while (i < len)
	{
		if (seen_export && !in_export_assign
			&& input[i] == '=')
		{
			in_export_assign = 1;
			output[j++] = input[i++];
			continue;
		}

		if (in_export_assign
			&& (ft_isspace(input[i]) || input[i] == '|'))
		{
			in_export_assign = 0;
			output[j++] = input[i++];
			continue;
		}

		if (input[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote, output[j++] = input[i++];
		else if (input[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote, output[j++] = input[i++];
		else if (input[i] == '$')
		{
			if (input[i + 1] == '\0')
			{
				output[j++] = '$';
				i++;
			}
			else if (input[i + 1] == '?')
			{
				if (seen_export && in_export_assign)
					output[j++] = '"';

				int  status = exit_status(0, 0);
				char status_str[12];
				int_to_str(status, status_str);
				k = 0;
				while (status_str[k])
					output[j++] = status_str[k++];
				i += 2;

				if (seen_export && in_export_assign)
					output[j++] = '"';
			}
			else if (ft_isdigit(input[i + 1]))
			{
				i += 2;
			}
			else if (ft_isalpha(input[i + 1]) || input[i + 1] == '_')
			{
				if (seen_export && in_export_assign)
					output[j++] = '"';

				int  var_start = i + 1;
				int  var_len   = 0;
				char var_name[256];

				while (ft_isalnum(input[var_start + var_len]) || input[var_start + var_len] == '_')
					var_len++;

				ft_strncpy(var_name, &input[var_start], var_len);
				var_name[var_len] = '\0';

				char *val = get_env_value(shell->env, var_name);
				if (val)
				{
					k = 0;
					while (val[k])
						output[j++] = val[k++];
				}
				i += var_len + 1;

				if (seen_export && in_export_assign)
					output[j++] = '"';
			}
			else
			{
				output[j++] = '$';
				i++;
			}
			continue;
		}
		else
		{
			output[j++] = input[i++];
		}
	}

	output[j] = '\0';
	return output;
}



int create_heredoc(char *delimiter, t_shell *shell)
{
	int pipefd[2];
	pid_t pid;
	int status;
	if (pipe(pipefd) == -1)
		return -1;

	// signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid < 0)
	{
		close(pipefd[0]); close(pipefd[1]);
		return (-1);
	}
	signal_setup(2);
	if (pid == 0)
	{
		// signal(SIGQUIT, SIG_IGN);
		g_signals = 69;
		rl_catch_signals = 1;
		int quoted = is_last_delim_quoted(shell->rl_copy);
		close(pipefd[0]);
		while (1) {
			char *line = readline("> ");
			if (g_signals == 130)
			{
				// herdoc_status(1, 1);
				// free(line);
				// raise(SIGINT);
				// printf("I was here\n");
				exit(130);
			}
			if (!line) {
				// Ctrl-D pressed
				ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", STDERR_FILENO);
				ft_putstr_fd(delimiter, STDERR_FILENO);
				ft_putstr_fd("')\n", STDERR_FILENO);
				exit(0);
			}
			if (ft_strcmp(line, delimiter) == 0) {
				// free(line);
				exit(0);
			}
			if (!quoted) {
				// char *expanded = replace_vars(line, shell); thsi case : << c cat
				// > '$HOME'
				// > "'$PWD'"
				// > c
				char *expanded = replace_vars_heredoc(line, shell);
				write(pipefd[1], expanded, ft_strlen(expanded));
				write(pipefd[1], "\n", 1);
				// free(expanded);
			} else {
				write(pipefd[1], line, ft_strlen(line));
				write(pipefd[1], "\n", 1);
			}
			// free(line);
		}
	}
	else{
		close(pipefd[1]);
		waitpid(pid, &status, 0);
		// printf("%d\n",status);
		if (WIFEXITED(status))
		{
			if(status == 33280)
			{
				close(pipefd[0]);
				return -2;
			}
		}

		// if (WIFSIGNALED(status) &&
		//     WTERMSIG(status) == SEGINT) {
		//     close(pipefd[0]);
		//     // printf("exit from here\n");
		//     return -2;
		// }
		// printf("=%d\n", g_signals);
		return pipefd[0];
	}
}

static int process_in_heredoc(t_executor *cur, t_lexer_list *lex, t_shell *sh)
{
	if (lex->type == HEREDOC) 
	{
		cur->fd_in = create_heredoc(lex->next->str, sh);
		// printf("%d\n", cur->fd_in);
		if (cur->fd_in == -1 || cur->fd_in == -2)
			return FAILED;
	} 
	else {
		cur->fd_in = open(lex->next->str, O_RDONLY);
		if (cur->fd_in == -1) {
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			perror(lex->next->str);
			return FAILED;
		}
	}
	return OK;
}

static int process_command(t_executor *current, t_lexer_list *lexer)
{
	int size = 0;
	t_lexer_list *tmp = lexer;

	while (tmp && tmp->type == CMD)
	{
		size++;
		tmp = tmp->next;
	}

	current->execs = (char **)ft_calloc(size + 1, sizeof(char *));
	if (!current->execs)
		return (FAILED);

	for (int i = 0; i < size; i++)
	{
		current->execs[i] = ft_strdup(lexer->str);
		if (!current->execs[i])
			return (FAILED);
		lexer = lexer->next;
	}
	return (OK);
}

static t_executor *process_lexemes(t_executor *list, t_executor *current, t_lexer_list **lexer, t_shell *shell)
{
	int ret;
	while (*lexer)
	{
		if ((*lexer)->type == PIPE)
		{
			(*lexer) = (*lexer)->next;
			break;
		}
		if ((*lexer)->type == rederect_out || (*lexer)->type == APPEND)
		{
			ret = process_out_append(current, *lexer);
			if (ret == FAILED)
				return (/*free_executor_list(list)*/NULL);
			*lexer = (*lexer)->next->next;
		}
		else if ((*lexer)->type == REDERECT_IN || (*lexer)->type == HEREDOC)
		{
			ret = process_in_heredoc(current, *lexer, shell);
			if (ret == FAILED)
				return (/*free_executor_list(list)*/NULL);
			*lexer = (*lexer)->next->next;
		}
		else if ((*lexer)->type == CMD)
		{
			ret = process_command(current, *lexer);
			if (ret == FAILED)
				return (/*free_executor_list(list)*/NULL);
			while (*lexer && (*lexer)->type == CMD)
				*lexer = (*lexer)->next;
		}
		else
		{
			*lexer = (*lexer)->next;
		}
		// if (herdoc_status(0, 0) == 1)
		// {
		//     printf("here\n");
		//     exit_status(1,130);
		//     // herdoc_status(1, 0);
		//     break;
		// }
	}
	return (list);
}


t_executor	*fill_executor_list(t_shell *shell, t_executor *list)
{
	t_executor		*current;
	t_lexer_list	*lexer;

	current = list;
	lexer = shell->lex_head; 
	while (current)
	{
		list = process_lexemes(list, current, &lexer, shell);
		if (!list)
		{
			exit_status(1, 1);
			return (NULL);
		}
		exit_status(1, 0);
		current = current->next;
	}
	return (list);
}
