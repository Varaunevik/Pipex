/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaunevik <vaunevik@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 11:46:17 by vaunevik          #+#    #+#             */
/*   Updated: 2024/05/20 13:57:32 by vaunevik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/pipex.h"

int pipexify(t_pipex *pipex, int argc, char **argv, char **envp)
{
    pipex->envp = envp;
    pipex->argc = argc;
    pipex->argv = argv;
    pipex->cmd = NULL;
    pipex->full_cmd = NULL;
    pipex->limiter = argv[2];
    pipex->heredoc = check_heredoc(pipex);
    if (pipex->heredoc)
    {
        pipex->infile = pipex->heredoc;
		pipex->heredoc = 1;
        if (dup2(pipex->infile, STDIN_FILENO) == -1)
            exit(free_pip(pipex, err_msg(ERR_PERROR, 1, NULL)));
    }
	pipex->infile = open_infile(pipex);
    pipex->outfile = 0;
    pipex->paths = split_envp(envp);
    if (!pipex->paths)
        return(free_pip(pipex, 0));
    return (1);
}

int free_pip(t_pipex *pipex, int error)
{
    my_free(pipex->paths, 1);
    my_free(pipex->full_cmd, 1);
    my_free(&pipex->cmd, 2);
    my_free(&pipex->limiter, 2);
    if (pipex->fd[READ])
        close(pipex->fd[READ]);
    if (pipex->fd[WRITE])
        close(pipex->fd[WRITE]);
    if (pipex->infile)
        close(pipex->infile);
    if (pipex->outfile)
        close(pipex->outfile);
    return(error);
}

void my_free(char **str, int opt)
{
    int i;

    if (str && opt == 1)
    {
        i = 0;
        while (str[i++])
        {
            free(str[i]);
            str[i] = NULL;
        }
        free(str);
    }
    else if (str && *str && opt == 2)
    {
        free(*str);
        *str = NULL;
    }
    str = NULL;
}

int    err_msg(int error, int exit, char *arg)
{
    ft_putstr_fd("pipex: ", 2);
    if (error == NO_CMD)
        ft_putstr_fd("command not found:", 2);
    if (error == NO_FILE)
        ft_putstr_fd("no such file or directory: ", 2);
    if (error == NO_PERM)
        ft_putstr_fd("permission denied: ", 2);
    if (error == CMD_FAIL)
        ft_putstr_fd("command failed: ", 2);
    if (error == INVAL_ARG)
        ft_putstr_fd("invalid number of arguments", 2);
    if (error == MEM_ERR)
        ft_putstr_fd("no memory available", 2);
    if (error == DUP_ERR)
        ft_putstr_fd("error dup fd", 2);
    if (error == PIPE_ERR)
        ft_putstr_fd("error creating pipe", 2);
    if (error == FORK_ERR)
        ft_putstr_fd("error forking process", 2);
    if (error == ERR_PERROR)
        perror();
    if (param && (error == NO_CMD ||error == NO_FILE ||error == NO_PERM ||error == CMD_FAIL))
        ft_putstr_fd(arg, 2);
    ft_putstr_fd("\n", 2);
    return(exit);
}









}
