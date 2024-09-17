#include "../../includes/minishell.h"

char *ft_getenv(t_main *main_s, char *var_name) //TODO
{
	t_env *menv_s;

	menv_s = main_s->env;
	while(menv_s && ft_strcmp(menv_s->var, var_name) != 0)
		menv_s = menv_s->next;
	return(ft_strdup(menv_s->var_value));
}

void ft_setenv(t_main *main_s, char *var_name, char *var_value, int overwrite) //TODO
{//preciso de cabeça para dar handle destes frees melhor
	t_env *menv_s;

	menv_s = main_s->env;
	while(menv_s->next && ft_strcmp(menv_s->var, var_name) != 0)
		menv_s = menv_s->next;
	if (!menv_s->next)
	{
		menv_s->next = new_menv_s();
		menv_s->next->var = ft_strdup(var_name);
		menv_s->next->var_value = var_value;
		menv_s->next->index = menv_s->index + 1;
	}
	else if(overwrite == 1)
	{
		free(menv_s->var_value);
		menv_s->var_value = var_value;
	}
	else
		free(var_value);
}

t_env *new_menv_s(void)
{//deprecated
	t_env *menv_s;

	menv_s = (t_env *) malloc(sizeof(t_env));
	if (!menv_s)
		perror("malloc menv failed"); //TODO trigger close of everything.
	menv_s->var = NULL;
	menv_s->var_value = NULL;
	menv_s->value = NULL;
	menv_s->index = 1;
	menv_s->next = NULL;
	return (menv_s);
}

// void export_env(t_main *main_s)
// {//deprecated
// 	char	**envp;
// 	int		i;
// 	t_env	*menv_s;

// 	if (main_s->env)
// 		return;
// 	main_s->env = new_menv_s();
// 	menv_s = main_s->env;
// 	envp = main_s->menv;
//     while (*envp != NULL)
//     {
// 		i = 0;
// 		while ((*envp)[i] && (*envp)[i] != '=')
// 			i++;
// 		menv_s->var = ft_strnjoin(NULL, *envp, i++);
// 		menv_s->var_value = ft_strnjoin(NULL, *envp + i, -1);
// 		if(*(++envp) == NULL)
// 			break;
// 		menv_s->next = new_menv_s();
//     	menv_s->next->index = menv_s->next->index + 1;
// 		menv_s = menv_s->next;
//     }
// }

void my_print_env(t_main *main_s)
{
	t_env *menv_s;

	menv_s = main_s->env;
	while(menv_s)
	{
		printf("%s=%s\n", menv_s->var, menv_s->var_value);
		menv_s = menv_s->next;
	}
}
