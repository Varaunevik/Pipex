#include "../includes/pipex.h"

void    make_children(t_pipex *pipex, char *command, int argc)
{
    if (!command ||!*command)
        exit(free_pip(pipex, err_msg(NO_CMD), 127, command));
    if (!get_commands(command, pipex))
        exit(free_pip(pipex, err_msg(MEM_ERR, 1, NULL)));
    access_path(pipex);
    if (close(pipex->fd[READ]) == -1)
        exit(free_pip(pipex, err_msg(ERR_PERROR, 1, NULL)));
    if (dup2(pipex->fd[WRITE], STDOUT_FILENO) == -1)
        exit(free_pip(pipex, err_msg(DUP_ERR, 1, NULL)));
    if (close(pipex->fd[WRITE]) == -1)
        exit(free_pip(pipex, err_msg(ERR_PERROR, 1, NULL)));
    execve(pipex->cmd, pipex->full_cmd, pipex->envp);
    exit(free_pip(pipex, err_msg(-1, 1, NULL)));
}

int main(int argc, char **argv, char **envp)
{
    t_pipex pipex;

    if (argc < 5 ||(!ft_strncmp(argv[1], "here_doc\0", 9) && argc < 6))
        return(err_msg(INVAL_ARG, 1, NULL));
    if (!pipexify(&pipex, argc, argv, envp))
        return(err_msg(ERR_PERROR, 1, NULL));
    while (arguments)
    {
        if (pipe(pipex.fd) == -1)
            return (free_pip(&pipex, err_msg(ERR_PERROR, 1, NULL)));
        pipex.pid = fork();
        if (pipex.pid < 0)
            exit(free_pip(&pipex, err_msg(ERR_PERROR, 1, NULL)));
        else if (!pipex.pid)
            make_children(&pipex, pipex.argv[cmd], argc); //need to update this and find a way for cmd count
        if (dup2(pipex.fd[READ], STDIN_FILENO) == -1)
            exit(free_pip(&pipex, err_msg(DUP_ERR, 1, NULL)));
        if (close(pipex.fd[READ]) == -1)
            exit(free_pip(&pipex, err_msg(ERR_PERROR, 1, NULL)));
        if (close(pipex.fd[WRITE]) == -1)
            exit(free_pip(&pipex, err_msg(ERR_PERROR, 1, NULL)));
    }
    parent_process(&pipex, pipex.argv[cmd]);
    return (0);
}

int check_heredoc(int argc, char **argv, t_pipex *pipex)
{
    int fd[2];
    char *line;

    if (argc <= 5 ||ft_strncmp(argv[1], "here_doc\0", 9))
        return(0);
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
    if (close(fd[WRITE]) == -1)
        exit(free_pip(pipex, err_msg(ERR_PERROR, 1, NULL)));
    return (fd[READ]);
}

void    parent_process(t_pipex *pipex, char *command)
{
    pipex->err = open_file(pipex, 1);
    if (pipex->err >= 0)
        exit(free_pip(pipex, err_msg(ERR_PERROR, pipex->err, NULL)));
    if (close(pipex->outfile) == -1)
        exit(free_pip(pipex, err_msg(ERR_PERROR, 1, NULL)));
    if (!command ||!*command)
        exit(free_pip(pipex, err_msg(NO_CMD, 127, command)));
    if (!get_commands(command, pipex))
        exit(free_pip(pipex, err_msg(MEM_ERR, 1, NULL)));
    access_path(pipex);
    execve(pipex->cmd, pipex->full_cmd, pipex->envp);
    exit(free_pip(pipex, 1));
}

int open_file(t_pipex *pipex, int file)
{
    if (!file && !pipex->heredoc)
    {
        if (access(pipex->argv[1], F_OK | R_OK) == -1)
            return(err_msg(NO_CMD, -2, pipex->argv[1]));
        pipex->infile = open(pipex->argv[1], O_RDONLY);
        if (pipex->infile == -1)
            return(1);
        if (dup2(pipex->infile, STDIN_FILENO) == -1)
            return (1);
    }
    else if (file == 1)
    {
            if (!access(pipex->argv[pipex->argc - 1], F_OK) && access(pipex->argv[pipex->argc - 1], W_OK))
                return (0);
            if (!pipex->heredoc)
                pipex->outfile = open(pipex->argv[pipex->argc - 1], O_CREAT |O_WRONLY |O_TRUNC, 0666);
            else 
                pipex->outfile = open(pipex->argv[pipex->argc - 1], O_CREAT |O_WRONLY |O_APPEND, 0666);
            if (pipex->outfile == -1 ||dup2(pipex->outfile, STDOUT_FILENO) == -1)
                return (1);
    }
    return (-1);
}