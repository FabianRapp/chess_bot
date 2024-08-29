
#include "neural_network.h"
#include "main.h"

void	store_neural_net(t_neural_network *neural_net, char *path)
{
	int fd = open(path, O_WRONLY);
	assert(fd != -1);
	int	write_ret = write(fd, neural_net, sizeof *neural_net);
	assert(write_ret == sizeof *neural_net);
	close(fd);
}

t_neural_network	load_neural_net(char *path)
{
	t_neural_network	neural_net;
	int	fd = open(path, O_RDONLY);
	assert(fd != -1);
	int	read_ret = read(fd, &neural_net, sizeof neural_net);
	assert(read_ret == sizeof neural_net);
	return (neural_net);
}

// idk, understand later
double generate_normal(double mean, double stddev)
{
	double	u1;
	double	u2;
	double	z0;

	u1 = rand() / (RAND_MAX + 1.0);
	u2 = rand() / (RAND_MAX + 1.0);

	// Box-Muller transform
	z0 = sqrtl(-2.0 * logl(u1)) * cos(2 * M_PI * u2);

	// Scale and shift to match mean and standard deviation
	return (z0 * stddev + mean);
}

void	init_layer(t_layer *layer, int input_count,
		int output_count)
{
	srand(time(NULL));
	int ret = posix_memalign((void **)&layer->weights, ALIGNMENT,
			sizeof(double) * output_count * input_count);
	assert(!ret);
	double	stddev = sqrtl(2.0 / input_count);
	for (int i = 0; i < output_count * input_count; i++)
		layer->weights[i] = generate_normal(0, stddev);
	ret = posix_memalign((void **)&layer->biases, ALIGNMENT,
			sizeof (double) * output_count);
	assert(!ret);
	stddev = sqrtl(2.0 / output_count);
	for (int i = 0; i < output_count; i++)
		layer->biases[i] = rand() / (double)RAND_MAX;
	layer->input_count = input_count;
	layer->output_count = output_count;
}

void	init_neural_net(t_neural_network *neural_net)
{
	ASSUME(HIDDEN_LAYER_COUNT >= 1);
	//init_layer(&neural_net->input_layer, INPUT_DIM, HIDDEN_LAYER_DIM);
	init_layer(neural_net->hidden_layers, INPUT_DIM, HIDDEN_LAYER_DIM);
	for (size_t i = 1; i < HIDDEN_LAYER_COUNT; i++)
	{
		init_layer(neural_net->hidden_layers + i, HIDDEN_LAYER_DIM,
			HIDDEN_LAYER_DIM);
	}
	init_layer(&neural_net->output_layer, HIDDEN_LAYER_DIM, OUTPUT_DIM);
}

double	rectified_linear_unit_activation(double x)
{
	return (MAX(x, 0));
}

double	do_activation(double sum, size_t layer)
{
	return (rectified_linear_unit_activation(sum));
}

void	backward_propagate(t_neural_network *neural_net)
{
}

// caller handels input cleanup and output allocation
void	forward_propagate(t_neural_network *neural_net, double *inputs,
		double *outputs)
{
	double	*current_input = inputs;
	double	*layer_out;
	t_layer	*cur;
	for (int i = 0; i < HIDDEN_LAYER_COUNT; i++)
	{
		cur = neural_net->hidden_layers + i;
		ASSUME(cur->output_count > 0);
		layer_out = malloc(sizeof(double) * cur->output_count);
		assert(layer_out);
		for (int out = 0; out < cur->output_count; out++)
		{
			double	sum = 0.0;
			__m256d vec_sum = _mm256_setzero_pd();
			int	in;
			double	*weights = cur->weights;
			const size_t	input_count = cur->input_count;
			const size_t	offset = out * input_count;
			for (in = 0; in < input_count - 4; in += 4)
			{
				__m256d vec_weights = _mm256_load_pd(weights + offset + in);
				__m256d vec_input = _mm256_load_pd(current_input + in);
				vec_sum = _mm256_fmadd_pd(vec_weights, vec_input, vec_sum);
				//sum += cur->weights[out * cur->input_count + in] * current_input[in];
			}
			alignas(ALIGNMENT)double temp[4];
    		_mm256_store_pd(temp, vec_sum);
    		sum = temp[0] + temp[1] + temp[2] + temp[3];
			for (;in < cur->input_count; in++)
			{
				sum += weights[offset + in] * current_input[in];
			}
			layer_out[out] = do_activation(sum, i);
		}
		if (i > 0)
			free(current_input);
		current_input = layer_out;
	}
	layer_out = outputs;
	cur = &neural_net->output_layer;
	for (int out = 0; out < cur->output_count; out++)
	{
		double	sum = 0.0;
		for (int in = 0; in < cur->input_count; in++)
		{
			sum += cur->weights[out * cur->input_count + in]
				* current_input[in];
		}
		layer_out[out] = do_activation(sum, HIDDEN_LAYER_COUNT);
	}
	if (HIDDEN_LAYER_DIM > 1)
		free(current_input);
}

void	eval_board(const t_piece board[HEIGHT][WIDTH], t_neural_network *neural_net, double *outputs)
{
	alignas(ALIGNMENT) double	board_converted[INPUT_DIM];
	size_t	index;
	for (size_t y = 0; y < HEIGHT; y++)
	{
		for (size_t x = 0; x < WIDTH; x++)
		{
			const t_piece	cur_piece = board[y][x];
			for (t_piece i = 0; i < PIECE_COUNT; i++)
			{
				index = y * WIDTH * PIECE_COUNT + x * PIECE_COUNT + i;
				if (cur_piece == i)
					board_converted[index] = 1.0;
				else
					board_converted[index] = 0.0;
			}
		}
	}
	ASSUME(index == INPUT_DIM - 1);
	forward_propagate(neural_net, board_converted, outputs);
}

double	avg(double *arr, size_t len)
{
	double	sum = 0.0;
	ASSUME(len);
	for (size_t i = 0; i < len; i++)
	{
		sum += arr[i];
	}
	return (sum / len);
}

t_move	select_move_neural_net(t_player *player, t_move *moves, size_t move_count)
{
	ASSUME(move_count && moves);
	double best_score = -INFINITY;
	t_move	best_move;
	t_piece	board[HEIGHT][WIDTH];
	memcpy(board, player->game->board, sizeof board);
	for (size_t i = 0; i < move_count; i++)
	{
		t_move	move = moves[i];
		t_piece	old_pos = board[move.yo][move.xo];
		t_piece	new_pos = board[move.yn][move.xn];
		board[move.yn][move.xn] = old_pos;
		board[move.yo][move.xo] = 0;
		double *scores = malloc(sizeof(double) * OUTPUT_DIM);
		assert(scores);
		eval_board(board, &(player->neural_net), scores);
		double	score = avg(scores, OUTPUT_DIM);
		free(scores);
		board[move.yn][move.xn] =  new_pos;
		board[move.yo][move.xo] = old_pos;
		best_score = MAX(best_score, score);
		best_move = (score == best_score) ? moves[i] : best_move;
	}
	dyn_arr_free((void **)&moves);
	ASSUME(!moves);
	return (best_move);
}
