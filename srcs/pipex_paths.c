#include "../includes/pipex.h"

char	*get_path(char **envp)
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