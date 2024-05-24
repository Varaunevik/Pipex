/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_paths.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaunevik <vaunevik@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 10:33:03 by vaunevik          #+#    #+#             */
/*   Updated: 2024/05/24 14:04:48 by vaunevik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/pipex.h"

static void	check_paths(t_pipex *pipex);

void	get_correct_path(t_pipex *pipex)
{
	if (ft_strchr(pipex->cmd, '/'))
	{
		if (access(pipex->cmd, F_OK) == 0)
		{
			if (access(pipex->cmd, X_OK) != 0)
				exit(free_pip(pipex, err_msg(NO_PERM, 126, pipex->cmd)));
			return ;
		}
		else
			exit(free_pip(pipex, err_msg(NO_CMD, 127, pipex->full_cmd[0])));
	}
	else
		check_paths(pipex);
}

static void	check_paths(t_pipex *pipex)
{
	char	*path;
	int		i;

	i = -1;
	while (pipex->paths[++i])
	{
		path = ft_strjoin(pipex->paths[i], pipex->cmd);
		if (!path)
			exit(free_pip(pipex, err_msg(MEM_ERR, 1, NULL)));
		if (access(path, F_OK) == 0)
		{
			my_free(&pipex->cmd, 2);
			pipex->cmd = path;
			if (access(path, X_OK))
				exit(free_pip(pipex, err_msg(ERR_PERROR, 126, NULL)));
			return ;
		}
		my_free(&path, 2);
	}
	exit(free_pip(pipex, err_msg(NO_CMD, 127, pipex->full_cmd[0])));
}

static char	*get_path(char **envp)
{
	int		i;
	char	*path;

	i = 0;
	path = NULL;
	while (envp[i] != NULL && envp[i][0] != '\0')
	{
		path = ft_strnstr(envp[i], "PATH=", 5);
		if (path)
		{
			path = ft_substr(path, 5, ft_strlen(path));
			break ;
		}
		i++;
	}
	if (!path)
		path = ft_strdup
			("/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
	return (path);
}

char	**split_envp(char **envp)
{
	char	*path;
	char	**paths;
	char	*tmp;
	int		i;

	i = 0;
	path = get_path(envp);
	paths = ft_split(path, ':');
	tmp = NULL;
	while (paths[i])
	{
		tmp = paths[i];
		paths[i] = ft_strjoin(paths[i], "/");
		free(tmp);
		i++;
	}
	free(path);
	return (paths);
}
