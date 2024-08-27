/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:17:04 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/08/27 11:13:47 by jqueijo-         ###   ########.fr       */
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
# include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

typedef struct s_env	t_env;
typedef struct s_token	t_token;
typedef struct s_pipex	t_pipex;
typedef struct s_main	t_main;

typedef enum e_token_type
{
	RED_IN,
	RED_OUT,
	HERE_DOC,
	RED_OUT_APP,
	PIPE,
	QUOTE,
	WORD,
	CMD,
	ARG,
	DELIM,
	PATH
}	t_token_type;

typedef struct s_env
{
	char	*value;
	char	*var;
	char	*var_value;
	int		index;
	t_env	*next;
}	t_env;

typedef struct s_token
{
	int		type;
	char	*value;
	char	**cmd;
	int		index;
	t_token *prev;
	t_token	*next;
}	t_token;

typedef struct s_pipex  //mine2
{
	pid_t		pid;
	int		status;
	char	*path;
	char	**cmd;
	int		pipe_fd[2];
	t_pipex *prev;
	t_pipex	*next;
}	t_pipex;

typedef struct s_main
{
	char	**menv;
	char	*user_input;
	char	*input_trim;
	char	*input_reorg;
	bool	silence_info;
	int		exe_fd[2]; //dont need
	char	***cmd; //dont need
	int		size; //dont need
	int		*fd_pipeline[2]; //dont need
	int		*pid_pipeline; //dont need
	t_env	*env;
	t_token	*tokens;
	t_pipex	*pipex;
	// t_main	*next; // don't need
}	t_main;

/************************/
/********* INIT *********/
/************************/

/* init.c */
void	init_main(t_main *main_s, char **envp);

/* cleanup.c */
void	free_main_input(t_main *main_s);
void	cleanup_main(t_main *main_struct);
void	free_double_array(char **array);
void	free_triple_array(char ***array);

/* cleanup_struct.c */
void	free_tokens(t_token *token);
void	free_env(t_env *first);

/* env.c */
t_env	*get_env(char **envp);

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
char	*ft_strcat(char *dest, const char *src);

/* is_x.c */
int		ft_isspace(int c);
int		ft_isoperator(int c);
int		ft_isquotes(int c);

/* print_utils.c */
void	print_tokens(t_token *tokens);
void	print_cmd_array(char ***cmd);

/************************/
/****** PROCESSING ******/
/************************/

/* process_input.c */
void	process_input(t_main *main_s, char *user_input);
char	*reorg_input(t_main *main_s);
char	*concat_tokens(t_token *first);

/* trim_input.c */
char	*trim_input(t_main	*main_s, char *user_input);

/* split_into_words.c */
char	**split_into_words(char const *s);

/* tokenize_input.c */
t_token	*tokenize_input(char **words);
int		token_assign(t_token *token);

/* tokenize_refine_word.c */
t_token	*tokenize_refine_word(t_main *main_s);
void	reindex_tokens(t_token *first);
void	append_token_front(t_token **first, t_token *target, t_token *new);

/* extract_operator_word.c */
void	extract_operator_word(t_main *main_s, t_token *current);

/* quotes_remover.c */
void	quotes_remover(t_token *first);

/* tokenize_smarter.c */
void	tokenize_smarter(t_token *first);
int		count_cmd_size(t_token *first);
t_token	*ft_token_new_late(char *string, int len);

/* create_cmd_array.c */
char	***create_cmd_array(t_main *main_s);

/************************/
/******* PARSING ********/
/************************/

/* first_pars.c */
bool	first_pars(t_main *main_s, t_token *first);
bool	parse_quotes(t_main *main_s, char *input);

/* error_msg.c */
void	syntax_error_msg(t_main *main_s);
void	syntax_error_pipe(t_main *main_s);

/* find_quotes.c */
bool	find_quotes(char *str);

/************************/
/******** PIPEX *********/
/************************/

int	ft_shell_pipex(t_main *main_s);
void	process_child_pipes(t_pipex *pipex_s);
void free_pipex_s(t_pipex *pipex_s);
void	close_all_fd(t_pipex *pipex_s);

//Create pipex_s structure
int		ft_process_tokens_s(t_main *main_s);
int ft_create_pipeline(t_main *main_s);
int	ft_update_pipex_s(t_token *tokens_s, t_pipex *pipex_s);
int	ft_update_cmds(t_token *tokens_s, t_pipex *pipex_s);
int	ft_update_fds(t_token *tokens_s, t_pipex *pipex_s);
int read_heredoc(t_token *tokens_s);
t_pipex *ft_init_pipex_s(void);

//Execute pipex cmds
void ft_exe_pipex_s(t_pipex *pipex_s, char **envp);
int	execute_command(t_pipex *pipex_s, char **envp); //temp
char	*get_cmd_path(char *cmd, char **envp);
void	exe_cmd_child(t_pipex *pipex_s, char **envp);

//pipex_utils
char	*ft_strnjoin(char *old_str, char *str_add, int size);
char	*ft_strstr(const char *big, const char *little);
char* get_file_name_from_fd(int fd);
void	print_struct(t_main *main_s);

#endif
