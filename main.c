#include "main.h"

void	cleanup(t_manager *manager)
{
	printf("placeholder manager cleanup\n");
}

int	main(void)
{
	printf("hello from parent!\n");
	t_manager	manager;

	size_t	move_count[GAME_COUNT];//array with the count of moves of all games
	t_move	*moves[GAME_COUNT];//2d array with all moves in all games, each array is 1 game
	int		*move_scores[GAME_COUNT];// 2d array with each moves score
									 // (moves_score[GAME][i] -> if i % 2 its a white turn otherwise black)
	for (int i = 0; i < GAME_COUNT; i++)
	{
		moves[i] = dyn_arr_init(1, sizeof (t_move));
		move_scores[i] = dyn_arr_init(1, sizeof (size_t));
	}


	if (!init(&manager))
		ERROR("manager");
	// for now good enough to start and join all at the same time
	for (int i = 0; i < GAME_COUNT; i++)
		launch_game(&manager, i);
	size_t	ongoing_games = GAME_COUNT;
	while (ongoing_games)
	{
		ongoing_games = 0;
		for (size_t i = 0; i < GAME_COUNT; i++)
		{
			t_game	*game = manager.games + i;

			pthread_mutex_lock(&game->mutex_eval);
			if (game->state == DONE)
			{
				pthread_mutex_unlock(&game->mutex_eval);
				continue ;
			}
			ongoing_games++;
			if (game->eval_turn < game->generate_turn)
			{
				move_count[i] = ++game->generate_turn;
				t_move	move = game->move;
				print_move(move, game);
				// score for captures
				// TODO: currently broken after moving execution to monitoring:
				// agents have outdated game state when doing their move
				// instead of fixing this rework threading engine
				int score = execute_move(game, true);
				if (game->state != ONGOING)
				{
					switch (game->state)
					{
						case (NOT_STARTED):
						case (ONGOING):
						case (DONE): ASSUME(0);
						case (BLACK_WON):
						{
							break ;
						}
						case (WHITE_WON):
						{
							break ;
						}
						case (TIE):
						{
							break ;
						}
					}
					game->state = DONE;
				}
				pthread_mutex_unlock(&game->mutex_eval);
				dyn_arr_add_save((void **)&move_scores[i], &score, move_count[i] - 1);
				dyn_arr_add_save((void **)&moves[i], &move, move_count[i] - 1);
				printf("turn %lu scored %d\n", move_count[i], move_scores[i][move_count[i] - 1]);
			}
			else
			{
				pthread_mutex_unlock(&game->mutex_eval);
				usleep(1000);
			}
		}
	}
	for (int i = 0; i < GAME_COUNT; i++)
	{
		pthread_join(manager.black_players[i].thread, NULL);
		pthread_join(manager.white_players[i].thread, NULL);
	}
	cleanup(&manager);
	return (0);
}
