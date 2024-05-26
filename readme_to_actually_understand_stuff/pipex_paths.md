# pipex_paths.c

### Split_envp

1. Will find path from env by calling the getpath function and store it in a char string. 
2. Then it will split the string by calling split with : as delimeter. 
3. Then we iterate through the paths and add a / at the end of each string in order to create searchable paths. 
4. Then we free the envp path we extracted and return the “/” terminated matrix path. 

```c
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
```

## Get_path()

1. We check whether the envp exists and is not empty. If it is not empty, we start iterating through the matrix searching for the PATH= by using strnstr. 
2. If we find the matching string we call substr to store the string from the 5th index position, as we only want to store the paths, not the PATH= chars. 
3. Then we break out of the loop. 
4. If no path is found we set the path to a standard path line, which will look very similar to the envp path. 
5. Then we return the found path or the “artificial” path.

```c
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
```

### Get_correct_path

1. We check whether the path contains a /, which is it does, it means it is an absolute path and we don’t have to search through all the paths. 
2. We simply check whether the file exists by using access, and then we check whether we have execute permission. If we dont have permission or if the file did not exist, we return error values and exit the process. 
3. If the path did not contain a / we have to iterate through all paths in the envp and check, so we call check_paths.

```c
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
```

## check_paths

1. We iterate through all paths and for each iteration add the current command to the end of the path. This will create a complete path that we can use to search for the command. 
2. If the path is found, we will free the current stored command and update it with the complete path instead of just the command. 
3. Then we check whether it is executable, which, if it is, we return. 
4. After each search we free the path that we created before searching. 
5. If no command was found we return with an error message.

```c
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
```
