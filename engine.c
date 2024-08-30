#include "main.h"
#include "engine.h"


int iters = 0;

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
//todo: castle
//todo: refactor
bool	in_check(t_player *player)
{
	t_color	color = player->color;
	t_game	*game = player->game;
	t_position	king_pos = game->positions[color][0];
	const int	directions[][4][2] = {{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}, {{1 , 1}, {-1, -1}, {1, -1}, {-1, 1}}};
	const t_uncolored_piece	pieces[][3] = {{KING, QUEEN, ROOK}, {KING, QUEEN, BISHOP}, };

	for (int i = 0; i < sizeof pieces / 3 / sizeof pieces[0][0]; i ++)
	{
		t_uncolored_piece cur_cmp[3];
	
		memcpy(cur_cmp, pieces[i], sizeof cur_cmp);
		for (int direction = 0; direction < 4; direction++)
		{
			int	direct_x = directions[i][direction][0];
			int	direct_y = directions[i][direction][1];
			int	steps = 0;
			for (int x = king_pos.x + direct_x, y = king_pos.y + direct_y;
					x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
					x += direct_x, y += direct_y
				)
			{
				steps++;
				t_piece	piece_in_sight = game->board[y][x];
				if (!piece_in_sight)
					continue ;
				if (color == piece_color(piece_in_sight))
					break ;
				t_uncolored_piece uncol_sight = uncolor_piece(piece_in_sight);
				for (int j = 0; j < 3; j++)
				{
					if (uncol_sight == cur_cmp[j] && (uncol_sight != KING || steps == 1))
					{
						return (true);
					}
				}
				break ;
			}
		}
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
			if (potential_pawn && color != piece_color(potential_pawn) && uncolor_piece(potential_pawn) == PAWN)
			{
				return (true);
			}
		}
		pawn_col = king_pos.x + 1;
		if (pawn_col < WIDTH)
		{
			t_piece	potential_pawn = game->board[pawn_row][pawn_col];
			if (potential_pawn && color != piece_color(potential_pawn) && uncolor_piece(potential_pawn) == PAWN)
			{
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
	if (in_check(&player_cpy))
	{
		return (true);
	}
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
		//for (int j = 1; j <= 7; j++)
		add_line_moves(player, move, moves, moves_count, 7);
	}
}

void	add_bishop_moves(t_player *player, t_move move, t_move **moves, size_t *moves_count)
{
	const int vecs[4][2] = BISHOP_VECS;
	for (uint8_t i = 0; i < 4; i++)
	{
		move.xn = vecs[i][0];
		move.yn = vecs[i][1];
		//for (int j = 1; j <= 7; j++)
		add_line_moves(player, move, moves, moves_count, 7);
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
		//for (int j = 1; j <= 7; j++)
		add_line_moves(player, move, moves, moves_count, 7);
	}
}

//todo: add pawn promation and en passnt
//todo: refactor
void	add_pawn_moves(t_player *player, t_move move, t_move **moves, size_t *moves_count)
{
	move.xn = move.xo;
	move.yn = move.yo;

	// move 1 vertical
	int	direct_y = -1;
	if (player->color == BLACK)
		direct_y = 1;
	move.yn += direct_y;
	if (bounds_check(move)
			&& !player->game->board[move.yn][move.xo]
			&& !endangers_king(move, player))
	{
		dyn_arr_add_save((void **)moves, &move, (*moves_count)++);
	}
	// take 1 vertical, 1 left
	move.xn--;
	if (bounds_check(move)
		&& player->game->board[move.yn][move.xn]
		&& player->color != piece_color(player->game->board[move.yn][move.xn])
		&& !endangers_king(move, player)
	)
	{
		dyn_arr_add_save((void **)moves, &move, (*moves_count)++);
	}
	// take 1 vertical, 1 right
	move.xn += 2;
	if (bounds_check(move)
		&& player->game->board[move.yn][move.xn]
		&& player->color != piece_color(player->game->board[move.yn][move.xn])
		&& !endangers_king(move, player)
	)
	{
		dyn_arr_add_save((void **)moves, &move, (*moves_count)++);
	}
	// move 2 vertical
	move.xn = move.xo;
	move.yn = move.yn + 2 * direct_y;
	if ((player->color == BLACK && move.yo == 1)
		|| (player->color == WHITE && move.yo == 6))
	{
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
			add_pawn_moves(player, move, moves, moves_count);
			break ;
		}
		default:
		{
			printf("%d\n", piece);
			ASSUME(0);
		}
		ASSUME(tmp.xo == move.xo && tmp.yo == move.yo);
	}
}

