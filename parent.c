#include "main.h"

int	launch_child(char **av, char **env)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		fprintf(stderr, "fork error\n");
		usleep(1000);
		//error
	}
	if (pid == 0)
	{
		execve(CHILD_PATH, av, env);
		exit(errno);
	}
	return (pid);
}

int	main(int ac, char *av[], char *env[])
{
	printf("hello from parent!\n");
	while (1)
	{
		launch_child(av, env);
	}
	return (0);
}
