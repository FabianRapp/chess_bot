#include "main.h"
#include "types.h"

//assumes aquired mutex for game
t_move	get_rdm_move(t_player *player)
{
	t_move	*moves = NULL;
	size_t	moves_count = 0;
	get_all_possible_moves(player, &moves, &moves_count);
	//debug logging------
	for (int i = 0; i < moves_count; i++)
	{
		ASSUME(!player->game->board[moves[i].yn][moves[i].xn]
			|| uncolor_piece(
				player->game->board[moves[i].yn][moves[i].xn]) != KING);
	}
	//---------------
	t_move	move = {0};
	if (!moves_count)
	{
		if (in_check(player))
		{
			printf("player %s lost!\n", color_to_str(player->color));
		}
		else
		{
			printf("(%s turn:)draw!\n", color_to_str(player->color));
		}
		player_cleanup(player);
		ASSUME(0);
	}
	move = moves[rand() % moves_count];
	dyn_arr_free((void **)&moves);
	return (move);
}


