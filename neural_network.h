#ifndef NEURAL_NETWORK_H
# define NEURAL_NETWORK_H

# include <math.h>
# include <unistd.h>
# include <fcntl.h>
# include "types.h"
# include <string.h>
# include <stdlib.h>
# include "./libft/libft.h"
# include <stdalign.h>
# include <immintrin.h>

# define INPUT_DIM (HEIGHT * WIDTH * PIECE_COUNT)
# define HIDDEN_LAYER_DIM 128
# define HIDDEN_LAYER_COUNT 2
# define OUTPUT_DIM 1 // diffrent scores per board

typedef struct s_player	t_player;

typedef struct s_layer
{
	double	*weights;
	double	*biases;
	int		input_count;
	int		output_count;
}	t_layer;

typedef struct s_neural_network
{
	t_layer	hidden_layers[HIDDEN_LAYER_COUNT]; //first is input layer
	t_layer	output_layer;
}	t_neural_network;

//public-----
t_move				select_move_neural_net(t_player *player, t_move *moves, size_t move_count);
void				init_neural_net(t_neural_network *neural_net);
void				store_neural_net(t_neural_network *neural_net, char *path);
t_neural_network	load_neural_net(char *path);

// priveate
void	eval_board(const t_piece board[HEIGHT][WIDTH],
				  t_neural_network *neural_net, double *outputs);
void	init_layer(t_layer *layer, int input_count, int output_count);
void	forward_propagate(t_neural_network *layer, double* inputs, double *outputs);


#endif //NEURAL_NETWORK_H
