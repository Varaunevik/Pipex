/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaunevik <vaunevik@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 10:16:09 by vaunevik          #+#    #+#             */
/*   Updated: 2024/01/22 09:30:39 by vaunevik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_substr(char *s, int start, int len)
{
	int		strlen;
	char	*substr;

	if (s == NULL)
		return (NULL);
	strlen = ft_strlen(s);
	if (start >= strlen || len == 0)
		return (ft_strdup(""));
	if (strlen - start >= len)
	{
		substr = malloc((len + 1) * sizeof(char));
		if (!substr)
			return (NULL);
		ft_strlcpy(substr, &s[start], len + 1);
	}
	else
	{
		substr = malloc((strlen - start + 1) * sizeof(char));
		if (!substr)
			return (NULL);
		ft_strlcpy(substr, &s[start], strlen - start + 1);
	}
	return (substr);
}
/*int main ()
{
	char const s[] = "How are you?";
	unsigned int start  = 4;
	size_t len = 7;
	char *result;
	result = ft_substr(s, start, len);
	printf("Result my substr: %s", result);
	return (0);
}*/
