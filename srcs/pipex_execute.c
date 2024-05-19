#include "../includes/pipex.h"

void check_path(t_pipex *pipex)
{
    char *path;
    int i;

    i = -1;
    while (pipex->paths[++i] && !ft_strchr(pipex->cmd, '/'))
    {
        path = ft_strjoin(pipex->paths[i], pipex->cmd);
        if (!path)
            exit(free_pip(pipex, err_msg(MEM_ERR, 1, NULL)));
        if (!access(path, F_OK))
        {
            my_free(&pipex->cmd, 2);
            pipex->cmd = path;
            if (access(path, X_OK))
                exit(my_free(pipex, err_msg(ERR_PERROR, 126, NULL)));
            return ;
        }
        my_free(&path, 2);
    }
    if (!access(pipex->cmd, F_OK) && access(pipex->cmd, X_OK))
        exit(free_pip(pipex, err_msg(NO_PERM, 126, pipex->full_cmd[0])));
    if (!access(pipex->cmd, F_OK))
        return ;
    exit (free_pip(pipex, err_msg(NO_FILE, 127, pipex->full_cmd[0])));
}

int get_commands(char *command, t_pipex *pipex)
{
    int     i;
    int     j;
    char    delimeter;

    i = 0;
    j = 0;
    pipex->full_cmd = (char **)malloc(sizeof(char *) * count_args(command) + 1);
    if (!pipex->full_cmd)
        return (0);
    while (pipex->full_cmd[i++])
    {
        delimeter = ' ';
        i += count_delimeter(&pipex->full_cmd[i], delimeter, 1);
        if (pipex->full_cmd[i] && ft_strchr("\'\"\0", pipex->full_cmd[i]))
            delimeter = pipex->full_cmd[i++];
        i += count_delimeter(&pipex->full_cmd[i], delimeter, 1);
        if (pipex->full_cmd[i] && fill_arguments(&pipex->full_cmd[i], delimeter, pipex->full_cmd, j++))
            return (*(int *)my_free(pipex->full_cmd, 1));
        i += count_delimeter(&pipex->full_cmd[i], delimeter, 1);
    }
    pipex->cmd = ft_strdup(pipex->full_cmd[0]);
    if (!pipex->cmd)
        return (*(int *)(my_free(pipex->full_cmd, 1)));
    return (1);
}

int fill_args(char *whole, char delimeter, char **full_cmd, int position)
{
    int count;
    int i;
    char *arg;

    count = 0;
    i = -1;
    while (whole[++i] && (whole[i] != delimeter))
        if (whole[i] != '\\')
            count++;
    arg = (char *)ft_calloc(sizeof(char), count + 1);
    if (!arg)
        return(0);
    count = 0;
    i = -1;
    while (whole[++i] && whole[i] != delimeter)
        if (whole[i] != '\\')
            arg[count++] = whole[i];
    full_cmd[position] = arg;
    return(1);
}

int	count_args(char *args)
{
	int		count;
	int		i;
	char	del;

	i = -1;
	count = 0;
	while (args[++i])
	{
		del = ' ';
        while (args[i] && args[i + 1] && args[i] == del)
            i++;
		if (args[i] && ft_strchr("\'\"\0", args[i]))
			del = args[i++];
        while (args[i] && args[i + 1] && args[i] == del)
            i++;
        if (args[i])
			count++;
        while (args[i] && args[i + 1] && args[i] != del)
            i++;
    }
	return (count);
}
