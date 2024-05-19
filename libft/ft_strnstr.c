/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaunevik <vaunevik@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 08:42:08 by vaunevik          #+#    #+#             */
/*   Updated: 2024/01/22 10:52:32 by vaunevik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strnstr(char *s1, char *s2, int n)
{
	int	s2len;
	int	i;
	int	j;

	s2len = ft_strlen(s2);
	if (s1 == s2 || s2len == 0)
		return (s1);
	i = 0;
	while (i < n && s1[i] != '\0')
	{
		j = 0;
		while (s1[i + j] != '\0' && s2[j] != '\0'
			&& (i + j) < n && s1[i + j] == s2[j])
		{
			j++;
			if ((j == n && j == s2len) || j == s2len)
				return ((s1 + i));
		}
		i++;
	}
	return (0);
}
/*#include <string.h>
int main()
{
	const char hay[] = "You are a teacher!";
	const char need[] = "teacher";
	size_t length = 25;
	char *res = ft_strnstr(hay, need, length);
	printf("Found at: %p\n", res);
	printf("Found the following: %s\n", res);

	res = strnstr(hay, need, length);
	printf("Found at (library version): %p\n", res);
    printf("Found the following (library version): %s", res);
	return (0);
}*/
