#include "main.h"

char	*color_to_str(t_color color)
{
	switch (color)
	{
		case BLACK: return "BLACK";
		case WHITE: return "WHITE";
		default: return "INVALID COLOR";
	}
}

char *piece_to_str(t_piece piece)
{
	switch (piece)
	{
		case EMPTY: return "EMPTY";
		case KING_B: return "KING_B";
		case KING_W: return "KING_W";
		case QUEEN_B: return "QUEEN_B";
		case QUEEN_W: return "QUEEN_W";
		case BISHOP_B: return "BISHOP_B";
		case BISHOP_W: return "BISHOP_W";
		case KNIGHT_B: return "KNIGHT_B";
		case KNIGHT_W: return "KNIGHT_W";
		case ROOK_B: return "ROOK_B";
		case ROOK_W: return "ROOK_W";
		case PAWN_B: return "PAWN_B";
		case PAWN_W: return "PAWN_W";
		default: ERROR("uknown piece");
	}
}

void	print_board(t_game *game)
{
	printf("  |");
	for (int x = 0; x < WIDTH; x++)
		printf("%-3d|", x);
	printf("\n");
	for (int y = 0; y < HEIGHT; y++)
	{
		printf("%-2d|", y);
		for (int x = 0; x < WIDTH; x++)
		{
			switch (game->board[y][x])
			{
				case (EMPTY): printf("%-3s|", "   "); break ;
				case (KING_B): printf("%-3s|", "KKB"); break ;
				case (KING_W): printf("%-3s|", "KKW"); break ;
				case (QUEEN_B): printf("%-3s|", "QB"); break ;
				case (QUEEN_W): printf("%-3s|", "QW"); break ;
				case (BISHOP_B): printf("%-3s|", "BB "); break ;
				case (BISHOP_W): printf("%-3s|", "BW "); break ;
				case (KNIGHT_B): printf("%-3s|", "KB "); break ;
				case (KNIGHT_W): printf("%-3s|", "KW "); break ;
				case (ROOK_B): printf("%-3s|", "RB "); break ;
				case (ROOK_W): printf("%-3s|", "RW "); break ;
				case (PAWN_B): printf("%-3s|", "PB "); break ;
				case (PAWN_W): printf("%-3s|", "PW "); break ;
				default: ERROR("INVALI PIECE TO PRINT");
			}
		}
		printf("\n");
		for (int x = 0; x < 4 * WIDTH + 3; x++)
		{
			printf("_");
		}
		printf("\n");
	}
	printf("\n");
	fflush(stdout);
	//usleep(2000000);
}
