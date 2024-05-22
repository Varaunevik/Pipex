/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaunevik <vaunevik@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 13:57:48 by vaunevik          #+#    #+#             */
/*   Updated: 2024/05/22 14:15:05 by vaunevik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/pipex.h"

static int	count_flags(char *command);
static int	fill_flags(char *cmd, char del, char **full_cmd, int pos);

int	cmd_split(char *cmd, t_pipex *pipex)
{
	int		i;
	int		pos;
	char	del;

	i = 0;
	pos = 0;
	if (!cmd || !*cmd)
		return (err_msg(NO_PERM, 0, NULL));
	pipex->full_cmd = (char **)malloc(sizeof(char *) * (count_flags(cmd) + 1));
	if (!pipex->full_cmd)
		return (0);
	while (cmd[i])
	{
		del = ' ';
		while (cmd && cmd[i] == del)
			i++;
		if (cmd[i] && ft_strchr("\"\'\0", cmd[i]))
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
	i = 0;
	while (cmd[i] && (cmd[i] != del))
	{
		if (cmd[i] != '\\')
			flag[count++] = cmd[i];
		i++;
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
		if (command[i] && ft_strchr("\"\'\0", command[i]))
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
