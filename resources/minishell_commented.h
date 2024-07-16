#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <stdlib.h>
# include <stddef.h>
# include <stdio.h>
# include <stdbool.h>
# include <unistd.h>

typedef struct s_token	t_token;
typedef struct s_main	t_main;

/* Other Possible sub-tokens: CMD_BUILTIN/EXTERNAL; PATH_ABSOLUTE/RELATIVE*/
typedef enum e_token_type
{
	RED_IN,
	RED_OUT,
	HERE_DOC,
	RED_OUT_APP,
	PIPE,
	QT_SIN,
	QT_DBL,
	WORD,
	CMD,
	ARG,
	DELIM,
	PATH
}	t_token_type;

typedef struct s_token
{
	int		type;
	char	*value;
	char	**cmd; // list of commands and flags for cmd tokens
	int		index;
	t_token	*next;
}	t_token;

typedef struct s_main
{
	char	**menv;
	t_token	*tokens;
	char	*user_input;
	char	*input_trim;
	char	*input_reorg;
	char	***cmd; // list of commands and flags to be execved
	int		size; // number of commands
	int		exe_fd[2]; // initial file descriptor from the < >> >> >
	int		*fd_pipeline[2]; // file descriptors from eachs of the execve processes rest is all closed
	int		*pid_pipeline; // process id of each of the execve child processes.
	bool	silence_info; // if string ends in & nothing will be printed at any point.
	t_main	*next; // for possible linked list?
}	t_main;

typedef struct s_all_mains
{
	t_main	*first; // linked list of t_main // structure for && ||
	char	**envp; // environment variables this will also be used for $VAR subs in the " "
	bool	silence_info; // if string ends in & nothing will be printed at any point.
}	t_all_mains;

#endif
