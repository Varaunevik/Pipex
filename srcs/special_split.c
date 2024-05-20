/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaunevik <vaunevik@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 13:57:48 by vaunevik          #+#    #+#             */
/*   Updated: 2024/05/20 14:16:20 by vaunevik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/pipex.h"

int	special_spilt(char *cmd, t_pipex *pipex)
{
	int	i;
	int k;
	char del;

	i = 0;
	j = 0;
	pipex->full_cmd = (char **)malloc(sizeof(char *) * count_flags(command) + 1);
	if (!pipex->full_cmd)
		return (0);
	while (cmd[i++])
	{
		del = ' ';
		while (cmd && cmd[i] == del)
			i++;
		if (cmd[i] && ft_strchr("\"\'\0", cmd[i]))
			del = cmd[i++];
		while (cmd && cmd[i] == del)
			i++;
		if (cmd[i] && fill_flags(&cmd[i], del, pipex->full_cmd, j++))
		{
			my_free(pipex->full_cmd, 1);
			return(0);
		}
		while (cmd[i] && cmd[i] != del)
			i++;
	}
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
	int	count;
	int	i;
	char *flag;

	count = 0;
	i = -1;
	while (cmd[++i] && (cmd[i] != del))
		if (cmd[i] != '\\')
			count++;
	flag = (char *)malloc(sizeof(char) * count + 1);
	if (!flag)
		return (0);
	count = 0;
	i = -1;
	while (cmd[++i] && (whole[i] != del))
		if (cmd[i] != '\\')
			flag[count++] = cmd[i];
	full_cmd[pos] = flag;
	return (1);
}

static int	count_flags(char *command)
{
	int	count;
	int	i;
	char del;

	i = -1;
	count = 0;
	while (command[++i])
	{
		del = ' ';
		while (command[i] && command[i + 1] && command[i] == del)
			i++;
		if (command[i] && ft_strchr("\"\'\0", command[i]))
			del = command[i++];
		while (command[i] && command[i + 1] && command[i] == del)
			i++;
		if (command[i])
			count++;
		while (command[i] && command[i + 1] && command[i] != del)
			i++;
	}
	return (count);
}
