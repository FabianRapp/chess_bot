#ifndef TYPES_H
# define TYPES_H

# include <stdint.h>

#define HEIGHT 8
#define WIDTH 8

typedef enum e_uncolored_piece
{
	KING = 0,
	QUEEN = 1,
	BISHOP = 2,
	KNIGHT = 3,
	ROOK = 4,
	PAWN = 5,
}	t_uncolored_piece;

typedef enum e_piece
{
	EMPTY = 0,
	KING_B = 1,
	KING_W = 2,
	QUEEN_B = 3,
	QUEEN_W = 4,
	BISHOP_B = 5,
	BISHOP_W = 6,
	KNIGHT_B = 7,
	KNIGHT_W = 8,
	ROOK_B = 9,
	ROOK_W = 10,
	PAWN_B = 11,
	PAWN_W = 12,
	PIECE_COUNT = 13, // includes empty fields
}	t_piece;

typedef enum e_color
{
	WHITE = 0,
	BLACK = 1,
	COLOR_COUNT = 2,
}	t_color;
// from field xo/yo to field xn/yn

typedef struct s_move
{
	int8_t	xo;//x old
	int8_t	yo;//y old
	int8_t	xn;//x new
	int8_t	yn;//y new
	int		score;
}	t_move;

#endif //TYPES_H

