# special_split.c

## cmd_split()

1. Checks whether the command exists or whether it is empty
2. Calls init_full_cmd to allocate space for the full command as a 2D array
3. Calls process_flags to iterate through the string in order to find and separate the flags
4. Calls ft_strdup with the first argument of the command (which is the “main” command, such as “cat” etc. - the one we will use when we search later)
5. If any error occurs we return 0. If all is good, we return 1.  

```c
int	cmd_split(char *cmd, t_pipex *pipex)
{
	if (!cmd || !*cmd)
		return (err_msg(NO_PERM, 0, NULL));
	if (!init_full_cmd(cmd, pipex))
		return (0);
	if (!process_flags(cmd, pipex))
		return (0);
	pipex->cmd = ft_strdup(pipex->full_cmd[0]);
	if (!pipex->cmd)
	{
		my_free(pipex->full_cmd, 1);
		return (0);
	}
	return (1);
}
```

### Init_full_cmd()

1. Will allocate space for the 2D matrix we want to store the command with its flags in. 
2. Finds the size by calling the count flags command and allocates space for them (plus one for NULL)

```c
static int	init_full_cmd(char *cmd, t_pipex *pipex)
{
	pipex->full_cmd = (char **)malloc(sizeof(char *) * (count_flags(cmd) + 1));
	if (!pipex->full_cmd)
		return (0);
	return (1);
}
```

## Process_flags()

1. Will iterate through the char string that holds the command with all its flags. 
2. We start off by setting the delimeter to space, and iterate through any trailing whitespace first. 
3. Then we check whether the character we currently are on is a “ or a ‘, which would be a case when for instance using grep “hello” and such. As we don’t want to grep “hello” but hello. 
4. This will now set the delimeter to either the single or double quote, indicating that the next time we meet it, it will be the end of the flag. 
5. We call the fill_flags function with the current delimeter (which could either be a space or single/double quotes and the current position in our main matrix)
6. Then, on return, we iterate over the letters until reaching the delimeter, which indicates we encounter a new flag. 
7. Then we null terminate the full_cmd array with a NULL pointer. 

```c
static int	process_flags(char *cmd, t_pipex *pipex)
{
	int		i;
	int		pos;
	char	del;

	i = 0;
	pos = 0;
	while (cmd[i])
	{
		del = ' ';
		while (cmd && cmd[i] == del)
			i++;
		if (cmd[i] && (cmd[i] == '"' || cmd[i] == '\''))
			del = cmd[i++];
		while (cmd && cmd[i] == del)
			i++;
		if (cmd[i] && !fill_flags(&cmd[i], del, pipex->full_cmd, pos++))
		{
			my_free(pipex->full_cmd, 1);
			return (0);
		}
		while (cmd[i] && cmd[i] != del)
			i++;
	}
	pipex->full_cmd[pos] = NULL;
	return (1);
}
```

## Fill_flags()

1. We start iterating over the flag until reaching the delimeter we set. This again could either be quotes or a space. 
2. We also check whether we have an escape character. If we do, we do not want to store it in our flag, and we don’t count it, nor do we store it. Essentially we just skip it to only get the character itself. So if we for instance have \’, we want to have the character ‘ and not the function it comes with (being a single quote). That is why we only save and count the char itself and not the escape character. If we were to remove this check, you will see that there will be some memory leaks. 
3. We iterate through and allocate memory for the number of characters in the command/flag, plus one for the null terminating character. Then we copy from the command to the newly allocated flag and assign the flag to the current position in the matrix we created in the other function above. 

```c
static int	fill_flags(char *cmd, char del, char **full_cmd, int pos)
{
	int		count;
	int		i;
	char	*flag;

	count = 0;
	i = 0;
	while (cmd[i] && (cmd[i] != del))
	{
		if (cmd[i] != '\\')
			count++;
		i++;
	}
	flag = (char *)malloc(sizeof(char) * (count + 1));
	if (!flag)
		return (0);
	flag[count] = '\0';
	count = 0;
	i = -1;
	while (cmd[++i] && (cmd[i] != del))
	{
		if (cmd[i] != '\\')
			flag[count++] = cmd[i];
	}
	full_cmd[pos] = flag;
	return (1);
}
```

### Count_flags()

```c
static int	count_flags(char *command)
{
	int		count;
	int		i;
	char	del;

	i = 0;
	count = 0;
	while (command[i])
	{
		del = ' ';
		while (command[i] && command[i] == del)
			i++;
		if (command[i] && (command[i] == '"' || command[i] == '\''))
			del = command[i++];
		while (command[i] && command[i] == del)
			i++;
		if (command[i])
			count++;
		while (command[i] && command[i] != del)
			i++;
	}
	return (count);
}
```
