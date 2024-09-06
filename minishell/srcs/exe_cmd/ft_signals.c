#include "../../includes/minishell.h"

void handle_sigint(int sig) 
{
    (void)sig;
    // printf("ctl-c activated in child\n");
    g_signal = 1;
    // rl_on_new_line();
    // rl_replace_line("X", 1);
    rl_point = 0;
    rl_redisplay();
    // rl_done = 1;
    // exit(0);
}


void handle_sigquit(int sig) 
{
    (void)sig;
    g_signal = 2;
    abort();
}

int set_sig_handlers(int signal, void (*func_name)(int)) 
{
    struct sigaction sa;

    sa.sa_handler = func_name; //handle_sigint;
    sa.sa_flags = 0;  //teels the karnel no flags no to restart system calls
    sigemptyset(&sa.sa_mask); //block all signals while handling SIGINT
    if (sigaction(signal, &sa, NULL) == -1)
    {
        perror("sigaction ctl-c");
        return(1);
    }
    return (0);
}

// int setup_signal_handlers(int process_type) 
// {
//     struct sigaction sa_int;
//     struct sigaction sa_quit;

//     if (process_type == 0)
//         sa_int.sa_handler = handle_sigint; //handle_sigint;
//     if (process_type == 1)
//         sa_int.sa_handler = handle_sigint_child; //handle_sigint;
//     sa_int.sa_flags = 0;  //teels the karnel no flags no to restart system calls
//     sigemptyset(&sa_int.sa_mask); //block all signals while handling SIGINT
//     if (sigaction(SIGINT, &sa_int, NULL) == -1)
//         return(perror("sigaction ctl-c"), 1);
//     sa_quit.sa_handler = handle_sigquit;
//     sa_quit.sa_flags = 0;
//     sigemptyset(&sa_quit.sa_mask);
//     if (sigaction(SIGQUIT, &sa_quit, NULL) == -1) 
//         return(perror("sigaction ctl-\\"), 1);
//     return (0);
// }

