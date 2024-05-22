/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaunevik <vaunevik@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 12:40:34 by vaunevik          #+#    #+#             */
/*   Updated: 2024/05/22 10:43:11 by vaunevik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef LIBFT_H
# define LIBFT_H

# include <stdio.h>
# include <unistd.h>
# include "get_next_line.h"

char	**ft_split(char *s, char c);
char	*ft_substr(char *s, int start, int len);
char	*ft_strnstr(char *s1, char *s2, int n);
int		ft_strncmp(char *s1, char *s2, int n);
int		ft_strlen(char *s);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_strdup(char *s1);
int		ft_strlcpy(char *dst, char *src, int dstsize);
char	*ft_strchr(const char *s, int c);
void	ft_putstr_fd(char *s, int fd);

#endif
