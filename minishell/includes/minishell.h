/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:17:04 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/22 15:54:52 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <stdlib.h>
# include <stddef.h>
# include <stdbool.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <limits.h>
# include <stdarg.h>

# define MAX_HIST 30
# ifndef BUFFERSIZE
#  define BUFFERSIZE 100
# endif

typedef struct s_env			t_env;
typedef struct s_token			t_token;
typedef struct s_pipex			t_pipex;
typedef struct s_main			t_main;
typedef struct s_hist			t_hist;

typedef enum e_sigmode
{
	SIGMAIN,
	SIGCMD,
	SIGHDC,
	SIGHD,
}	t_sigmode;

typedef enum e_neg_operator
{
	NEG_PIPE = -1,
	NEG_LESS = -2,
	NEG_GREATER = -3,
	NEG_VAR = -4
}	t_neg_operator;

typedef enum e_token_type
{
	RED_IN = 1,
	RED_OUT,
	HERE_DOC,
	RED_OUT_APP,
	PIPE,
	QUOTE,
	WORD,
	CMD,
	ARG,
	DELIM,
	DELIM_QUOTE,
	PATH,
	CONC,
	VAR,
	EMPTY
}	t_token_type;

typedef struct s_env
{
	char	*value;
	char	*var;
	char	*var_value;
	t_env	*next;
}	t_env;

typedef struct s_token
{
	int		type;
	char	*value;
	int		index;
	t_token	*prev;
	t_token	*next;
}	t_token;

typedef struct s_pipex
{
	pid_t	pid;
	int		status;
	char	*path;
	char	**cmd;
	int		pipe_fd[2];
	t_token	*token;
	t_pipex	*prev;
	t_pipex	*next;
	t_main	*main_s;
}	t_pipex;

typedef struct s_hist
{
	int				idx;
	char			*usr_input;
	struct s_hist	*next;
	struct s_hist	*prev;
}	t_hist;

typedef struct s_main
{
	char	**menv;
	char	*user_input;
	char	*input_trim;
	char	*input_reorg;
	bool	silence_info;
	int		status;
	int		temp_fd[2];
	int		err_fd[2];
	t_env	*env;
	t_env	*export;
	t_token	*tokens;
	t_pipex	*pipex;
	t_hist	*history;
}	t_main;

/************************/
/********* INIT *********/
/************************/

/* init.c */
void		init_main(char **envp);

/* cleanup.c */
void		free_main_input(void);
void		cleanup_main(void);
void		free_double_array(char **array);
void		ft_free(char **str);

/* cleanup_struct.c */
void		free_pipex_s(void);
void		free_tokens(t_token **first);
void		free_env(t_env **first);

/* env_init.c */
t_env		*get_env(char **envp);
void		append_env_back(t_env **first, t_env *new_env);
char		*env_extract_var(t_env	*new_env);
char		*env_extract_var_value(char *str);

/* export_init.c */
t_env		*sort_env(void);
t_env		*ft_export_new(char *str);
char		*env_check_name(char *str);
void		export_set_ordered(t_env **first, t_env *new);

/************************/
/********* UTILS ********/
/************************/

/* len_utils.c */
int			count_arrays(char **array);
int			ft_strlen(const char *str);
int			count_words(const char *str);
char		*ft_strnjoin(char *old_str, char *str_add, int size);

/* str_utils.c */
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ft_strcmp(const char *s1, const char *s2);
char		*ft_strdup(const char *s);
size_t		ft_strlcpy(char *dst, const char *src, size_t size);
char		*ft_strcat(char *dest, const char *src);

/* char_extract_utils.c */
char		*extract_before_i(char *str, int i);
char		*extract_from_i(char *str, int i);
int			iterate_quotes(char *str);

/* is_x.c */
int			ft_isspace(int c);
int			ft_isoperator(int c);
int			ft_isquotes(int c);
int			ft_ischar(int c);
int			ft_isdigit(int c);

/* find_x_utils.c */
bool		find_quotes(char *str);
bool		find_equal(char *str);
bool		find_plus(char *str);

/* print_utils.c */
void		print_tokens(t_token *tokens);
void		print_env(t_env *env);
void		print_menv(char **menv);

/* token_utils.c */
void		reassign_tokens(t_token *first);
void		reindex_tokens(t_token *first);
void		token_append_before(t_token **first, t_token *target, t_token *new);
void		token_append_after(t_token *target, t_token *new);
void		token_extract_before(t_token **first, t_token *current, int i);

/* token_utils2.c */
void		token_extract_after(t_token *current, int i);
void		clean_tokens(t_token **first);
void		delete_token(t_token **first, t_token *current);
bool		token_is_redirect(t_token *current);

/* var_utils.c */
int			ft_isvar1stchar(int c);
int			ft_isvarchar(int c);
char		*var_extract_before(t_token **first, t_token *current, int i);
char		*var_extract_after(t_token *current, int i);
char		*extract_inside_quotes(char *str);

/* fd_utils.c */
void		ft_putendl_fd(char *s, int fd);
void		ft_putstr_fd(char *s, int fd);
void		ft_putchar_fd(char c, int fd);
void		print_err(char *format, ...);

/* to_utils.c */
int			ft_atoi(const char *nptr);
long long	ft_atoi_lng(const char *nptr);
char		*ft_itoa(int n);

/************************/
/****** PROCESSING ******/
/************************/

/* input_process.c */
void		input_process(char *user_input);
char		*input_and_tokens_reorg(void);
char		*concat_tokens_to_char(t_token *first);

/* input_trim_and_tokenize.c */
char		*input_trim_and_tokenize(char *user_input);

