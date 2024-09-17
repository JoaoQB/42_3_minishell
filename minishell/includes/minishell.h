/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:17:04 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/13 16:54:12 by jqueijo-         ###   ########.fr       */
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
#include <signal.h>
#include <limits.h>

# define MAX_HIST 30

typedef struct s_env	t_env;
typedef struct s_token	t_token;
typedef struct s_pipex	t_pipex;
typedef struct s_main	t_main;
typedef struct s_hist	t_hist;
extern volatile sig_atomic_t g_signal;

typedef enum e_neg_operator
{
	NEG_PIPE = -1,
	NEG_LESS = -2,
	NEG_GREATER = -3,
	NEG_VAR = -4
} t_neg_operator;

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
	PATH,
	CONC,
	VAR
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
	int		pipe_fd[2]; //restruct in case of *
	t_pipex *prev;
	t_pipex	*next;
	t_main	*main_s;
}	t_pipex;

typedef struct s_hist
{
	int		idx;
	char	*usr_input;
    struct s_hist *next;
    struct s_hist *prev;
} t_hist;

typedef struct s_main
{
	char	**menv;
	char	*user_input;
	char	*input_trim;
	char	*input_reorg;
	bool	silence_info;
	t_env	*env;
	t_token	*tokens;
	t_pipex	*pipex; //fandre-b added
	t_hist	*history; //fandre-b added
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
void	ft_free(char **str);

/* cleanup_struct.c */
void	free_tokens(t_token **first);
void	free_env(t_env **first);

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

/* str_utils2.c */
int		ft_strcmp(const char *s1, const char *s2);
size_t	ft_strlcpy2(char *dst, const char *src, size_t size);

/* char_extract_utils.c */
char	*extract_before_i(char *str, int i);
char	*extract_from_i(char *str, int i);
int		iterate_quotes(char *str);

/* is_x.c */
int		ft_isspace(int c);
int		ft_isoperator(int c);
int		ft_isquotes(int c);
int		ft_ischar(int c);
int		ft_isdigit(int c);

/* print_utils.c */
void	print_tokens(t_token *tokens);
void	print_cmd_array(char ***cmd);
void	print_env(t_env *env);
void	print_menv(char **menv);

/* token_utils.c */
void	reassign_tokens(t_token *first);
void	reindex_tokens(t_token *first);
void	token_append_before(t_token **first, t_token *target, t_token *new);
void	token_append_after(t_token *target, t_token *new);
void	token_extract_before(t_token **first, t_token *current, int i);

/* token_utils2.c */
void	token_extract_after(t_token *current, int i);
void	clean_tokens(t_token **first);
void	delete_token(t_token **first, t_token *current);

/* var_utils.c */
int		ft_isvar1stchar(int c);
int		ft_isvarchar(int c);
char	*var_extract_before(t_token **first, t_token *current, int i);
char	*var_extract_after(t_token *current, int i);
char	*extract_inside_quotes(char *str);

/* fd_utils.c */
void	ft_putendl_fd(char *s, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putchar_fd(char c, int fd);

/* to_utils.c */
int		ft_atoi(const char *nptr);
double	ft_atoi_dbl(const char *nptr);

/************************/
/****** PROCESSING ******/
/************************/

/* input_process.c */
void	input_process(t_main *main_s, char *user_input);
char	*input_and_tokens_reorg(t_main *main_s);
char	*concat_tokens_to_char(t_token *first);

/* input_trim_and_tokenize.c */
char	*input_trim_and_tokenize(t_main *main_s, char *user_input);

/* split_into_words.c */
char	**split_into_words(char const *s);

/* input_tokenize.c */
t_token	*input_tokenize(char **words);
int		token_assign(char *str);

/* token_separate_operator.c */
t_token	*token_separate_operator(t_main *main_s);

/* tilde_replace.c */
void	tilde_replace(t_main *main_s, t_token *first);

/* quotes_hide_operators.c */
void	quotes_hide_operators(t_token *first);
void	hide_operators(char *str);
void	quotes_revert_operators(t_token *first);
void	revert_operators(char *str);

/* token_split_words.c */
void	token_split_words(t_token **first);

/* quotes_remover.c */
void	quotes_remover(t_token *first);

/* tokenize_smarter.c */
void	tokenize_smarter(t_token *first);

/* var_swap.c */
void	var_swap(t_main *main_s, t_token **first);

// /* var_divide.c */
// void	var_divide(t_main *main_s, t_token **first, t_token *current);

/* var_conc.c */
t_token	*var_conc(t_token *target, t_token **first);

/* var_remove_quotes.c */
t_token	*var_remove_quotes(t_token **first, t_token *current, int i);
t_token	*var_conc_quotes(t_token **first, t_token *current);
char	*var_extract_after(t_token *current, int i);

/* var_replace.c */
t_token	*var_replace(t_main *main_s, t_token *current, int i);

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
t_pipex *ft_init_pipex_s(t_main *mains_s);

//Execute pipex cmds
int	edge_cases(t_pipex *pipex_s);
void ft_exe_pipex_s(t_main *main_s, char **envp);
int	execute_command(t_pipex *pipex_s, char **envp);
char	*get_cmd_path(char *cmd, char **envp);
void	exe_cmd_child(t_pipex *pipex_s, char **envp);

//pipex_utils
char	*ft_strnjoin(char *old_str, char *str_add, int size);
char	*ft_strstr(const char *big, const char *little);
char* get_file_name_from_fd(int fd);
void	print_struct(t_main *main_s);
void	print_check_processes(t_pipex *pipex_s);

/************************/
/******* HISTORY ********/
/************************/

t_hist *ft_init_hist_s(void);
void add_to_history(t_main *main_s);
void get_history(t_hist *hist_s, int index);
void    ft_rm_history(t_hist **hist_s);
void free_history(t_hist *hist_s);

/************************/
/****** EDGE CASES ******/
/************************/

void run_pwd(void);
int run_cd(t_pipex *pipex_s);
int run_echo(t_pipex *pipex_s);
int special_edge_cases(t_pipex *pipex_s);

/************************/
/****** BUILT INS *******/
/************************/
void	ft_exit(t_pipex *pipex);

/************************/
/***** ENV FUNCTIONS ****/
/************************/

// ft_getenv(char *var_name); TODO
// ft_setenv(char *var_name, char *var_value, int overwrite); TODO
char *ft_getenv(t_main *main_s, char *var_name); //TODO
void ft_setenv(t_main *main_s, char *var_name, char *var_value, int overwrite); //TODO
t_env *new_menv_s(void);
void export_env(t_main *main_s);
void my_print_env(t_main *main_s);

/************************/
/*** SIGNAL HANDLERS ****/
/************************/

// int setup_signal_handlers(int process_type);
int set_sig_handlers(int signal, void (*func_name)(int));
void handle_sigquit(int sig);
void handle_sigint(int sig);

#endif
