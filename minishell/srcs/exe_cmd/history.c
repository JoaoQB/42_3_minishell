/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:51:15 by fandre-b          #+#    #+#             */
/*   Updated: 2024/09/18 17:58:36 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_hist *ft_init_hist_s(void)
{
    t_hist *hist_s;

    hist_s = (t_hist *)safe_malloc(sizeof(t_hist));
    hist_s->idx = 1;
    hist_s->prev = NULL;
    hist_s->next = NULL;
    return(hist_s);
}

void add_to_history(t_main *main_s)
{
    t_hist  *hist_s;
    t_hist  *temp;
    // char    *dup_input;

    hist_s = main_s->history;
    while(hist_s && hist_s->next)
        hist_s = hist_s->next;
    if (hist_s && ft_strcmp(hist_s->usr_input, main_s->user_input) == 0)
        return;
    // dup_input = ft_strdup();
    // if(!dup_input)
    //     perror("safe_malloc systeam history failled");
    // add_history(dup_input);
    // free(dup_input);
    add_history(main_s->user_input);//TODO isto dá leaks...
    if (hist_s && hist_s->idx == MAX_HIST)
        ft_rm_history(&main_s->history);
    temp = ft_init_hist_s();
    temp->prev = hist_s;
    temp->usr_input = main_s->user_input;
    main_s->user_input = NULL;
    if(hist_s)
    {
        temp->idx = hist_s->idx + 1;
        hist_s->next = temp;
    }
    else
        main_s->history = temp;
}

void    get_history(t_hist *hist_s, int index)
{//TODO poder receber numero de cmds para printar
    char *str_token;
    t_hist *curr_hist_s;

    curr_hist_s = hist_s;
    if (index == -1)
    {
        while(curr_hist_s)
        {
            str_token = curr_hist_s->usr_input;
            if (!str_token)
                break;
            printf("%d %s\n", curr_hist_s->idx, str_token);
            curr_hist_s = curr_hist_s->next;
        }
    }
}

void    ft_rm_history(t_hist **hist_s)
{
    t_hist *temp;

    if (!(*hist_s))
        return;
    temp = (*hist_s);
    (*hist_s) = (*hist_s)->next;
    free(temp->usr_input);
    free(temp);
    temp = (*hist_s);
    while (temp)
    {
        temp->idx -= 1;
        temp = temp->next;
    }

}

void free_history(t_hist *hist_s)
{
    t_hist *temp;

    while(hist_s)
    {
        free(hist_s->usr_input);
        temp = hist_s;
        hist_s = hist_s->next;
        free(temp);
    }
    rl_clear_history();
}
