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
		printf("cleanup BLACK\n");
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
	const t_color	origin_color = piece_color(game->board[move.yo][move.xo]);
	const t_piece	target_piece = game->board[move.yn][move.xn];
	return (target_piece == EMPTY || origin_color != piece_color(target_piece));
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

//todo: en passnt
//todo: refactor
bool	in_check(t_player *player)
{
	t_color	color = player->color;
	t_game	*game = player->game;
	t_position	king_pos = game->positions[color][0];

	ASSUME(king_pos.type == KING_W || king_pos.type == KING_B);
	int k = 0;
	for (int x = king_pos.x - 1; x >= 0; x--)
	{
		t_piece	piece_in_sight = game->board[king_pos.y][x];
		if (piece_in_sight)
		{
			t_uncolored_piece uncol_sight = uncolor_piece(piece_in_sight);
			if (color != piece_color(piece_in_sight)
				&& ((uncol_sight == KING && !k) || uncol_sight == QUEEN
					|| uncol_sight == ROOK))
			{
				printf("1\n");
				printf("%s; x: %d; y: %d\n", piece_to_str(piece_in_sight), x, king_pos.y);
				return (true);
			}
			break ;
		}
		k++;
	}
	k = 0;
	for (int x = king_pos.x + 1; x < WIDTH; x++)
	{
		t_piece	piece_in_sight = game->board[king_pos.y][x];
		if (piece_in_sight)
		{
			t_uncolored_piece uncol_sight = uncolor_piece(piece_in_sight);
			if (color != piece_color(piece_in_sight)
				&& ((uncol_sight == KING && !k) || uncol_sight == QUEEN
					|| uncol_sight == ROOK))
			{
				printf("2\n");
				printf("%s; x: %d; y: %d\n", piece_to_str(piece_in_sight), x, king_pos.y);
				return (true);
			}
			break ;
		}
		k++;
	}
	k = 0;
	for (int y = king_pos.y - 1; y >= 0; y--)
	{
		t_piece	piece_in_sight = game->board[y][king_pos.x];
		if (piece_in_sight)
		{
			t_uncolored_piece uncol_sight = uncolor_piece(piece_in_sight);
			if (color != piece_color(piece_in_sight)
				&& ((uncol_sight == KING && !k) || uncol_sight == QUEEN
					|| uncol_sight == ROOK))
			{
				printf("3\n");
				printf("%s; x: %d; y: %d\n", piece_to_str(piece_in_sight), king_pos.x, y);
				return (true);
			}
			break ;
		}
		k++;
	}
	k = 0;
	for (int y = king_pos.y + 1; y < HEIGHT; y++)
	{
		t_piece	piece_in_sight = game->board[y][king_pos.x];
		if (piece_in_sight)
		{
			t_uncolored_piece uncol_sight = uncolor_piece(piece_in_sight);
			if (color != piece_color(piece_in_sight)
				&& ((uncol_sight == KING && !k) || uncol_sight == QUEEN
					|| uncol_sight == ROOK))
			{
				printf("4\n");
				printf("%s; x: %d; y: %d\n", piece_to_str(piece_in_sight), king_pos.x, y);
				return (true);
			}
			break ;
		}
		k++;
	}
	k = 0;
	for (int x= king_pos.x - 1, y = king_pos.y - 1; x>= 0 && y >= 0; x--, y--)
	{
		t_piece	piece_in_sight = game->board[y][x];
		if (piece_in_sight)
		{
			t_uncolored_piece uncol_sight = uncolor_piece(piece_in_sight);
			if (color != piece_color(piece_in_sight)
				&& ((uncol_sight == KING && !k) || uncol_sight == QUEEN
					|| uncol_sight == BISHOP))
			{
				printf("3\n");
				printf("%s; x: %d; y: %d\n", piece_to_str(piece_in_sight), x, y);
				return (true);
			}
			break ;
		}
		k++;
	}
	k = 0;
	for (int x= king_pos.x + 1, y = king_pos.y + 1; x < WIDTH && y < HEIGHT; x++, y++)
	{
		t_piece	piece_in_sight = game->board[y][x];
		if (piece_in_sight)
		{
			t_uncolored_piece uncol_sight = uncolor_piece(piece_in_sight);
			if (color != piece_color(piece_in_sight)
				&& ((uncol_sight == KING && !k) || uncol_sight == QUEEN
					|| uncol_sight == BISHOP))
			{
				printf("3\n");
				printf("%s; x: %d; y: %d\n", piece_to_str(piece_in_sight), x, y);
				return (true);
			}
			break ;
		}
		k++;
	}
	k = 0;
	for (int x= king_pos.x - 1, y = king_pos.y + 1; x>= 0 && y < HEIGHT; x--, y++)
	{
		t_piece	piece_in_sight = game->board[y][x];
		if (piece_in_sight)
		{
			t_uncolored_piece uncol_sight = uncolor_piece(piece_in_sight);
			if (color != piece_color(piece_in_sight)
				&& ((uncol_sight == KING && !k) || uncol_sight == QUEEN
					|| uncol_sight == BISHOP))
			{
				printf("3\n");
				printf("%s; x: %d; y: %d\n", piece_to_str(piece_in_sight), x, y);
				return (true);
			}
			break ;
		}
		k++;
	}
	k = 0;
	for (int x= king_pos.x + 1, y = king_pos.y - 1; x < WIDTH && y >= 0; x++, y--)
	{
		t_piece	piece_in_sight = game->board[y][x];
		if (piece_in_sight)
		{
			t_uncolored_piece uncol_sight = uncolor_piece(piece_in_sight);
			if (color != piece_color(piece_in_sight)
				&& ((uncol_sight == KING && !k) || uncol_sight == QUEEN
					|| uncol_sight == BISHOP))
			{
				printf("3\n");
				printf("%s; x: %d; y: %d\n", piece_to_str(piece_in_sight), x, y);
				return (true);
			}
			break ;
		}
		k++;
	}
	// check knights
	const int vecs[8][2] = KNIGHT_VECS;
	t_move	move;
	move.xo = king_pos.x;
	move.yo = king_pos.y;
	for (uint8_t i = 0; i < 8; i++)
	{
		move.xn = move.xo + vecs[i][0];
		move.yn = move.yo + vecs[i][1];
		if (bounds_check(move))
		{
			t_piece	piece = game->board[move.yn][move.xn];
			if (piece && piece_color(piece) != color
					&& uncolor_piece(piece) == KNIGHT)
			{
				printf("5\n");
				printf("%s; x: %d; y: %d\n", piece_to_str(piece), move.xn, move.yn);
				return (true);
			}
		}
	}
	// check pawns
	int	pawn_row = king_pos.y;
	if (color == BLACK)
		pawn_row += 1;
	else
		pawn_row -= 1;
	if (pawn_row >= 0 && pawn_row < HEIGHT)
	{
		int	pawn_col;
		pawn_col = king_pos.x - 1;
		if (pawn_col >= 0)
		{
			t_piece	potential_pawn = game->board[pawn_row][pawn_col];
			if (potential_pawn && color != piece_color(potential_pawn))
			{
				printf("6\n");
				printf("%s; x: %d; y: %d\n", piece_to_str(potential_pawn), pawn_col, pawn_row);
				return (true);
			}
		}
		pawn_col = king_pos.x + 1;
		if (pawn_col < WIDTH)
		{
			t_piece	potential_pawn = game->board[pawn_row][pawn_col];
			if (potential_pawn && color != piece_color(potential_pawn))
			{
				printf("7\n");
				printf("%s; x: %d; y: %d\n", piece_to_str(potential_pawn), pawn_col, pawn_row);
				return (true);
			}
		}
	}
	return (false);
}