void	get_all_possible_moves(t_player *player, t_move **ret_moves, size_t *ret_moves_count)
{
	t_move move = {0};
	*ret_moves_count = 0;
	const t_color	color = player->color;
	const	uint8_t	piece_count = player->game->piece_count[color];
	ASSUME(piece_count >= 0 && piece_count <= 16);
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

int	get_capture_piece_score(t_piece piece)
{
	t_uncolored_piece	uncol_piece = uncolor_piece(piece);
	switch (uncol_piece)
	{
		case (KING): ASSUME(0);
		case (QUEEN): return (15);
		case (BISHOP): return (7);
		case (KNIGHT): return (7);
		case (ROOK): return (7);
		case (PAWN): return (2);
		default: ASSUME(0);
	}
}

int	execute_move(t_game *game, t_move move, bool print)
{
	memcpy(game->all_boards + game->generate_turn, game->board, sizeof game->board);
	t_piece	piece = game->board[move.yo][move.xo];
	t_color	color = piece_color(piece);
	t_uncolored_piece uncol_piece = uncolor_piece(piece);
	int		score = 0;


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

		score += get_capture_piece_score(game->board[move.yn][move.xn]);
		for (int i = 0; i < 16; i++)
		{
			if (game->positions[opp_color][i].x == move.xn
				&& game->positions[opp_color][i].y == move.yn)
			{
				game->piece_count[opp_color]--;
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
			ASSUME(i < game->piece_count[opp_color] - 1);
		}
		bzero(game->positions[opp_color] + game->piece_count[opp_color],
				postion_data_size);
	}
	// update old data structure
	game->board[move.yn][move.xn] =
		game->board[move.yo][move.xo];
	game->board[move.yo][move.xo] = 0;
	return (score);
}

// playr thread loop
void	*game_loop(void *player_data)
{
	t_player	*player = (t_player *)player_data;

	while (1)
	{
		pthread_mutex_lock(&player->game->mutex);
		while (player->color != player->game->turn)
			pthread_cond_wait(&player->game->turn_over, &player->game->mutex);
		t_piece	king = player->game->positions[player->color][0].type;
		ASSUME(king == KING_B || king == KING_W);//king should never not exist
		pthread_mutex_lock(&player->game->mutex_eval);
		if (player->game->state != ONGOING)
		{
			pthread_mutex_unlock(&player->game->mutex_eval);
			player_cleanup(player);
		}
		pthread_mutex_unlock(&player->game->mutex_eval);
		t_move	*moves;
		size_t	move_count = 0;

		get_all_possible_moves(player, &moves, &move_count);
		//for (int i = 0; i < move_count; i++)
		//	print_move(moves[i], player->game);
		if (!move_count)
		{
			pthread_mutex_lock(&player->game->mutex_eval);
			if (in_check(player))
			{
				if (player->color == BLACK)
					player->game->state = WHITE_WON;
				else
					player->game->state = BLACK_WON;
			}
			else
				player->game->state = TIE;
			pthread_mutex_unlock(&player->game->mutex_eval);
			player_cleanup(player);
			ASSUME(0);
		}
		pthread_mutex_lock(&player->game->mutex_eval);
		size_t	turn = player->game->generate_turn;
		player->game->moves[turn] = select_move_neural_net(player, moves, move_count);
		player->game->moves[turn].score = execute_move(player->game, player->game->moves[turn], false);
		player->game->turn = inverse_color(player->game->turn);
		printf("generate_turn: %lu\n", player->game->generate_turn);
		player->game->generate_turn++;
		if (player->game->generate_turn >= MAX_MOVES)
		{
			player->game->state = TIE;
			pthread_mutex_unlock(&player->game->mutex_eval);
			player_cleanup(player);
			ASSUME(0);
		}
		pthread_mutex_unlock(&player->game->mutex_eval);
		pthread_cond_broadcast(&player->game->turn_over);
		pthread_mutex_unlock(&player->game->mutex);
	}
	ASSUME(0);
	return (NULL);
}

