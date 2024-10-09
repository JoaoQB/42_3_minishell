#include "../../includes/minishell.h"

void handle_sigint(int sig) 
{
   (void)sig;
    g_signal = 1;
    printf("\n");
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
    // rl_point = 0;
    // rl_done = 1;
    // exit(0);
}

void handle_sigchild(int sig) 
{
    t_pipex *pipex_s;
    pid_t   pid;
    int     status;

    (void)sig;
    while(1)
    {
        pid = waitpid(-1, &status, WNOHANG);
        if(pid == 0 || pid == -1)
            break;
        pipex_s = minishell()->pipex;
        while (pipex_s && pipex_s->pid != pid)
            pipex_s = pipex_s->next;
        if (pipex_s)
        {
            process_child_pid(pipex_s);
            //free_pipex_node(pipex_s); // TODO reddy to test
        }
    }
}

void handle_sigquit(int sig) 
{
   (void)sig;
    g_signal = 2;
    printf("this is an forced core dump\n");
    // abort(); //TODO exit with abort value or free and core dump
    free_main_input();
	cleanup_main();
	exit(1); // General error
}

int set_sig_handlers(int signal, void(*func_name)(int)) 
{
    struct sigaction sa;

    sa.sa_handler = func_name; //handle_sigint;
    sa.sa_flags = 0;  //teels the karnel no flags no to restart system calls
    sigemptyset(&sa.sa_mask); //block all signals while handling SIGINT
    if (sigaction(signal, &sa, NULL) == -1)
    {
        perror("sigaction failed");
        return (1);
    }
    return (0);
}