bool	endangers_king(t_move move, t_player *player)
{
	t_game		game_cpy = *player->game;
	t_player	player_cpy = *player;
	player_cpy.game = &game_cpy;
	execute_move(&game_cpy, move, false);
	printf("checking move xo: %d; yo: %d-> xn: %d; yn: %d\n", move.xo, move.yo, move.xn, move.yn);
	if (in_check(&player_cpy))
	{
		printf("endangers king:\n");
		//t_game		game_cpy = *player->game;
		//t_player	player_cpy = *player;
		//player_cpy.game = &game_cpy;
		//execute_move(&game_cpy, move, true);
		print_board(player->game);
		printf("->\n");
		print_board(&game_cpy);
		printf("-------------------------\n");
		return (true);
	}
	printf("does not endanger king\n");
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
			&& !endangers_king(move, player))
		{
			dyn_arr_add_save((void **)moves, &move, (*moves_count)++);
		}
		if (!bounds_check(move) || player->game->board[move.yn][move.xn])
			return ;
	}
}

void	add_king_moves(t_player *player, t_move move, t_move **moves, size_t *moves_count)
{
	const int vecs[8][2] = KING_VECS;
	for (uint8_t i = 0; i < 8; i++)
	{
		move.xn = vecs[i][0];
		move.yn = vecs[i][1];
		add_line_moves(player, move, moves, moves_count, 1);
	}
}

