/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaunevik <vaunevik@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 08:26:33 by vaunevik          #+#    #+#             */
/*   Updated: 2024/03/02 13:38:46 by vaunevik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"

void	free_list(t_list *list)
{
	t_list	*temp;

	if (!list)
		return ;
	while (list)
	{
		temp = list->next;
		free(list->content);
		free(list);
		list = temp;
	}
	list = NULL;
}

int	find_newline(t_list *node)
{
	int	i;

	i = 0;
	if (!node)
		return (0);
	while (node->content[i])
	{
		if (node->content[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

void	create_line(t_list **list, char **line, int length)
{
	int		j;
	t_list	*tmp;

	tmp = *list;
	if (tmp == NULL)
		return ;
	while (tmp)
	{
		j = 0;
		while (tmp->content[j] != '\0')
		{
			if (tmp->content[j] == '\n')
			{
				length++;
				break ;
			}
			length++;
			j++;
		}
		tmp = tmp->next;
	}
	*line = malloc((length + 1) * sizeof(char));
	if (!*line)
		return ;
	(*line)[length] = '\0';
}

void	pass_over_node(t_list *list, t_list **pass_over)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (list->content[j])
		j++;
	while (list->content[i] && list->content[i] != '\n')
		i++;
	if (list->content[i] == '\n')
		i++;
	(*pass_over)->content = malloc ((j - i) + 1);
	if (!(*pass_over)->content)
	{
		free(*pass_over);
		return ;
	}
	j = 0;
	while (list->content[i])
	{
		(*pass_over)->content[j] = list->content[i];
		i++;
		j++;
	}
	(*pass_over)->content[j] = '\0';
}
