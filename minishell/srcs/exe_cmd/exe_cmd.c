/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juka <juka@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:51:15 by juka              #+#    #+#             */
/*   Updated: 2024/08/21 18:10:04 by juka             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	exe_cmd(t_main *main_s)
{
	ft_process_cmd(main_s);
	print_struct(main_s);
	//ft_exe_pipe(main_s);
	//recieve signal when i do exit, so i can properly free it and pass responsability
	return (0);
}

int		ft_process_cmd(t_main *main_s)
{
	t_token	*tokens_s;
	t_pipex	*pipex_s;

	tokens_s = main_s->tokens;
	create_pipeline(main_s);
	pipex_s = main_s->pipex;
	while (tokens_s->next != NULL)
		tokens_s = tokens_s->next;
	while (pipex_s->next != NULL)
		pipex_s = pipex_s->next;
	while(tokens_s != NULL)
	{
		if (tokens_s->prev == NULL || tokens_s->prev->type == PIPE)
		{
			create_array_cmd(tokens_s, pipex_s);
			pipex_s = pipex_s->prev;
		}
		tokens_s = tokens_s->prev;
	}
	return (0);
}

int	create_array_cmd(t_token *tokens_s, t_pipex *pipex_s)
{
	int count;
	t_token *curr_token;

	curr_token = tokens_s;
	count = 0;
	while (curr_token && curr_token->type != PIPE)
	{
		if (curr_token->type == CMD || curr_token->type == ARG)
			count += 1;
		curr_token = curr_token->next;
	}
	pipex_s->cmd = (char **) malloc (sizeof (char *) * (count + 1));
	if (!pipex_s->cmd)
		return (perror("Malloc commads array"), errno);
	extract_pipe_cmds(tokens_s, pipex_s);
	extract_pipe_fds(tokens_s, pipex_s);
	return (0);
}
int	extract_pipe_cmds(t_token *tokens_s, t_pipex *pipex_s)
{	
	int count;

	count = 0;
	while (tokens_s && tokens_s->type != PIPE) //funcao para este
	{
		if (tokens_s->type == CMD || tokens_s->type == ARG)
		{
			pipex_s->cmd[count++] = ft_strnjoin(NULL, tokens_s->value, -1);
			pipex_s->cmd[count] = NULL;
			if (!pipex_s->cmd[count - 1])
				return (perror("malloc fail"), errno);
		}
		tokens_s = tokens_s->next;
	}
	return (0);
}

int	extract_pipe_fds(t_token *tokens_s, t_pipex *pipex_s)
{
	int		*io_fd;

	io_fd = pipex_s->pipe_fd;
	while (tokens_s && tokens_s->type != PIPE)
	{
		if ((tokens_s->type == RED_IN || tokens_s->type == HERE_DOC) && io_fd[0] > 2)
			close (io_fd[0]);
		if ((tokens_s->type == RED_OUT || tokens_s->type == RED_OUT_APP) && io_fd[1] > 2)
			close (io_fd[1]);
		if(tokens_s->type == HERE_DOC)
			io_fd[0] = read_heredoc(tokens_s);
		else if (tokens_s->type == RED_IN)
			io_fd[0] = open(tokens_s->next->value, O_RDONLY, 0666);
		else if (tokens_s->type == RED_OUT)
			io_fd[1] = open(tokens_s->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		else if (tokens_s->type == RED_OUT_APP)
			io_fd[1] = open(tokens_s->next->value, O_WRONLY | O_CREAT | O_APPEND, 0666);
		if (io_fd[0] ==-1 || io_fd[1] == -1)
			return(printf("%s: No such file or directory\n", tokens_s->next->value), errno); //TODO return error
		tokens_s = tokens_s->next;
	}
	return (0);
}

int create_pipeline(t_main *main_s)
{
	t_pipex *pipex_s;
	t_token *tokens_s;
	int	piper[2];

	main_s->pipex = new_node();
	tokens_s = main_s->tokens;
	pipex_s = main_s->pipex;
	pipex_s->pipe_fd[0] = STDIN_FILENO;
	pipex_s->pipe_fd[1] = STDOUT_FILENO;
	while (tokens_s != NULL)
	{
		if(tokens_s->type == PIPE)
		{
			pipex_s->next = new_node();
			pipex_s->next->prev = pipex_s;
			if (pipe(piper) == -1)
				return(perror ("pipe"), errno);
			pipex_s->next->pipe_fd[1] = pipex_s->pipe_fd[1];
			pipex_s->pipe_fd[1] = piper[0];
			pipex_s->next->pipe_fd[0] = piper[1];
			pipex_s = pipex_s->next;
		}
		tokens_s = tokens_s->next;
	}
	return (0);
}

t_pipex *new_node(void)
{
    t_pipex *pipex_s;

    pipex_s = (t_pipex *)malloc(sizeof(t_pipex));
    if (!pipex_s)
        return (NULL);
    pipex_s->pid = 0;
    pipex_s->status = 0;
	pipex_s->cmd = NULL;
    pipex_s->pipe_fd[0] = -2;
    pipex_s->pipe_fd[1] = -2;
	pipex_s->prev = NULL;
    pipex_s->next = NULL;
    return pipex_s;
}

int read_heredoc(t_token *tokens_s)
{
	if (!tokens_s->next || tokens_s->next->type != DELIM || tokens_s->value == NULL)
		return (perror("Delimiter not found or invalid"), -1);
	printf("to read until: %s\n", tokens_s->value);
	printf("here_doc still not working: \n");
	return (-1);
}

// free_exe_cmd(main_s) //TODO
// {

// }

void	print_struct(t_main *main_s)
{
	t_pipex *pipex_s;
	int	i;
	int j;

	j = 0;
	pipex_s = main_s->pipex;
	printf("\n\n --> Printing pipe_struct an cmds<--\n\n");
	while(pipex_s)
	{
		printf("   {pipe %d}\n", j++);
		printf("pid %d with status %d\n", pipex_s->pid, pipex_s->status);
		printf("inout_fd are: %d / %d\n", pipex_s -> pipe_fd[0], pipex_s -> pipe_fd[1]);
		printf(" -input name: %s\n", get_file_name_from_fd(pipex_s -> pipe_fd[0]));
		printf(" -output name: %s\n", get_file_name_from_fd(pipex_s -> pipe_fd[1]));
		printf("will execute:\n  --> ");
		i = -1;
		while (pipex_s->cmd[++i])
			printf("%s ", pipex_s->cmd[i]);
		if (pipex_s->cmd[i] == NULL)
			printf("%s", pipex_s->cmd[i]);
		printf("\n\n");
		pipex_s = pipex_s->next;
	}
	return ;
}

char	*ft_strnjoin(char *old_str, char *str_add, int size)
{
	int		len;
	int		i;
	char	*new_str;

	if (!str_add[0])
		return (old_str);
	len = 0;
	while (old_str && old_str[len])
		len++;
	i = 0;
	while (str_add && str_add[i])
		i++;
	if (i < size || size == -1)
		size = i;
	new_str = (char *) malloc (size + len + 1);
	if (!new_str)
		return (NULL);
	i = -1;
	while (old_str && ++i < len)
		new_str[i] = old_str[i];
	i = -1;
	while (str_add && ++i < size)
		new_str[len + i] = str_add[i];
	new_str[len + i] = '\0';
	return (free (old_str), new_str);
}

char* get_file_name_from_fd(int fd) {
    static char filename[1024];
    char path[64];

    snprintf(path, sizeof(path), "/proc/self/fd/%d", fd);
    ssize_t len = readlink(path, filename, sizeof(filename) - 1);
    if (len != -1) {
        filename[len] = '\0';
    } else {
        strcpy(filename, "unknown");
    }
    return filename;
}

