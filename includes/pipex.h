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

/*-----------------pipex_main.c----------------*/
int open_infile(t_pipex *pipex);
int main(int argc, char **argv, char **envp);

/*-----------------pipex_paths.c---------------*/
char	**split_envp(char **envp);
void	get_correct_path(t_pipex *pipex);

/*-----------------pipex_utilc.c---------------*/
int    err_msg(int error, int exit, char *arg);
void my_free(char **str, int opt);
int free_pip(t_pipex *pipex, int error);
int pipexify(t_pipex *pipex, int argc, char **argv, char **envp);

/*-----------------special_split.c-------------*/
int	special_spilt(char *cmd, t_pipex *pipex);

#endif
