#include "../../includes/minishell.h"

// void handle_sigint(int signum, siginfo_t *info, void *context) {
//     if (info->si_code == SI_QUEUE) {
//         t_ctx *ctx = (t_ctx *)info->si_value.sival_ptr;
//         printf("Received signal %d with context: some_field=%d, another_field=%d\n",
//                signum, ctx->some_field, ctx->another_field);
//         // Perform cleanup
//         free(ctx);
//     } else {
//         printf("Received signal %d\n", signum);
//     }
//     exit(1);
// }

// void handle_sigint(int sig)
void handle_sigint(int sig, siginfo_t *info, void *main_s) 
{
   (void)sig;
   (void)info;
   (void)main_s;
    g_signal = 1;
    printf("\n");
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
    // rl_point = 0;
    // rl_done = 1;
    // exit(0);
}

// void handle_sigquit(int sig)
void handle_sigquit(int sig, siginfo_t *info, void *main_s) 
{
   (void)sig;
   (void)info;
   (void)main_s;
    g_signal = 2;
    printf("this is an forced core dump\n");
    abort(); //TODO exit with abort value or free and core dump
}

// int set_sig_handlers(int signal, void(*func_name)(int)) 
// {
//     struct sigaction sa;

//     sa.sa_handler = func_name; //handle_sigint;
//     sa.sa_flags = 0;  //teels the karnel no flags no to restart system calls
//     sigemptyset(&sa.sa_mask); //block all signals while handling SIGINT
//     if (sigaction(signal, &sa, NULL) == -1)
//     {
//         perror("sigaction failed");
//         return (1);
//     }
//     return (0);
// }

// int set_sig_handlers(int signal, void (*handler), t_cxt *context)
int set_sig_handlers(int signal, void (*handler)(int, siginfo_t *, void *), void *main_s)
{
    struct sigaction sa;

    (void)(t_main *)main_s;
    sa.sa_sigaction = handler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask); //block all signals while handling SIGINT
    if (sigaction(signal, &sa, NULL) == -1)
    {
        perror("sigaction failed");
        return (1);
    }
    return (0);
}