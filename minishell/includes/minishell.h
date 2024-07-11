/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:17:04 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/07/11 10:56:33 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

typedef enum e_token_type
{
	RED_IN,
	RED_OUT,
	DELIM,
	RED_OUT_APP,
	PIPE,
	QT_SIN,
	QT_DBL,
	WORD,
	CMD,
	ARG,
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

/************************/
/********* INIT *********/
/************************/

/* init.c */
t_main	*init_main(t_main *main, char **envp);

/* cleanup.c */
void	cleanup_main(t_main *main_struct);
void	free_double_array(char **array);
void	free_main_input(t_main *main_s);
void	free_tokens(t_token *token);

/* env.c */
char	**get_env(char **envp);

/************************/
/********* UTILS ********/
/************************/

/* len_utils.c */
int		count_arrays(char **array);
int		ft_strlen(const char *str);
int		count_words(const char *str);

/* str_utils.c */
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strdup(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);

/* is_x.c */
int		ft_isspace(int c);
int		ft_isoperator(int c);

/* split_spaces.c */
char	**split_into_words(char const *s);

/************************/
/****** PROCESSING ******/
/************************/

/* process_input.c */
void	process_input(t_main *main_s, char *user_input);
void	print_tokens(t_token *tokens);
char	*reorg_input(t_main *main_s);

/* trim_input.c */
char	*trim_input(t_main	*main_s, char *user_input);

/* tokenize_input.c */
t_token	*tokenize_input(char **words);
int		token_assign(t_token *token);

/* tokenize_refine_word.c */
t_token	*tokenize_refine_word(t_main *main_s);
void	reindex_tokens(t_token *first);
void	append_token_front(t_token **first, t_token *target, t_token *new);

/* reorg_word.c */
void	reorg_word(t_main *main_s, t_token *token);
t_token	*ft_token_new_late(char *string, int len);

/************************/
/******* PARSING ********/
/************************/

/* first_pars.c */
bool	first_pars(t_main *main_s, t_token *first);

/* error_msg.c */
void	syntax_error_msg(t_main *main_s);
void	syntax_error_pipe(t_main *main_s);

#endif
