#ifndef ENGINE_H
# define ENGINE_H

# include <stdint.h>
# include "types.h"

// for legal move generation--
#define KING_VECS		{{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}}
#define QUEEN_VECS		KING_VECS
#define BISHOP_VECS		{{1, 1}, {-1, 1}, {-1, -1}, {1, -1}}
#define KNIGHT_VECS		{{-2, -1}, {-2, 1}, {2, -1}, {2, 1}, {-1, -2}, {1, -2}, {-1, 2}, {1, 2}}
#define ROOK_VECS		{{0, 1}, {-1, 0}, {0, -1}, {1, 0}}
#define PAWN_VECS_WHITE	{{},}
#define PAWN_VECS_BLACK {{},}
//-----------------------------



#endif //ENGINE_H
