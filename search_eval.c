#include <stdio.h>

int Evaluate() {

	while(pieces) {
		piece = IDXtoU64(U64toIDX(pieces));
		moves = (*f)(piece, all_pieces, side);
		if(moves) {
			if(side & bd->WhitePieces) {
				bd->WhiteAttacking |= moves;
			} else {
				bd->BlackAttacking |= moves;
			}
		} 
	






}

int AlphaBeta(int depth, int alpha, int beta) {

	if (depth == 0)
		return Evaluate();

	GenerateLegalMoves();

	while (MovesLeft()) {
		MakeNextMove();
		val = -AlphaBeta(depth - 1, -beta, -alpha);
		UnmakeMove();
		if (val >= beta)
			return beta;
		if (val > alpha)
			alpha = val;
	}
	return alpha;
}