/* split_into_words.c */
char		**split_into_words(char const *s);

/* input_tokenize.c */
t_token		*input_tokenize(char **words);
int			token_assign(char *str);

/* token_separate_operator.c */
t_token		*token_separate_operator(void);

/* tilde_replace.c */
void		tilde_replace(t_token *first);

/* quotes_hide_operators.c */
void		quotes_hide_operators(t_token *first);
void		hide_operators(char *str);
void		quotes_revert_operators(t_token *first);
void		revert_operators(char *str);

/* token_split_words.c */
void		token_split_words(t_token **first);

/* quotes_remover.c */
void		quotes_remover(t_token *first);

/* tokenize_smarter.c */
void		tokenize_smarter(t_token *first);

/* var_swap.c */
void		var_swap(t_token **first);

/* var_conc.c */
t_token		*var_conc(t_token *target, t_token **first);

/* var_remove_quotes.c */
t_token		*var_remove_quotes(t_token **first, t_token *current, int i);
t_token		*var_conc_quotes(t_token **first, t_token *current);
char		*var_extract_after(t_token *current, int i);

/* var_replace.c */
t_token		*var_replace(t_token *current, int i);
char		*var_replace_qstnmrk(void);
char		*var_check_env(t_env *env, char *str);

/* here_doc expand*/
char		*heredoc_expand(t_token *delim, char *str);

/* token_check_before_pipex.c */
void		token_check_before_pipex(t_token *first);

/************************/
/******* PARSING ********/
/************************/

/* first_pars.c */
bool		first_pars(t_token *first);
bool		parse_quotes(char *input);

/* error_msg.c */
void		syntax_error_msg(void);
void		syntax_error_pipe(void);

/************************/
/******* HISTORY ********/
/************************/

t_hist		*ft_init_hist_s(void);
void		add_to_history(void);
void		get_history(t_hist *hist_s, int index);
void		rt_one_history(void);
void		free_history(void);

/************************/
/***** PROCESS PIPES ****/
/************************/

//pipe init/processing pipex_struct.c
t_token		*find_next_pipe(t_token *tokens_s);
void		ft_n_update_fds(t_pipex *pipex_s);
void		ft_n_update_cmds(t_pipex *pipex_s);
t_pipex		*ft_init_pipex_s(void);
t_pipex		*add_back_pipex_s(void);

/************************/
/***** HANDLE FILES *****/
/************************/

int			ft_close(int *fd);
void		close_all_fd(t_pipex *pipex_s);
int			ft_process_redirect(t_token *tk_s, int *fd);
void		ft_update_fds(t_token *tk_s, t_pipex *pipex_s);

/************************/
/****** HERE DOC ********/
/************************/

void		input_error(t_token *token);
void		heredoc_child(int *piper, t_token *tokens_s);
int			read_heredoc(t_token *tokens_s);
void		ft_update_heredoc(t_token *tk_s, t_pipex *pipex_s);

/************************/
/****** EDGE CASES ******/
/************************/

int			special_edge_cases(t_pipex *pipex_s);
int			edge_cases(t_pipex *pipex_s);

/************************/
/******** PIPEX *********/
/************************/

//pipex_main.c
int			check_for_pipeline(void);
void		exe_cmd_child(t_pipex *pipex_s, char **envp);
void		new_process_tokens(void);
int			get_final_status(void);
int			ft_shell_pipex(void);

//pipex utils //exe_cmd.c
void		read_error_fd(void);
void		create_error_fd(void);
void		process_child_pid(t_pipex *curr_pipex_s);
int			process_child_pipes(t_pipex *pipex_s);

/************************/
/***** FIND EXE PATH ****/
/************************/

//path
int			is_directory(char *path);
int			check_cmd_path(t_pipex *pipex_s);
char		*find_in_path(char *paths, t_pipex *pipex_s);
char		*get_cmd_path(t_pipex *pipex_s);
int			ft_n_update_path(t_pipex *pipex_s);

//path utils
int			file_access(char *file_path);
int			ft_is_path(char *str);

/************************/
/****** BUILT INS *******/
/************************/

/* run_built_ins.c */
char		*run_pwd(bool print);
int			run_cd(t_pipex *pipex_s);
void		run_echo(t_pipex *pipex_s);
void		run_unset(t_pipex *pipex_s);
int			run_export(t_pipex *pipex_s);

/* ft_exit.c */
void		ft_exit_pid(t_pipex *pipex);
void		ft_exit(int status);
void		ft_exit_builtins(t_pipex *pipex);

/* ft_unset.c */
void		export_unset(char *export_var);
void		env_unset(char *env_var);

/* ft_export.c */
void		print_export(t_env *first);
void		export_check(t_env *new);

/************************/
/***** ENV FUNCTIONS ****/
/************************/

void		my_print_env(void);
void		ft_setenv(char *var_nm, char *var_vl, int ovwr);
char		*ft_getenv(char *var_name);
char		*env_get_value(char *var_name, char *var_value);
char		**get_array_env(void);

/************************/
/*** SIGNAL HANDLERS ****/
/************************/

//signals functions

void		handle_sigchild(int sig);
void		sigint_handler_hd(int sig);
void		sigint_handler_hdc(int sig);
void		sigquit_handler_cmd(int sig);
void		sigint_handler_cmd(int sig);

//signals setting
void		sigint_handler_main(int sig);
int			set_sig_handlers(int signal, void (*func_name)(int));
void		set_signals(int sigmode);

/************************/
/***** OVERALL UTILS ****/
/************************/

void		critical_error(char *err_print);
void		*safe_malloc(size_t size);
t_main		*minishell(void);

#endif
