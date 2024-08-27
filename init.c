#include "main.h"

int	launch_game(t_manager *manager, size_t game_index)
{
	reset_game(manager->games + game_index);
	manager->black_players[game_index].color = BLACK;
	manager->white_players[game_index].color = WHITE;
	pthread_create(&manager->black_players[game_index].thread, NULL, game_loop,
			manager->black_players + game_index);
	pthread_create(&manager->white_players[game_index].thread, NULL, game_loop,
			manager->white_players + game_index);
	if (errno)
		return (0);
	return (1);
}

int	init(t_manager *manager)
{
	errno = 0;
	srand(time(NULL));
	bzero(manager, sizeof *manager);
	manager->game_count = GAME_COUNT;
	manager->games = calloc(manager->game_count, sizeof(t_game));
	manager->white_players = calloc(manager->game_count, sizeof(t_player));
	manager->black_players = calloc(manager->game_count, sizeof(t_player));
	if (errno)
		return (0);
	for (size_t i = 0; i < GAME_COUNT; i++)
	{
		manager->black_players[i].game = manager->games + i;
		manager->white_players[i].game = manager->games + i;
		pthread_mutex_init(&manager->games[i].mutex, NULL);
		pthread_cond_init(&manager->games[i].turn_over, NULL);
	}
	return (1);
}
