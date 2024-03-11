# ifndef CUB3D_H
# define CUB3D_H

# include "../get_next_line/get_next_line.h"
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>
#	include <readline/readline.h>
#	include <readline/history.h>

# define ERR_INFILE "Infile"
# define ERR_OUTFILE "Outfile"
# define ERR_INPUT "Invalid number of arguments.\n"
# define ERR_PIPE "Pipe"
# define ERR_CMD "Command not found\n"

typedef struct s_colors
{
	void *arg1;
	void *arg2;
	void *arg3;
} t_colors;

#endif