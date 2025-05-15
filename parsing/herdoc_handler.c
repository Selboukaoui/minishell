/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 11:34:49 by asebban           #+#    #+#             */
/*   Updated: 2025/05/15 13:32:31 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern int	g_signals;

static	void	write_content(int fd,
		const char *line, t_shell *shell, bool quoted)
{
	char	*to_write;
	size_t	len;

	if (!quoted)
	{
		to_write = replace_vars_heredoc((char *)line, shell);
		len = ft_strlen(to_write);
	}
	else
	{
		to_write = (char *)line;
		len = ft_strlen(line);
	}
	write(fd, to_write, len);
	write(fd, "\n", 1);
}

void	heredoc_child_loop(int write_fd,
		char *delimiter, t_shell *shell, int quoted)
{
	char	*line;

	while (true)
	{
		line = readline("> ");
		if (g_signals == 130)
		{
			(free(line), ft_malloc(0, 0));
			exit(130);
		}
		if (!line)
		{
			handle_eof(delimiter);
			ft_malloc(0, 0);
			exit(0);
		}
		if (is_delimiter(line, delimiter))
		{
			free(line);
			ft_malloc(0, 0);
			exit(0);
		}
		write_content(write_fd, line, shell, quoted);
		free(line);
	}
}

static	int	fork_heredoc_child(int pipefd[2],
		char *delimiter, t_shell *shell)
{
	pid_t	pid;
	int		quoted;

	pid = fork();
	if (pid < 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	signal_setup(2);
	if (pid == 0)
	{
		g_signals = 69;
		rl_catch_signals = 1;
		quoted = is_last_delim_quoted(shell->rl_copy);
		close(pipefd[0]);
		heredoc_child_loop(pipefd[1], delimiter, shell, quoted);
	}
	return (pid);
}

static	int	wait_and_return_heredoc(int pid, int read_fd)
{
	int	status;

	(void)read_fd;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 130)
		{
			close(read_fd);
			return (-2);
		}
	}
	return (read_fd);
}

int	create_heredoc(char *delimiter, t_shell *shell)
{
	int	pipefd[2];
	int	pid;
	int	result_fd;

	if (open_heredoc_pipe(pipefd) < 0)
		return (-1);
	pid = fork_heredoc_child(pipefd, delimiter, shell);
	if (pid < 0)
		return (-1);
	close(pipefd[1]);
	result_fd = wait_and_return_heredoc(pid, pipefd[0]);
	return (result_fd);
}
