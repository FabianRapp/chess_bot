#include "main.h"

t_color	piece_color(t_piece piece)
{
	assert(piece);
	return (piece % 2);
}
t_uncolored_piece	uncolor_piece(t_piece piece)
{
	assert(piece);
	return ((piece - 1) / 2);
}

t_color	inverse_color(t_color color)
{
	switch (color)
	{
		case (WHITE): return (BLACK);
		case (BLACK): return (WHITE);
		default: ASSUME(0);
	}
}

