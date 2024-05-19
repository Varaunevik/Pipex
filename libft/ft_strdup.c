/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaunevik <vaunevik@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 09:37:28 by vaunevik          #+#    #+#             */
/*   Updated: 2024/01/22 10:42:55 by vaunevik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strdup(char *s1)
{
	int		len;
	int		i;
	char	*cpy;

	len = 0;
	i = 0;
	while (s1[len] != '\0')
	{
		len++;
	}
	cpy = malloc((len + 1) * sizeof(char));
	if (!cpy)
		return (NULL);
	while (s1[i] != '\0')
	{
		cpy[i] = s1[i];
		i++;
	}
	cpy[i] = '\0';
	return (cpy);
}
/*int main()
{
	const char s1[] = "Heyyy";
	char *str;
	str = ft_strdup(s1);
	printf("Print string before: %s", s1);
	printf("Print result: %s", str);
	free(str);
	return (0);
}*/
