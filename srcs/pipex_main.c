/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaunevik <vaunevik@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 10:31:23 by vaunevik          #+#    #+#             */
/*   Updated: 2024/05/25 17:19:05 by vaunevik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/pipex.h"

static void	open_outfile(t_pipex *pipex);

void	child_process(t_pipex *pipex, char *command, int count)
{
	if (count == 0 && pipex->infile == -1)
		exit(free_pip(pipex, 0));
	if (!command || !*command)
		exit(free_pip(pipex, err_msg(NO_CMD, 0, command)));
	if (!cmd_split(command, pipex))
		exit(free_pip(pipex, err_msg(MEM_ERR, 1, NULL)));
	get_correct_path(pipex);
	close(pipex->fd[READ]);
	if (dup2(pipex->fd[WRITE], STDOUT_FILENO) == -1)
		exit(free_pip(pipex, err_msg(DUP_ERR, 1, NULL)));
	close(pipex->fd[WRITE]);
	execve(pipex->cmd, pipex->full_cmd, pipex->envp);
	exit(free_pip(pipex, err_msg(CMD_FAIL, 1, NULL)));
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int		i;

	i = -1;
	if (argc < 5 || (!ft_strncmp(argv[1], "here_doc\0", 9) && argc < 6))
		return (err_msg(INVAL_ARG, 1, NULL));
	if (!pipexify(&pipex, argc, argv, envp))
		return (err_msg(ERR_PERROR, 1, NULL));
	while (++i < (argc - 4 - pipex.heredoc))
	{
		if (pipe(pipex.fd) == -1)
			return (free_pip(&pipex, err_msg(ERR_PERROR, 1, NULL)));
		pipex.pid = fork();
		if (pipex.pid < 0)
			return(free_pip(&pipex, err_msg(ERR_PERROR, 1, NULL)));
		else if (!pipex.pid)
			child_process(&pipex, pipex.argv[2 + pipex.heredoc + i], i);
		close(pipex.fd[WRITE]);
		if (dup2(pipex.fd[READ], STDIN_FILENO) == -1)
			exit(free_pip(&pipex, err_msg(DUP_ERR, 1, NULL)));
		close(pipex.fd[READ]);
	}
	wait_for_children(i);
	last_cmd(&pipex, pipex.argv[2 + pipex.heredoc + i]);
	return (0);
}

void	last_cmd(t_pipex *pipex, char *command)
{
	open_outfile(pipex);
	close(pipex->outfile);
	if (!command || !*command)
		exit(free_pip(pipex, err_msg(NO_CMD, 127, command)));
	if (!cmd_split(command, pipex))
		exit(free_pip(pipex, err_msg(MEM_ERR, 1, NULL)));
	get_correct_path(pipex);
	execve(pipex->cmd, pipex->full_cmd, pipex->envp);
	exit(free_pip(pipex, 1));
}

static void	open_outfile(t_pipex *pipex)
{
	if (!access(pipex->argv[pipex->argc - 1], F_OK)
		&& access(pipex->argv[pipex->argc - 1], W_OK))
		exit(free_pip(pipex, err_msg(NO_PERM, 1, pipex->argv[pipex->argc
					- 1])));
	if (!pipex->heredoc)
		pipex->outfile = open(pipex->argv[pipex->argc - 1],
				O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else
		pipex->outfile = open(pipex->argv[pipex->argc - 1],
				O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (pipex->outfile == -1 || dup2(pipex->outfile, STDOUT_FILENO) == -1)
		exit(free_pip(pipex, err_msg(ERR_PERROR, 0, NULL)));
}

int	open_infile(t_pipex *pipex)
{
	if (access(pipex->argv[1], F_OK))
		return (err_msg(NO_FILE, 1, pipex->argv[1]));
	if (!access(pipex->argv[1], F_OK) && access(pipex->argv[1], R_OK))
		return (err_msg(NO_PERM, 1, pipex->argv[1]));
	pipex->infile = open(pipex->argv[1], O_RDONLY);
	if (pipex->infile == -1)
		return (1);
	if (dup2(pipex->infile, STDIN_FILENO) == -1)
		return (1);
	return (0);
}
