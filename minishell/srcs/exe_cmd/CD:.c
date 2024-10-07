	CD:
return (printf("%s: %s\n", cmd[0], "too many arguments"), 1); //TODO Handle error s
return (free(new_dir), printf("%s: %s: %s\n", cmd[0], cmd[1], strerror(errno)), 1);//TODO Handle error s
	
	cd: dir: error print

	is_dir:
printf("%s: filename argument required\n", path); //TODO Handle error s
printf("usage: %s filename [arguments]\n", path);
status = 2???
	error 126 when dir
printf("%s: %s\n", path, strerror(EISDIR)); //TODO Handle error s
pipex_s->status = 126;
	error 127 cmd not found
ft_putstr_fd(pipex_s->cmd[0], 2);
ft_putstr_fd(": command not found\n", 2);
pipex_s->status = 127;

	IOfiles:
return(printf("%s: %s\n", tokens_s->next->value, strerror(errno)), errno); //TODO Handle error s
its stucking my files checki9ng *heredoc()

also handle pipes, opens closes
exit status out of bounds
signals

// What else am i missing