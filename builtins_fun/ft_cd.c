/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:03:20 by selbouka          #+#    #+#             */
/*   Updated: 2025/05/20 11:55:48 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	env_var_update(t_environ_list *env, char *key, const char *value)
{
	t_environ_node	*node;

	node = ft_getenv(env, key);
	if (node)
	{
		node->value = ft_strdup(value);
		if (node->value)
			return (OK);
		return (FAIL);
	}
	node = ft_malloc (sizeof(t_environ_node), 1);
	if (!node)
		return (FAIL);
	node->key = ft_strdup(key);
	node->value = ft_strdup(value);
	if (!node->key || !node->value)
		return (FAIL);
	node->next = env->head;
	env->head = node;
	env->size++;
	return (OK);
}

int	cd_no_args(t_shell *shell)
{
	t_environ_node	*home;
	char			old_pwd[PATH_MAX];

	home = ft_getenv(shell->env, "HOME");
	if (!home)
		return (ft_putstr_fd("cd: HOME not set\n", 2), FAIL);
	if (chdir(home->value) != 0)
		return (perror("minishell"), FAIL);
	if (!getcwd(old_pwd, sizeof(old_pwd)))
		return (ft_putstr_fd("minishell: getcwd failed\n", 2), FAIL);
	if (!env_var_update(shell->env, "PWD", home->value))
		return (FAIL);
	if (!env_var_update(shell->env, "OLDPWD", old_pwd))
		return (FAIL);
	return (OK);
}

int	handle_getcwd_failure(t_shell *shell, char *arg, char *new_pwd, char **x)
{
	printf("gtcwd failed\n");
	if (!ft_strcmp("..", arg) || ! ft_strcmp("../", arg))
	{
		if (!env_var_update(shell->env, "OLDPWD", ft_strjoin(new_pwd, *x)))
			return (FAIL);
		*x = ft_strjoin(*x, "/..");
		if (!env_var_update(shell->env, "PWD", ft_strjoin(new_pwd, *x)))
			return (FAIL);
	}
	else if (!ft_strcmp(".", arg) || !ft_strcmp("./", arg))
	{
		if (!env_var_update(shell->env, "OLDPWD", ft_strjoin(new_pwd, *x)))
			return (FAIL);
		*x = ft_strjoin(*x, "/.");
		if (!env_var_update(shell->env, "PWD", ft_strjoin(new_pwd, *x)))
			return (FAIL);
	}
	if (!arg)
		return (cd_no_args(shell));
	chdir(arg);
	// char	c[PATH_MAX];
	// int		flag;

	// flag = 0;
	
	// if (chdir(arg) != 0)
	// {
	// 	if ((!ft_strcmp(arg, "..") || !ft_strcmp(arg, "/..") ) && !getcwd(c, PATH_MAX))
	// 		(ft_cat(arg), flag = 1);
	// 	else
	// 		return (perror("minishell"), FAIL);
	// }
	// printf ("arg = %s\n", arg);
	// if (flag == 1 && chdir(arg) != 0)
	// 	return (perror("minishell"), FAIL);
	if (getcwd(new_pwd, PATH_MAX))
	{
		env_var_update(shell->env, "PWD", new_pwd);
		return (*x = NULL, OK);
	}
	return (ft_putstr_fd(ERR, 2), FAIL);
}
// if (!arg)
// return (cd_no_args(shell));
// if (ft_chdir(arg) == FAIL)
// return (FAIL);
// if (getcwd(new_pwd, PATH_MAX))
// {
// env_var_update(shell->env, "PWD", new_pwd);
// return (*x = NULL, OK);
// }
// return (ft_putstr_fd(ERR, 2), FAIL);
int	handle_normal_cd(t_shell *shell, char *arg, char *old_pwd, char *new_pwd)
{
	printf("Normal cd\n");
	if (chdir(arg) != 0)
	{
		printf ("arg == %s\npwd == %s\n", arg, getcwd(NULL, 0));
		return (perror("minishell"), FAIL);
	}
	// char	c[PATH_MAX];
	// int		flag;

	// flag = 0;
	// if (chdir(arg) != 0)
	// {
	// 	// printf ("arg (hndl normal)= %s\n", arg);
	// 	if ((!ft_strcmp(arg, "..") || !ft_strcmp(arg, "/..")) && open("2", O_RDONLY) == -1/*!getcwd(c, PATH_MAX)*/)
	// 		( flag = 1);
	// 	else
	// 	{
	// 		printf ("c = %s\n", c);
	// 		return (perror("XXXXminishell"), FAIL);
	// 	}
	// }
	// printf ("arg  =   %s\n", arg);
	// if (flag == 1 && chdir(arg) != 0)
	// 	return (perror("YYYYminishell"), FAIL);
	if (!getcwd(new_pwd, PATH_MAX))
		return (ft_putstr_fd("cd: error retrieving new directory\n", 2), FAIL);
	if (!env_var_update(shell->env, "PWD", new_pwd))
		return (FAIL);
	if (!env_var_update(shell->env, "OLDPWD", old_pwd))
		return (FAIL);
	return (OK);
}

int	cd(t_shell *shell, char **arg)
{
	static char	old_pwd[PATH_MAX];
	static char	new_pwd[PATH_MAX];
	static char	*x;

	if (arg[2])
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2), FAIL);
	if (!getcwd(old_pwd, PATH_MAX))
		return (handle_getcwd_failure(shell, arg[1], new_pwd, &x));
	if (!arg[1])
	{
		if (cd_no_args(shell) == FAIL)
			return (FAIL);
		return (OK);
	}
	return (handle_normal_cd(shell, arg[1], old_pwd, new_pwd));
}
