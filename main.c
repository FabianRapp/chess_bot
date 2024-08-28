#include "main.h"

void	cleanup(t_manager *manager)
{
	printf("placeholder manager cleanup\n");
}

int	main(void)
{
	printf("hello from parent!\n");
	t_manager	manager;

	if (!init(&manager))
		ERROR("manager");
	// for now good enough to start and join all at the same time
	//while (1)
	{
		for (int i = 0; i < GAME_COUNT; i++)
			launch_game(&manager, i);
		for (int i = 0; i < GAME_COUNT; i++)
		{
			pthread_join(manager.black_players[i].thread, NULL);
			pthread_join(manager.white_players[i].thread, NULL);
		}
	}
	cleanup(&manager);
	return (0);
}
