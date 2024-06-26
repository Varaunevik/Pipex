# pipex_utils.c

### pipexify()

This function is initializing our struct and calling corresponding functions to fill arrays and split all our arguments etc. 

If heredoc is present we want the infile to be set to it, as we don’t have an infile. Then we set heredoc to 1 as a flag and redirect the heredoc to the stdin_fileno.

If we dont have heredoc, we want to open the infile by calling that function. If the return value from this function is 1 it tells us an error occured on opening the infile, and we set the infile to -1 as a flag for the child process function to later check. 

Then we call split_envp to get all our paths to get ready for checking our paths so we can use execve.

```c
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
```

## check_heredoc

Will check whether heredoc is present first by using strncmp to compare the second argument with here_doc\0. 

If it is present, we continue by opening a pipe which will pipe input from the heredoc (stdin) to the second command, as we don’t have any real file we can open with a real file descriptor to redirect. 

Then we start off by printing to the stdin (which currently is the heredoc). 

Then we read from the stdin by using get next line. 

We read line by line from stdin until the user sends us the limiter (we check this by using strncmp again). 

We make sure to put the string we just read into the write end of the open pipe, and we free the line we read (and stored) using gnl. After each read we print a new promt “>” and call get next line again. 

Then, when limiter is met, we exit the loop and free the last line. Then we close the write end of the file descriptor and return the read end, which will be used for the next command. 

```c

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
```
