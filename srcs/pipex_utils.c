/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaunevik <vaunevik@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 11:46:17 by vaunevik          #+#    #+#             */
/*   Updated: 2024/05/25 17:18:36 by vaunevik         ###   ########.fr       */
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
		pipex->infile = -1;
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

void	wait_for_children(int num_children)
{
	while (--num_children > 0)
		waitpid(-1, NULL, 0);
}

static int	check_heredoc(t_pipex *pipex)
{
	int		fd[2];
	char	*line;

	if (ft_strncmp(pipex->argv[1], "here_doc\0", 9))
		return (0);
	if (pipe(fd) == -1)
		exit(free_pip(pipex, err_msg(ERR_PERROR, 1, NULL)));
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
