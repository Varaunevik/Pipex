/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaunevik <vaunevik@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 11:50:22 by vaunevik          #+#    #+#             */
/*   Updated: 2024/01/22 09:05:49 by vaunevik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_strncmp(char *s1, char *s2, int n)
{
	int	i;

	i = 0;
	if (n == 0)
	{
		return (0);
	}
	while (s1[i] == s2[i] && s1[i] != '\0' && i < n - 1)
	{
		i++;
	}
	if (s1[i] == s2[i])
	{
		return (0);
	}
	else
	{
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	}
}
/*
#include <string.h>
int main()
{
	const char s1[] = "Hello";
	const char s2[] = "el";
	size_t	n = 2;
	int	result;
	result = ft_strncmp(s1, s2, n);
	printf("My ft_strncmp: %d", result);
	result = strncmp(s1, s2, n);
	printf("Library strncmp: %d", result);
	return (0);
}*/
