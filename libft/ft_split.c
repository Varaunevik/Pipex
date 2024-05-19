/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaunevik <vaunevik@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 14:36:13 by vaunevik          #+#    #+#             */
/*   Updated: 2024/01/22 09:37:43 by vaunevik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	ft_wordcount(const char *str, char c);
static void	ft_freeall(char **str, int count);
static char	*ft_wordfill(char const *str, int start, char delimeter);
static void	ft_fill_split(char const *s, char c, char **split);

static int	ft_wordcount(const char *str, char c)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && str[i] == c)
			i++;
		if (str[i] && str[i] != c)
		{
			count++;
			while (str[i] && str[i] != c)
				i++;
		}
	}
	return (count);
}

static void	ft_freeall(char	**str, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(str[i]);
		i++;
	}
	free(str);
}

static char	*ft_wordfill(char const *str, int start, char delimeter)
{
	char	*element;
	int		i;

	i = 0;
	while (str[start + i] != delimeter && str[start + i] != '\0')
		i++;
	element = malloc((i + 1) * sizeof(char));
	if (!element)
		return (NULL);
	i = 0;
	while (str[start + i] != delimeter && str[start + i] != '\0')
	{
		element[i] = str[start + i];
		i++;
	}
	element[i] = '\0';
	return (element);
}

static void	ft_fill_split(char const *s, char c, char **split)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[j] && i < ft_wordcount(s, c))
	{
		while (s[j] && s[j] == c)
			j++;
		if (s[j] && s[j] != c)
		{
			split[i] = ft_wordfill(s, j, c);
			if (!split[i])
			{
				ft_freeall(split, i);
				return ;
			}
			i++;
			while (s[j] && s[j] != c)
				j++;
		}
	}
	split[i] = NULL;
}

char	**ft_split(char *s, char c)
{
	char	**split;

	if (!s)
		return (NULL);
	split = malloc((ft_wordcount(s, c) + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	ft_fill_split(s, c, split);
	return (split);
}
/*int main()
{
	char const split[] = "Hi can you split this?";
	char c = ' ';
	char **arr;
	arr = ft_split(split, c);
	int i = 0;
	while (arr[i] != NULL)
	{
		printf("[%s]", arr[i]);
		i++;
	}
	return (0);
}*/