void	add_queen_moves(t_player *player, t_move move, t_move **moves, size_t *moves_count)
{
	const int vecs[8][2] = KING_VECS;
	for (uint8_t i = 0; i < 8; i++)
	{
		move.xn = vecs[i][0];
		move.yn = vecs[i][1];
		for (int j = 1; j <= 7; j++)
			add_line_moves(player, move, moves, moves_count, j);
	}
}

void	add_bishop_moves(t_player *player, t_move move, t_move **moves, size_t *moves_count)
{
	const int vecs[4][2] = BISHOP_VECS;
	for (uint8_t i = 0; i < 4; i++)
	{
		move.xn = vecs[i][0];
		move.yn = vecs[i][1];
		for (int j = 1; j <= 7; j++)
			add_line_moves(player, move, moves, moves_count, j);
	}
}

void	add_knight_moves(t_player *player, t_move move, t_move **moves, size_t *moves_count)
{
	const int vecs[8][2] = KNIGHT_VECS;
	for (uint8_t i = 0; i < 8; i++)
	{
		move.xn = move.xo + vecs[i][0];
		move.yn = move.yo + vecs[i][1];
		if (bounds_check(move) && color_check(move, player->game)
			&& !endangers_king(move, player))
		{
			dyn_arr_add_save((void **)moves, &move, (*moves_count)++);
		}
	}
}

//todo: add casteling
void	add_rook_moves(t_player *player, t_move move, t_move **moves, size_t *moves_count)
{
	const int vecs[4][2] = ROOK_VECS;
	for (uint8_t i = 0; i < 4; i++)
	{
		move.xn = vecs[i][0];
		move.yn = vecs[i][1];
		for (int j = 1; j <= 7; j++)
			add_line_moves(player, move, moves, moves_count, j);
	}
}

//todo: add pawn promation and en passnt
void	add_pawn_moves(t_player *player, t_move move, t_move **moves, size_t *moves_count)
{
	move.xn = move.xo;
	int	direct_y = -1;
	if (player->color == BLACK)
		direct_y = 1;
	move.yn = move.yo + direct_y;
	if (bounds_check(move)
			&& !player->game->board[move.yn][move.xo]
			&& !endangers_king(move, player))
	{
		dyn_arr_add_save((void **)moves, &move, (*moves_count)++);
	}
	move.xn--;
	if (bounds_check(move)
		&& player->game->board[move.yn][move.xn]
		&& player->color != piece_color(player->game->board[move.yn][move.xn]
		&& !endangers_king(move, player))
	)
	{
		dyn_arr_add_save((void **)moves, &move, (*moves_count)++);
	}
	move.xn += 2;
	if (bounds_check(move)
		&& player->game->board[move.yn][move.xn]
		&& player->color != piece_color(player->game->board[move.yn][move.xn]
		)//&& !endangers_king(move, player))
	)
	{
		dyn_arr_add_save((void **)moves, &move, (*moves_count)++);
	}
	if ((player->color == BLACK && move.yo == 1)
		|| (player->color == WHITE && move.yo == 6))
	{
		move.xn = move.xo;
		move.yn += direct_y;
		if (!player->game->board[move.yn][move.xo] && !endangers_king(move, player))
		{
			dyn_arr_add_save((void **)moves, &move, (*moves_count)++);
		}
	}
}

void	add_pieces_moves(t_position cur_position, t_player *player, t_move move, t_move **moves, size_t *moves_count)
{
	t_uncolored_piece	piece = uncolor_piece(cur_position.type);
	move.xo = cur_position.x;
	move.yo = cur_position.y;
	t_move tmp = move;
	switch (piece)
	{
		case (KING):
		{
			add_king_moves(player, move, moves, moves_count);
			break ;
		}
		case (QUEEN):
		{
			add_queen_moves(player, move, moves, moves_count);
			break ;
		}
		case (BISHOP):
		{
			add_bishop_moves(player, move, moves, moves_count);
			break ;
		}
		case (KNIGHT):
		{
			add_knight_moves(player, move, moves, moves_count);
			break ;
		}
		case (ROOK):
		{
			add_rook_moves(player, move, moves, moves_count);
			break ;
		}
		case (PAWN):
		{
			//add_pawn_moves(player, move, moves, moves_count);
			break ;
		}
		default:
		{
			printf("%d\n", piece);
			ERROR("unknown piece");
		}
		ASSUME(tmp.xo == move.xo && tmp.yo == move.yo);
	}
}

