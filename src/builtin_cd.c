/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 00:20:14 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/13 00:20:14 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_to_home(char **env, t_terminal *t);
static int	cd_to_oldcwd(char **env, char *oldcwd, t_terminal *t);
static int	cd_to_this(char *new_dir);
static char	*save_cwd(char cwd[], char **env, t_terminal *t);

int	builtin_cd(char **argv, int argc, t_terminal *t)
{
	char	*oldcwd;
	char	cwd[PATH_MAX];
	int		cd_exit_code;

	if (!t || !t->env)
		return (EXIT_SUCCESS);
	if (argc > 2)
	{
		ft_fprintf(ERROR, "cd: too many arguments\n");
		return (GENERAL_ERROR);
	}
	oldcwd = save_cwd(cwd, t->env, t);
	if (argc == 1 || !ft_strcmp(argv[1], "--"))
		cd_exit_code = cd_to_home(t->env, t);
	else if (!ft_strcmp(argv[1], "-"))
		cd_exit_code = cd_to_oldcwd(t->env, oldcwd, t);
	else
		cd_exit_code = cd_to_this(argv[1]);
	getcwd(cwd, PATH_MAX);
	env_set(t->env, "PWD", cwd, t);
	freen((void *)&oldcwd);
	return (cd_exit_code);
}

static char	*save_cwd(char cwd[], char **env, t_terminal *t)
{
	const char	*saved = (const char *)ft_strdup(env_lookup(env, "OLDPWD",
				VALUE));

	getcwd(cwd, PATH_MAX);
	env_set(env, "OLDPWD", cwd, t);
	return ((char *)saved);
}

static int	cd_to_home(char **env, t_terminal *t)
{
	const char	*home_ptr = (const char *)env_lookup(env, "HOME", VALUE);

	if (!home_ptr)
	{
		ft_fprintf(ERROR, "cd: HOME not set\n");
		return (GENERAL_ERROR);
	}
	if (chdir(home_ptr) == -1)
	{
		perror("cd");
		return (GENERAL_ERROR);
	}
	env_set(env, "PWD", (char *)home_ptr, t);
	return (EXIT_SUCCESS);
}

static int	cd_to_oldcwd(char **env, char *oldcwd, t_terminal *t)
{
	if (!oldcwd)
	{
		ft_fprintf(ERROR, "cd: OLDPWD not set\n");
		return (GENERAL_ERROR);
	}
	if (chdir(oldcwd) == -1)
	{
		perror(oldcwd);
		return (GENERAL_ERROR);
	}
	env_set(env, "PWD", oldcwd, t);
	ft_fprintf(STDOUT, "%s\n", (char *)oldcwd);
	return (EXIT_SUCCESS);
}

static int	cd_to_this(char *new_dir)
{
	if (!new_dir)
		return (GENERAL_ERROR);
	if (chdir(new_dir) == -1)
	{
		if (errno == ENOENT)
			ft_fprintf(ERROR, "cd: %s: No such file or directory\n", new_dir);
		else if (errno == ENOTDIR)
			ft_fprintf(ERROR, "cd: %s: Not a directory\n", new_dir);
		else if (errno == EACCES)
			ft_fprintf(ERROR, "cd: %s: Permission denied\n", new_dir);
		else
			perror("cd");
		return (GENERAL_ERROR);
	}
	return (EXIT_SUCCESS);
}
