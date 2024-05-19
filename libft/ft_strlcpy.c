/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaunevik <vaunevik@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 08:34:43 by vaunevik          #+#    #+#             */
/*   Updated: 2024/01/22 08:59:33 by vaunevik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_strlcpy(char *dst, char *src, int dstsize)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (src[i] != '\0')
	{
		i++;
	}
	if (dstsize == 0)
		return (i);
	while (j < dstsize - 1 && src[j] != '\0')
	{
		dst[j] = src[j];
		j++;
	}
	dst[j] = '\0';
	return (i);
}
/*
#include <string.h>
int main ()
{
	char dst[20];
    const char src[] = "Hello world";
    size_t  result;

    result = strlcpy(dst, src, sizeof(dst));
    printf("Test 1 lib strlcat: %s Length: %zu\n", dst, result);
    result = strlcpy(dst, src, 5);
    printf("Test 2 lib strlcat: %s Length: %zu\n", dst, result);
    result = strlcpy(dst, src, 10);
    printf("Test 3 lib strlcat: %s Length: %zu\n", dst, result);

    result = ft_strlcpy(dst, src, sizeof(dst));
    printf("Test 1 my ft_strlcat: %s Length: %zu\n", dst, result);
    result = ft_strlcpy(dst, src, 5);
    printf("Test 2 my ft_strlcat: %s Length: %zu\n", dst, result);
    result = ft_strlcpy(dst, src, 10);
    printf("Test 3 my ft_strlcat: %s Length: %zu\n", dst, result);
    return (0);
} */
