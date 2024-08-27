#include "main.h"

void	player_cleanup(t_player *player)
{
	//cleanup ..
	if (player->color == WHITE)
	{
		printf("cleanup WHITE\n");
		player->game->turn = BLACK;
	}
	else
	{
		printf("cleanup BLACKWHITE\n");
		player->game->turn = WHITE;
	}
	pthread_cond_broadcast(&player->game->turn_over);
	pthread_mutex_unlock(&player->game->mutex);
	pthread_exit(NULL);
}

bool	bounds_check(t_move move)
{
	if (move.xo < 0 || move.yo < 0 || move.xn < 0 || move.yn < 0
		|| move.xo >= WIDTH || move.yo >= HEIGHT
		|| move.xn >= WIDTH || move.yn >= HEIGHT)
	{
		return (false);
	}
	return (true);
}

bool	color_check(t_move move, t_game *game)
{
	bool	color_check = (game->board[move.yo][move.xo] % 2)
				!= (game->board[move.yn][move.xn] % 2);
	bool	empty_check = !game->board[move.yn][move.xn];
	return (color_check || empty_check);
}

t_move	handle_in_check(t_player *player)
{
	t_move	move = {0};
	t_move	*moves = dyn_arr_init(sizeof move, 1);
	size_t	move_count = 0;
	//..loop()...
	if (!move_count)
	{
		// set game over condition
		player_cleanup(player);
		__builtin_unreachable();
	}
	else
		return (move);
}

bool	in_check(t_player *player)
{
	//if (...)
	//	return (true);
	return (false);
}

bool	endagers_kings(t_move move, t_player *player)
{
	//if (..)
	//	return (true);
	return (false);
}

// used for line movents for king, queen, bishop and rook
// move.xo/yo for base coordinates
// move.xn/ym for directional coordinates
// step count for maximal steps of xn/yn each
void	add_line_moves(t_player *player, t_move move, t_move **moves, size_t *moves_count, size_t step_count)
{
	int	direct_x = move.xn;
	int	direct_y = move.yn;
	assert(MAX(direct_x, direct_y) <= 1 && MIN(direct_x, direct_y) >= -1);
	move.xn = move.xo;
	move.yn = move.yo;
	for (size_t i = 1; i <= step_count; i++)
	{
		move.xn += direct_x;
		move.yn += direct_y;
		if (bounds_check(move) && color_check(move, player->game)
			&& !endagers_kings(move, player))
		{
			dyn_arr_add_save((void **)moves, &move, (*moves_count)++);
		}
		if (!bounds_check(move) || player->game->board[move.yn][move.xn])
			return ;
	}
}

