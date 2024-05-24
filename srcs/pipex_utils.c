/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaunevik <vaunevik@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 11:46:17 by vaunevik          #+#    #+#             */
/*   Updated: 2024/05/24 10:39:39 by vaunevik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/pipex.h"

static int	check_heredoc(t_pipex *pipex);
static char	*find_limiter(t_pipex *pipex);

int	pipexify(t_pipex *pipex, int argc, char **argv, char **envp)
{
	pipex->envp = envp;
	pipex->argc = argc;
	pipex->argv = argv;
	pipex->cmd = NULL;
	pipex->full_cmd = NULL;
	pipex->limiter = find_limiter(pipex);
	if (!pipex->limiter)
		return (0);
	pipex->heredoc = check_heredoc(pipex);
	if (pipex->heredoc)
	{
		pipex->infile = pipex->heredoc;
		pipex->heredoc = 1;
		if (dup2(pipex->infile, STDIN_FILENO) == -1)
			exit(free_pip(pipex, err_msg(ERR_PERROR, 1, NULL)));
	}
	else if (open_infile(pipex) == 1)
			exit(free_pip(pipex, 0));
	pipex->outfile = 0;
	pipex->paths = split_envp(envp);
	if (!pipex->paths)
		return (free_pip(pipex, 0));
	return (1);
}

static char	*find_limiter(t_pipex *pipex)
{
	char	*lim;

	lim = ft_strjoin(pipex->argv[2], "\n");
	if (!lim)
		return (0);
	return (lim);
}

static int	check_heredoc(t_pipex *pipex)
{
	int		fd[2];
	char	*line;

	if (ft_strncmp(pipex->argv[1], "here_doc\0", 9))
		return (0);
	if (pipe(fd) == -1)
		exit(free_pip(pipex, err_msg(PIPE_ERR, 1, NULL)));
	ft_putstr_fd("> ", 1);
	line = get_next_line(STDIN_FILENO);
	while (line && ft_strncmp(line, pipex->limiter, ft_strlen(line)))
	{
		ft_putstr_fd(line, fd[WRITE]);
		my_free(&line, 2);
		ft_putstr_fd("> ", 1);
		line = get_next_line(STDIN_FILENO);
	}
	my_free(&line, 2);
	close(fd[WRITE]);
	return (fd[READ]);
}

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
	if (error == PIPE_ERR)
		ft_putstr_fd("error creating pipe", 2);
	if (error == FORK_ERR)
		ft_putstr_fd("error forking process", 2);
	if (error == ERR_PERROR)
		perror(arg);
	ft_putstr_fd("\n", 2);
	return (exit);
}
