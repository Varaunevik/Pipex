/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaunevik <vaunevik@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 13:57:48 by vaunevik          #+#    #+#             */
/*   Updated: 2024/05/25 13:40:05 by vaunevik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/pipex.h"

static int	count_flags(char *command);
static int	fill_flags(char *cmd, char del, char **full_cmd, int pos);

static int	init_full_cmd(char *cmd, t_pipex *pipex)
{
	pipex->full_cmd = (char **)malloc(sizeof(char *) * (count_flags(cmd) + 1));
	if (!pipex->full_cmd)
		return (0);
	return (1);
}

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
