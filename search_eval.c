#include <stdio.h>

int Evaluate() {
	int score = 0;
	if(bd->to_move == WHITE) {
		U64 w_pieces = bd->WhitePieces;
		while(w_pieces) {
			U64 w_piece = IDXtoU64(U64toIDX(w_pieces));
			if(w_piece & bd->WhitePawns)
				score += 1;
			else if(w_piece & bd->WhiteKnights)
				score += 3;
			else if(w_piece & bd->WhiteBishops)
				score += 3;
			else if(w_piece & bd->WhiteRooks)
				score += 5;
			else if(w_piece & bd->WhiteQueens)
				score += 9;

			Pop(w_pieces);
		}
	} else (bd->to_move == BLACK) {
		U64 b_pieces = bd->BlackPieces;
		while(b_pieces) {
			U64 b_piece = IDXtoU64(U64toIDX(b_pieces));
			if(b_piece & bd->BlackPawns)
				score += 1;
			else if(b_piece & bd->BlackKnights)
				score += 3;
			else if(w_piece & bd->BlackBishops)
				score += 3;
			else if(w_piece & bd->BlackRooks)
				score += 5;
			else if(w_piece & bd->BlackQueens)
				score += 9;

			Pop(b_pieces);
		}
	}

	return score;
}
int AlphaBeta(int depth, int alpha, int beta) {
	struct Move_list *mv_list;
	mv_list = (struct Move_list *)malloc(sizeof(struct Move_list));
	mv_list->total_count = 0;

	if (depth == 0) {
		free(mv_list);
		return Evaluate();
	}

	generateAllMoves(mv_list);
	for (int i = 0; i < mv_list->total_count; i++) {
		Mv mv = mv_list->moves[i].mv;
		if(!make_move(mv_list->moves[i])) {
			continue;
		}
		int val = -AlphaBeta(depth - 1, -beta, -alpha);

		undo_move(bd, mv_list->moves[i].undo);
		
		if (val >= beta)
			return beta;
		if (val > alpha)
			alpha = val;
	}
	free(mv_list->moves[0].undo);
	free(mv_list);
	return alpha;
}
