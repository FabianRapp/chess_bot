#include "main.h"

void	cleanup(t_manager *manager)
{
	printf("placeholder manager cleanup\n");
}

double	move_progress_score(double score, t_game_state result, double progress, t_color player)
{
	switch (result)
	{
		case (BLACK_WON):
		{
			if (player == BLACK)
			{
					score += progress * score;
			}
			else
			{
			}
			break ;
		}
		case (WHITE_WON):
		{
			if (player == WHITE)
			{
				if (score > 0)
				{
					score += progress * score;
				}
			}
			else
			{
			}
			break ;
		}
		case (TIE):
		{
			if (score == 0)
			{
				score -= progress * 0.001;
			}
			break;
		}
		default: ASSUME(0);
	}
	return (score);
}

void	get_scores(t_game *game, double *score_b, double *score_w)
{
	*score_b = 0;
	*score_w = 0;
	t_game_state result = game->state;

	ASSUME(result == TIE || result == BLACK_WON || result == WHITE_WON);
	double	progress;
	for (int i = 0; i < game->generate_turn; i++)
	{
		progress = (double)i / game->generate_turn;
		if (i % 2)
		{
			*score_b += move_progress_score(game->moves[i].score, result, progress, BLACK);;
		}
		else
		{
			*score_w += move_progress_score(game->moves[i].score, result, progress, WHITE);;
		}
	}
}


int	main(void)
{
	printf("hello from parent!\n");
	t_manager	manager;

	size_t	move_count[GAME_COUNT];//array with the count of moves of all games
	t_move	*moves[GAME_COUNT];//2d array with all moves in all games, each array is 1 game
									 // (moves_score[GAME][i] -> if i % 2 its a white turn otherwise black)
	t_game_state	results[GAME_COUNT];
	for (int i = 0; i < GAME_COUNT; i++)
	{
		moves[i] = dyn_arr_init(1, sizeof (t_move));
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
				move_count[i] = ++game->eval_turn;
				// score for captures
				// TODO: currently broken after moving execution to monitoring:
				// agents have outdated game state when doing their move
				// instead of fixing this rework threading engine
				if (game->state != ONGOING)
				{
					switch (game->state)
					{
						case (NOT_STARTED):
						case (ONGOING):
						case (DONE): ASSUME(0);
						case (BLACK_WON):
						{
							printf("BLACK won\n");
							break ;
						}
						case (WHITE_WON):
						{
							printf("WHITE won\n");
							break ;
						}
						case (TIE):
						{
							printf("TIE\n");
							break ;
						}
					}
					results[i] = game->state;
					game->state = DONE;
				}
				pthread_mutex_unlock(&game->mutex_eval);
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
		t_game	*game = manager.games + i;
		double	black_score;
		double	white_score;
		pthread_join(manager.black_players[i].thread, NULL);
		pthread_join(manager.white_players[i].thread, NULL);
		for (int j = 0; j < game->generate_turn; j++)
		{
			if (game->moves[j].score)
				printf("score: %lf\n", game->moves[j].score);
		}
		game->state = results[i];
		get_scores(game, &black_score, &white_score);
		printf("black_score: %lf\nwhite_score: %lf\n", black_score, white_score);
		store_neural_net(&manager.black_players[i].neural_net, BLACK);
		store_neural_net(&manager.white_players[i].neural_net, WHITE);
		print_board(game);
		game->state = DONE;
		
	}
	cleanup(&manager);
	return (0);
}
