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


void handle_sigquit(int sig) 
{
    (void)sig;
    g_signal = 2;
    printf("this is an forced core dump\n");
    abort(); //TODO is this forbidden? if so i have to pass signals

}

int set_sig_handlers(int signal, void (*func_name)(int)) 
{
    struct sigaction sa;

    sa.sa_handler = func_name; //handle_sigint;
    sa.sa_flags = 0;  //teels the karnel no flags no to restart system calls
    sigemptyset(&sa.sa_mask); //block all signals while handling SIGINT
    if (sigaction(signal, &sa, NULL) == -1)
    {
        perror("sigaction failed");
        return(1);
    }
    return (0);
}
