/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaunevik <vaunevik@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 10:31:23 by vaunevik          #+#    #+#             */
/*   Updated: 2024/05/20 13:00:00 by vaunevik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/pipex.h"

void    make_children(t_pipex *pipex, char *command, int argc)
{
    if (!command ||!*command)
        exit(free_pip(pipex, err_msg(NO_CMD, 127, command));
    if (!get_commands(command, pipex))
        exit(free_pip(pipex, err_msg(MEM_ERR, 1, NULL)));
    access_path(pipex);
    close(pipex->fd[READ]);
    if (dup2(pipex->fd[WRITE], STDOUT_FILENO) == -1)
        exit(free_pip(pipex, err_msg(DUP_ERR, 1, NULL)));
    close(pipex->fd[WRITE]);
    execve(pipex->cmd, pipex->full_cmd, pipex->envp);
    exit(free_pip(pipex, err_msg(CMD_FAIL, 1, NULL)));
}

int main(int argc, char **argv, char **envp)
{
    t_pipex pipex;
	int		i;

	i = -1;
    if (argc < 5 ||(!ft_strncmp(argv[1], "here_doc\0", 9) && argc < 6))
        return(err_msg(INVAL_ARG, 1, NULL));
	if (!pipexify(&pipex, argc, argv, envp));
		return(err_msg(ERR_PERROR, 1, NULL));
    while (++i < (argc - 2 - pipex->heredoc))
	{
        if (pipe(pipex.fd) == -1)
            return (free_pip(&pipex, err_msg(ERR_PERROR, 1, NULL)));
        pipex.pid = fork();
        if (pipex.pid < 0)
            exit(free_pip(&pipex, err_msg(ERR_PERROR, 1, NULL)));
        else if (!pipex.pid)
            make_children(&pipex, pipex.argv[2 + pipex->heredoc + i], argc);
		if (dup2(pipex.fd[READ], STDIN_FILENO) == -1)
            exit(free_pip(&pipex, err_msg(DUP_ERR, 1, NULL)));
        close(pipex.fd[READ]);
        close(pipex.fd[WRITE]);
    }
    parent_process(&pipex, pipex.argv[2 + pipex->heredoc + i]);
    return (0);
}

int check_heredoc(t_pipex *pipex)
{
    int fd[2];
    char *line;

	if (ft_strncmp(pipex->argv[1], "here_doc\0", 9))
		return (0);
    if (pipe(fd) == -1)
        exit(free_pip(pipex, err_msg(PIPE_ERR, 1, NULL)));
    ft_putstr_fd("> ", 1);
    line = get_next_line(STDIN_FILENO);
    while (line && ft_strncmp(line, pipex->limiter, ft_strlen(line)))
    {
        ft_putstr_fd(line, fd[WRITE]);
        free_line(&line, 2);
        ft_putstr_fd("> ", 1);
        line = get_next_line(STDIN_FILENO);
    }
    free_line(&line, 2);
    close(fd[WRITE]);
    return (fd[READ]);
}

void    parent_process(t_pipex *pipex, char *command)
{
    open_outfile(pipex);
    close(pipex->outfile);
    if (!command ||!*command)
        exit(free_pip(pipex, err_msg(NO_CMD, 127, command)));
    if (!get_commands(command, pipex))
        exit(free_pip(pipex, err_msg(MEM_ERR, 1, NULL)));
    access_path(pipex);
    execve(pipex->cmd, pipex->full_cmd, pipex->envp);
    exit(free_pip(pipex, 1));
}

int	open_outfile(t_pipex *pipex)
{
	if (!access(pipex->argv[pipex->argc - 1], F_OK) && access(pipex->argv[pipex->argc - 1], W_OK))
		exit(free_pip(pipex, err_msg(NO_PERM, 1, NULL)));
	if (!pipex->heredoc)
		pipex->outfile = open(pipex->argv[pipex->argc - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else
		pipex->outfile = open(pipex->argv[pipex->argc - 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (pipex->outfile == -1 ||dup2(pipex->outfile, STDOUT_FILENO) == -1)
		exit(free_pip(pipex, err_msg(ERR_PERROR, 0, NULL)));
}

int open_infile(t_pipex *pipex)
{
	if (access(pipex->argv[1], F_OK | R_OK) == -1)
		return(err_msg(NO_PERM, 0, pipex->argv[1])); //should mby differentiate between a non-existent file and a file that cannot be open?
	pipex->infile = open(pipex->argv[1], O_RDONLY);
	if (pipex->infile == -1)
		return(1);
	if (dup2(pipex->infile, STDIN_FILENO) == -1)
		return (1);
}
