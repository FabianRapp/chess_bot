#ifndef MAIN_H
# define MAIN_H

/* Missing moves:
* pawn promotion
* en passnt
* casteling
*/
/*TODO:
 * create look up table for pieces that protect the king
 * differ from check/no check move generation
 * missingg moves
 * code cleanup and simplification
*/

typedef enum e_uncolored_piece	t_uncolored_piece;
typedef enum e_piece			t_piece;
typedef enum e_color			t_color;
typedef struct s_move			t_move;
typedef struct s_player			t_player;
typedef struct s_position		t_position;
typedef struct s_game			t_game;

#define ERROR(str) do {fprintf(stderr, "error in %s at %d: %s\n", __FILE__,\
							__LINE__, str);exit(1);} while(0)

#ifdef NO_DEBUG
# define ASSUME(cond) do { if (!(cond)){__builtin_unreachable();}} while(0)
#else // NO_DEBUG
# define ASSUME(cond) assert(cond)
#endif//NO_DEBUG

#ifndef GAME_COUNT
# define GAME_COUNT 1
#endif // GAME_COUNT

# ifndef ALIGNMENT
#  define ALIGNMENT 32
# endif//ALIGNMENT

# ifndef MAX_MOVES
#  define MAX_MOVES 1000
# endif

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
#include "neural_network.h"
#include "types.h"

typedef enum e_game_state
{
	NOT_STARTED = 0,
	ONGOING = 1,
	TIE = 2,
	BLACK_WON,
	WHITE_WON,
	DONE
}	t_game_state;

typedef struct s_position
{
	uint8_t				x;
	uint8_t				y;
	t_piece				type;
}	t_position;

typedef struct s_player
{
	t_color				color;
	t_game				*game;
	pthread_t			thread;
	t_neural_network	neural_net;
}	t_player;

typedef struct s_game
{
	t_piece			board[HEIGHT][WIDTH];//has all pieces with empty field
	t_piece			all_boards[MAX_MOVES][HEIGHT][WIDTH];
	t_position		positions[COLOR_COUNT][16];//has all pieces without empty fields
	uint8_t			piece_count[COLOR_COUNT];
	t_move			moves[MAX_MOVES];
	pthread_cond_t	turn_over;
	pthread_mutex_t	mutex;
	bool			check;
	uint8_t			turn;
	pthread_mutex_t	mutex_eval;
	t_game_state	state;
	size_t			generate_turn;
	size_t			eval_turn;
}	t_game;

typedef struct s_manager
{
	size_t		game_count;
	t_game		*games;
	t_player	*black_players;
	t_player	*white_players;
}	t_manager;


// engine.c
void	*game_loop(void *player_data);
int		execute_move(t_game *game, t_move move, bool print);
bool	in_check(t_player *player);
void	get_all_possible_moves(t_player *player, t_move **ret_moves, size_t *ret_moves_count);
t_move	get_rdm_move(t_player *player);

// neural_network.c
void				init_neural_net(t_neural_network *neural_net);
t_move				select_move_neural_net(t_player *player, t_move *moves, size_t move_count);
void				store_neural_net(t_neural_network *neural_net, t_color color);
void				load_neural_net(t_neural_network *neural_net, t_color color);

// utils1.c
t_color				piece_color(t_piece piece);
t_uncolored_piece	uncolor_piece(t_piece piece);
t_color				inverse_color(t_color color);

//debug.c
char	*piece_to_str(t_piece piece);
void	print_board(t_game *game);
char	*color_to_str(t_color color);
void	print_move(t_move move, t_game *game);

// init.c
int	launch_game(t_manager *manager, size_t game_index);
int	init(t_manager *manager);

// reset.c
void	place_new_piece(t_game *game, uint8_t x, uint8_t y, t_piece piece);
void	reset_game(t_game *game);

#endif
