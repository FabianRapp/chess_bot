#include "main.h"

void	place_new_piece(t_game *game, uint8_t x, uint8_t y, t_piece piece)
{
	t_color color = piece_color(piece);

	game->positions[color][game->piece_count[color]].x = x;
	game->positions[color][game->piece_count[color]].y = y;
	game->positions[color][game->piece_count[color]].type = piece;
	game->piece_count[color]++;
	game->board[y][x] = piece;
}

void	reset_game(t_game *game)
{
	bzero(game->board, sizeof game->board);
	bzero(game->positions, sizeof game->positions);

	game->state = NOT_STARTED;
	game->turn = WHITE;
	game->check = false;
	game->piece_count[BLACK] = 0;
	game->piece_count[WHITE] = 0;
	game->generate_turn = 0;
	game->eval_turn = 0;

	place_new_piece(game, 4, 0, KING_B);
	place_new_piece(game, 4, 7, KING_W);

	place_new_piece(game, 3, 0, QUEEN_B);
	place_new_piece(game, 3, 7, QUEEN_W);

	place_new_piece(game, 2, 0, BISHOP_B);
	place_new_piece(game, 5, 0, BISHOP_B);
	place_new_piece(game, 2, 7, BISHOP_W);
	place_new_piece(game, 5, 7, BISHOP_W);

	place_new_piece(game, 1, 0, KNIGHT_B);
	place_new_piece(game, 6, 0, KNIGHT_B);
	place_new_piece(game, 1, 7, KNIGHT_W);
	place_new_piece(game, 6, 7, KNIGHT_W);

	place_new_piece(game, 0, 0, ROOK_B);
	place_new_piece(game, 7, 0, ROOK_B);
	place_new_piece(game, 0, 7, ROOK_W);
	place_new_piece(game, 7, 7, ROOK_W);

	for (size_t x = 0; x < WIDTH; x++)
	{
		place_new_piece(game, x, 1, PAWN_B);
		place_new_piece(game, x, 6, PAWN_W);
	}
}

