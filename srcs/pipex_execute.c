/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_execute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaunevik <vaunevik@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 11:46:28 by vaunevik          #+#    #+#             */
/*   Updated: 2024/05/20 14:14:02 by vaunevik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
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
                exit(free_pip(pipex, err_msg(ERR_PERROR, 126, NULL)));
            return ;
        }
        my_free(&path, 2);
    }
    /*if (!access(pipex->cmd, F_OK) && access(pipex->cmd, X_OK))
        exit(free_pip(pipex, err_msg(NO_PERM, 126, pipex->full_cmd[0])));
    if (!access(pipex->cmd, F_OK))
        return ;*/
    exit(free_pip(pipex, err_msg(NO_FILE, 127, pipex->full_cmd[0])));
}