//assumes aquired mutex for game
t_move	get_rdm_move(t_player *player)
{
	t_move move = {0};
	if (in_check(player))
		return (handle_in_check(player));
	t_move	*moves = NULL;
	size_t	moves_count = 0;

	moves = dyn_arr_init(sizeof(t_move), 1);
	for (uint8_t y = 0; y < HEIGHT; y++)
	{
		for (uint8_t x = 0; x < WIDTH; x++)
		{
			if (player->game->board[y][x] != EMPTY
					&& player->game->board[y][x] % 2 == player->color)
			{
				move.xo = x;
				move.yo = y;
				t_move tmp = move;
				t_uncolored_piece piece = (player->game->board[y][x] - 1) / 2;
				switch ((t_uncolored_piece)piece)
				{
					case (KING):
					{
						const int vecs[8][2] = KING_VECS;
						for (uint8_t i = 0; i < 8; i++)
						{
							move.xn = vecs[i][0];
							move.yn = vecs[i][1];
							if (piece == KING)
								add_line_moves(player, move, &moves, &moves_count, 1);
							else
							{
								for (int j = 1; j <= 7; j++)
									add_line_moves(player, move, &moves, &moves_count, j);
							}
						}
						break ;
					}
					case (QUEEN):
					{
						const int vecs[8][2] = KING_VECS;
						for (uint8_t i = 0; i < 8; i++)
						{
							move.xn = vecs[i][0];
							move.yn = vecs[i][1];
							if (piece == KING)
								add_line_moves(player, move, &moves, &moves_count, 1);
							else
							{
								for (int j = 1; j <= 7; j++)
									add_line_moves(player, move, &moves, &moves_count, j);
							}
						}
						break ;
					}
					case (BISHOP):
					{
						const int vecs[4][2] = BISHOP_VECS;
						for (uint8_t i = 0; i < 4; i++)
						{
							move.xn = vecs[i][0];
							move.yn = vecs[i][1];
							for (int j = 1; j <= 7; j++)
								add_line_moves(player, move, &moves, &moves_count, j);
						}
						break ;
					}
					case (KNIGHT):
					{
						const int vecs[8][2] = KNIGHT_VECS;
						for (uint8_t i = 0; i < 8; i++)
						{
							move.xn = move.xo + vecs[i][0];
							move.yn = move.yo + vecs[i][1];
							if (bounds_check(move) && color_check(move, player->game)
								&& !endagers_kings(move, player))
							{
								dyn_arr_add_save((void **)&moves, &move, moves_count++);
							}
						}
						break ;
					}
					case (ROOK):
					{
						const int vecs[4][2] = ROOK_VECS;
						for (uint8_t i = 0; i < 4; i++)
						{
							move.xn = vecs[i][0];
							move.yn = vecs[i][1];
							for (int j = 1; j <= 7; j++)
								add_line_moves(player, move, &moves, &moves_count, j);
						}
						break ;
					}
					case (PAWN):
					{
						move.xn = move.xo;
						int	direct_y = -1;
						if (player->color == BLACK)
							direct_y = 1;
						move.yn = move.yo + direct_y;
						if (bounds_check(move)
								&& !player->game->board[move.yn][move.xo])
						{
							dyn_arr_add_save((void **)&moves, &move, moves_count++);
						}
						move.xn--;
						if (bounds_check(move)
							&& player->game->board[move.yn][move.xn]
							&& (player->game->board[move.yo][move.xo] % 2)
								!= (player->game->board[move.yn][move.xn] % 2)
						)
						{
							dyn_arr_add_save((void **)&moves, &move, moves_count++);
						}
						move.xn += 2;
						if (bounds_check(move)
							&& player->game->board[move.yn][move.xn]
							&& (player->game->board[move.yo][move.xo] % 2)
								!= (player->game->board[move.yn][move.xn] % 2)
						)
						{
							dyn_arr_add_save((void **)&moves, &move, moves_count++);
						}
						if ((player->color == BLACK && move.yo == 1)
							|| (player->color == WHITE && move.yo == 6))
						{
							move.xn = move.xo;
							move.yn += direct_y;
							if (!player->game->board[move.yn][move.xo])
								dyn_arr_add_save((void **)&moves, &move, moves_count++);
						}
						break ;
					}
					default:
					{
						printf("%d\n", piece);
						ERROR("unknown piece");
					}
					assert(tmp.xo == move.xo && tmp.yo == move.yo);
				}
			}
		}
	}
	printf("possible moves: %lu\n", moves_count);
	bzero(&move, sizeof move);
	if (!moves_count)
	{
		// signal draw
		printf("draw!");
		//return (move);
		player_cleanup(player);
		__builtin_unreachable();
	}
	move = moves[rand() % moves_count];
	dyn_arr_free((void **)&moves);
	return (move);
}

void	execute_move(t_player *player, t_move move)
{
	printf("moving %d/%d(%s) to %d/%d(%s) (x/y)\n", move.xo, move.yo,
			piece_to_str(player->game->board[move.yo][move.xo]),
			move.xn, move.yn,
			piece_to_str(player->game->board[move.yn][move.xn]));
	player->game->board[move.yn][move.xn] =
		player->game->board[move.yo][move.xo];
	player->game->board[move.yo][move.xo] = 0;
}

void	*game_loop(void *player_data)
{
	t_player	*player = (t_player *)player_data;

	if (player->color == BLACK)
		printf("game loop black\n");
	else
		printf("game loop white\n");
	while (1)
	{
		pthread_mutex_lock(&player->game->mutex);
		while (player->color != player->game->turn)
			pthread_cond_wait(&player->game->turn_over, &player->game->mutex);
		if (player->color == BLACK)
			printf("blacks turn\n");
		else
			printf("whites turn\n");
		t_move rdm_move = get_rdm_move(player);
		execute_move(player, rdm_move);
		print_board(player->game);
		if (player->color == WHITE)
			player->game->turn = BLACK;
		else
			player->game->turn = WHITE;
		pthread_cond_broadcast(&player->game->turn_over);
		pthread_mutex_unlock(&player->game->mutex);
	}
	player_cleanup(player);
	return (NULL);
}

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
