/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaunevik <vaunevik@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 15:02:28 by vaunevik          #+#    #+#             */
/*   Updated: 2024/01/22 14:07:12 by vaunevik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i++;
	}
	if (s[i] == (char)c)
		return ((char *)&s[i]);
	else
		return (NULL);
}
/*int main()
{
	const char s[] = "Hello there";
	int c = 'o';
	char *result;
	result = ft_strchr(s, c);
	printf("Address of first o in string: %p\n", &s[4]);
	printf("Address of first occurence: %p", result);
	return (0);
}*/
