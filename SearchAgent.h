#ifndef SEARCHAGENT_H
#define SEARCHAGENT_H

#include "Board.h"
#include "Evaluator.h"
#include "Move.h"
#include <vector>

class SearchAgent {
public:
	SearchAgent(Board b) : board(b), evaluator(board), bestMoves(MAX_PLY), checkedNodes(0), ply(0) {}

	Move GetBestMove(int outputIndicator = 0);

private:
	Board board;
	Evaluator evaluator;
	std::vector<Move> bestMoves;
	int checkedNodes;
	int ply;

	static const int MAX_PLY = 50;
	static const int MIN_SCORE = -30000;
	static const int MATE_SCORE = -10000;
	static const int MAX_SCORE = +30000;
	static const int ILLEGAL = 30000;

	int AlphaBeta(int depth, int alpha, int beta);
};

#endif /* SEARCHAGENT_H */

