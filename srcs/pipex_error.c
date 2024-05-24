/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaunevik <vaunevik@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 14:33:33 by vaunevik          #+#    #+#             */
/*   Updated: 2024/05/24 14:34:43 by vaunevik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/pipex.h"

int	free_pip(t_pipex *pipex, int error)
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
	return (error);
}

void	my_free(char **str, int opt)
{
	int	i;

	i = 0;
	if (str && opt == 1)
	{
		while (str[i])
		{
			free(str[i]);
			i++;
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

int	err_msg(int error, int exit, char *arg)
{
	ft_putstr_fd("pipex: ", 2);
	if (arg && (error == NO_CMD || error == NO_FILE || error == NO_PERM
			|| error == CMD_FAIL))
		ft_putstr_fd(arg, 2);
	if (error == NO_CMD)
		ft_putstr_fd(": command not found", 2);
	if (error == NO_FILE)
		ft_putstr_fd(": no such file or directory", 2);
	if (error == NO_PERM)
		ft_putstr_fd(": permission denied", 2);
	if (error == CMD_FAIL)
		ft_putstr_fd(": command failed", 2);
	if (error == INVAL_ARG)
		ft_putstr_fd("invalid number of arguments", 2);
	if (error == MEM_ERR)
		ft_putstr_fd("no memory available", 2);
	if (error == DUP_ERR)
		ft_putstr_fd("error dup fd", 2);
	if (error == FORK_ERR)
		ft_putstr_fd("error forking process", 2);
	if (error == ERR_PERROR)
		perror(arg);
	ft_putstr_fd("\n", 2);
	return (exit);
}
