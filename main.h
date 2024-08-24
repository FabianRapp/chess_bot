#ifndef MAIN_H
#define MAIN_H

/* Missing moves:
* pawn promotion
* knight movement
* en passnt
* casteling
*/

#include <stdio.h>
#include <sys/param.h>
#include <assert.h>
#include "./libft/libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#ifndef GAME_COUNT
# define GAME_COUNT 1
#endif // GAME_COUNT

#define HEIGHT 8
#define WIDTH 8

#define ERROR(str) do {fprintf(stderr, "error in %s at %d: %s\n", __FILE__,\
							__LINE__, str);exit(1);} while(0)

// use % 2 to determine the color
// use (t_piece var -1) / 2 to match t_uncolored_piece to t_piece
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
}	t_piece;

#define KING_VECS		{{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}}
#define QUEEN_VECS		KING_VECS
#define BISHOP_VECS		{{1, 1}, {-1, 1}, {-1, -1}, {1, -1}}
#define KNIGHT_VECS		{{-2, -1}, {-2, 1}, {2, -1}, {2, 1}, {-1, -2}, {1, -2}, {-1, 2}, {1, 2}}
#define ROOK_VECS		{{0, 1}, {-1, 0}, {0, -1}, {1, 0}}
#define PAWN_VECS_WHITE	{{},}
#define PAWN_VECS_BLACK {{},}

#define BLACK 1
#define WHITE 0

typedef struct s_game t_game;

// fom field xo/yo to field xn/yn
typedef struct s_move
{
	int8_t	xo;
	int8_t	yo;
	int8_t	xn;
	int8_t	yn;
}	t_move;
//typedef struct s_move
//{
//	uint8_t	xo : 3;
//	uint8_t	yo : 3;
//	uint8_t	xn : 3;
//	uint8_t	yn : 3;
//}	t_move;

typedef struct s_thread
{
	uint8_t		color	;
	t_game		*game;
	pthread_t	thread;
}	t_player;

typedef struct s_game
{
	t_piece			board[HEIGHT][WIDTH];
	pthread_cond_t	turn_over;
	pthread_mutex_t	mutex;
	bool			check;
	uint8_t			turn ;
}	t_game;

typedef struct s_manager
{
	size_t		game_count;
	t_game		*games;
	t_player	*black_players;
	t_player	*white_players;
}	t_manager;

#endif
