# pipex_main.c

### main()

1. Will check whether the amount of commands are correct.
2. Then it will call pipexify, which will initialize all values necessary for the program to begin. Such as opening infile, parsing the arguments and so on.
3. Then it will start iterating through all the commands (minus 1, which will be handled by the parent call and written to the outfile).
4. Inside the loop we start off by piping the fd stored in our struct. Then we fork() to create a child process. 
5. we check whether this is a child, and if so, we call the child_process function to execute the current command with the opened file descriptors (stored in the struct). 
6. Then we redirect the read end to the standard input and close both of the pipe ends before starting on the next command. 
7. When exiting the while loop it means we only have one command left, which we want to be handled by the parent, as it will not write into the outfile and not the write end of the pipe. 

You actually don’t call `wait()`, wait takes one `int*` argument that points to a integer where the child’s exit status will be stored. If you don’t care about the child’s exit status (which is quite often), you can just call `wait(NULL)`.

```c
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
			exit(free_pip(&pipex, err_msg(ERR_PERROR, 1, NULL)));
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

void wait_for_children(int num_children)
{
    while (--num_children > 0)
        waitpid(-1, NULL, 0);
}

```

### child_process()

1. Will check whether command exists (if it is empty).
2. We check whether we are on the first command and the infile does not exist (error when opening the infile), if that is the case we just want to exit the current child process and skip the command. This is just to behave similar to the bash shell command where we skip the first command whenever the infile does not exist/does not have correct permission. The next command will then read as from an empty file. 
3. Then it will call the cmd_split to split the current command (the command with its flags) and make it into a 2D matrix. This function will also check whether the path is an absolute path or just a standard command.
4. Then we will call the get_correct_path which will use access to check whether our command exists and whether it is executable
5. Then we close the read end of the pipe as we are not going to use it in this function. 
6. We will redirect the write end to the standard output and then close the write end. 
7. Lastly we call the execve function to execute our command using the correct file descriptors. 
8. If execve fails we will exit (the function will only reach exit if execve fails, if not, this child process will be “taken over” by the command, and also terminated by it)

```c
void	child_process(t_pipex *pipex, char *command, int count)
{
	if (!command || !*command)
		exit(free_pip(pipex, err_msg(NO_CMD, 0, command)));
	if (count == 0 && pipex->infile == -1)
		exit(free_pip(pipex, 0));
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
```

### last_cmd()

This function will only be called once all the child processes has executed their commands and can be seen as the “parent executer”. 

It will open the outfile and redirect the fd to the stdoutput so we can write the result of the command to that file. 

Its structure is otherwise quite similar to the child function, except some checks.

```c
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
```

### Open_infile()

This is a pretty straightforward functionn that will first check whether the infile exists. If it does not, we return an error message with 1 as return value. 

Then we check wether the file exists, but we do not have any read permission, if this is the case, we again return an error message and 1 as value. 

If the file exists and we have permissions, we want to go ahead and open the file in read only mode. 

Then we redirect the file descriptor to stdin getting ready for our first command to read from the file (which now is standard input). If any error occurs we return 1 to the caller function which will handle errors accordingly.

```c
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
```

### Open_outfile()

The function first checks whether the file exists, but with no write permission. If that is the case, we just want to exit the program because that is what the shell does.

We then check whether the heredoc is set to one, because if it is, we want to have some different file open commands. If we have no heredoc, we want to truncate if the file already exists, while if we have heredoc, we want to append to the file. 

Lastly we check whether there was any error when opening the file and we redirect the outfile to the stdout to write into it when we call our last command function. 

```c
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
```
