/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaunevik <vaunevik@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:18:00 by vaunevik          #+#    #+#             */
/*   Updated: 2024/05/22 14:21:32 by vaunevik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <sys/wait.h>
# include <unistd.h>

/* defines for error handling */
# define NO_CMD -1
# define NO_FILE -2
# define NO_PERM -3
# define INVAL_ARG -4
# define MEM_ERR -5
# define PIPE_ERR -6
# define DUP_ERR -7
# define FORK_ERR -8
# define NO_PATH -9
# define CMD_FAIL -10
# define ERR_PERROR -11

# define READ 0
# define WRITE 1

/* defines for structure*/
typedef struct s_pipex
{
	int		infile;
	int		outfile;
	int		heredoc;
	char	*limiter;
	pid_t	pid;
	int		fd[2];
	int		argc;
	char	**argv;
	char	**envp;
	char	**paths;
	char	*cmd;
	char	**full_cmd;
}			t_pipex;

/*-----------------pipex_main.c----------------*/
int			open_infile(t_pipex *pipex);
int			main(int argc, char **argv, char **envp);
void		child_process(t_pipex *pipex, char *command);
void		parent_process(t_pipex *pipex, char *command);

/*-----------------pipex_paths.c---------------*/
char		**split_envp(char **envp);
void		get_correct_path(t_pipex *pipex);

/*-----------------pipex_utilc.c---------------*/
int			err_msg(int error, int exit, char *arg);
void		my_free(char **str, int opt);
int			free_pip(t_pipex *pipex, int error);
int			pipexify(t_pipex *pipex, int argc, char **argv, char **envp);

/*-----------------special_split.c-------------*/
int			cmd_split(char *cmd, t_pipex *pipex);

#endif