void	get_all_possible_moves(t_player *player, t_move **ret_moves, size_t *ret_moves_count)
{
	t_move move = {0};
	const t_color	color = player->color;
	const	uint8_t	piece_count = player->game->piece_count[color];
	ASSUME(piece_count >= 0 && piece_count <= 16);
	//if (in_check(player))
	//{
	//	handle_in_check(player);
	//	return ;
	//}
	*ret_moves = dyn_arr_init(sizeof(t_move), 1);

	for (uint8_t i = 0; i < piece_count; i++)
	{
		t_position			cur_position = player->game->positions[color][i];
		if (cur_position.type != EMPTY)
		{
			add_pieces_moves(cur_position, player, move, ret_moves, ret_moves_count);
		}
	}
}

//assumes aquired mutex for game
t_move	get_rdm_move(t_player *player)
{
	t_move	*moves = NULL;
	size_t	moves_count = 0;
	get_all_possible_moves(player, &moves, &moves_count);
	printf("possible moves: %lu\n", moves_count);
	t_move	move = {0};
	if (!moves_count)
	{
		if (in_check(player))
		{
			printf("player %s lost!\n", color_to_str(player->color));
			exit(0);
		}
		else
		{
			printf("(%s turn:)draw!\n", color_to_str(player->color));
		}
		exit(0);
		player_cleanup(player);
		ASSUME(0);
	}
	move = moves[rand() % moves_count];
	dyn_arr_free((void **)&moves);
	return (move);
}

void	execute_move(t_game *game, t_move move, bool print)
{
	t_piece	piece = game->board[move.yo][move.xo];
	t_color	color = piece_color(piece);
	t_uncolored_piece uncol_piece = uncolor_piece(piece);

	t_piece	target = game->board[move.yn][move.xn];

	if (print)
	{
		printf("%s is moving %d/%d(%s) to %d/%d(%s) (x/y)\n",
				color_to_str(color),
				move.xo, move.yo,
				piece_to_str(piece),
				move.xn, move.yn,
				piece_to_str(target)
		);
	}
	
	// update new postion data structre
	for (int i = 0; i < 16; i++)
	{
		if (game->positions[color][i].x == move.xo
			&& game->positions[color][i].y == move.yo)
		{
			game->positions[color][i].x = move.xn;
			game->positions[color][i].y = move.yn;
			break ;
		}
		ASSUME(i < 15);
	}
	if (target)
	{
		t_color	opp_color = inverse_color(color);
		const uint8_t	postion_data_size =
				sizeof game->positions[opp_color][0];
		printf("xo: %d; yo: %d\n", move.xo, move.yo);
		printf("xn: %d; yn: %d\n", move.xn, move.yn);
		for (int i = 0; i < 16; i++)
		{
			printf("game->positions[opp_color][i] x: %d; y: %d\n", game->positions[opp_color][i].x, game->positions[opp_color][i].y);
			if (game->positions[opp_color][i].x == move.xn
				&& game->positions[opp_color][i].y == move.yn)
			{
				uint8_t	shifts = 15 - i;
				if (!shifts)
					break ;
				ASSUME(shifts >= 0 && shifts < 16);
				memmove(game->positions[opp_color] + i,
						game->positions[opp_color] + i + 1,
						shifts * postion_data_size
					);
				break ;
			}
			if (!(i < game->piece_count[opp_color] - 1))
			{
				printf("i: %d; opp_color: %s; color: %s; opp_cnt: %d; cnt: %d\n", i,
						color_to_str(opp_color), color_to_str(color), game->piece_count[opp_color], game->piece_count[color]);
				print_board(game);
				ASSUME(i < game->piece_count[opp_color] - 1);
			}
		}
		game->piece_count[opp_color]--;
		bzero(game->positions[opp_color] + game->piece_count[opp_color],
				postion_data_size);
	}
	// update old data structure
	game->board[move.yn][move.xn] =
		game->board[move.yo][move.xo];
	game->board[move.yo][move.xo] = 0;
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
		t_piece	king = player->game->positions[player->color][0].type;
		if (king != KING_B && king != KING_W)
		{
			printf("%s lost his king!\n", color_to_str(player->color));
			exit(0);
			player_cleanup(player);
		}
		printf("%ss turn!\n", color_to_str(player->color));
		t_move rdm_move = get_rdm_move(player);
		execute_move(player->game, rdm_move, true);
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
