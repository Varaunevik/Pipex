#ifndef PIPEX_H
# define PIPEX_H

# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include "../libft/libft.h"

/* defines for error handling */
# define NO_CMD (int)-1
# define NO_FILE (int)-2
# define NO_PERM (int)-3
# define INVAL_ARG (int)-4
# define MEM_ERR (int)-5
# define PIPE_ERR (int)-6
# define DUP_ERR (int)-7
# define FORK_ERR (int)-8
# define NO_PATH (int)-9
# define CMD_FAIL (int)-10
# define ERR_PERROR (int)-11

# define READ 0
# define WRITE 1

/* defines for structure*/
typedef struct s_pipex
{
    int     infile;
    int     outfile;
    int     heredoc;
    char    *limiter;
    pid_t   pid;
    int     fd[2];
    int     argc;
    char    **argv;
    char    **envp;
    char    **paths;
    char    *cmd;
    char    **full_cmd;
}   t_pipex;


#endif