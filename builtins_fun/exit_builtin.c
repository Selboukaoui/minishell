/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:11:51 by asebban           #+#    #+#             */
/*   Updated: 2025/05/13 15:05:50 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static	void	clean_exit(t_shell *shell, int exit_code)
{
	(void)shell;
	ft_malloc(0, 0);
	exit(exit_code);
}

static	bool	is_arg_number(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	if (!arg[i])
		return (false);
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (false);
		i++;
	}
	return (true);
}

int	exit_builtin(t_shell *shell, char **args, int in_pipe)
{
	int	exit_code;
	int	arg_count;

	arg_count = 0;
	exit_code = exit_status(0, 0);
	while (args[arg_count])
		arg_count++;
	if (arg_count == 1 && !in_pipe)
		(printf("exit\n"), clean_exit(shell, exit_code));
	if (arg_count == 1 && in_pipe)
		clean_exit(shell, exit_code);
	if (!is_arg_number(args[1]))
	{
		(printf("exit\n"), ft_putstr_fd("minishell: exit: ", STDERR_FILENO));
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		clean_exit(shell, 2);
	}
	if (arg_count > 2)
	{
		exit_status(1, 127);
		return (ft_putstr_fd("exit\nminishell: exit: too many arguments\n",
				STDERR_FILENO), EXIT_FAILURE);
		
	}
	printf("exit\n");
	exit_code = ft_atoi(args[1]);
	return (clean_exit(shell, exit_code), EXIT_SUCCESS);
}
