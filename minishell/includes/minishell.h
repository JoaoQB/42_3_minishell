/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:17:04 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/08/13 18:35:10 by jqueijo-         ###   ########.fr       */
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
typedef struct s_env	t_env;

/* Other Possible sub-tokens: CMD_BUILTIN/EXTERNAL; PATH_ABSOLUTE/RELATIVE*/
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
	t_token	*next;
}	t_token;

typedef struct s_main
{
	t_env	*env;
	t_token	*tokens;
	char	*user_input;
	char	*input_trim;
	char	*input_reorg;
	char	***cmd;
	int		size;
	int		exe_fd[2];
	int		*fd_pipeline[2];
	int		*pid_pipeline;
	bool	silence_info;
	t_main	*next;
}	t_main;

// typedef struct s_all_mains
// {
// 	t_main	*first;
// 	char	**envp;
// 	bool	silence_info;
// }	t_all_mains;

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
void	print_env(t_env *env);

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

// /* split_into_words_quotes.c */
// char	**split_into_words_quotes(char const *s);

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

#endif
