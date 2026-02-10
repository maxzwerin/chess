#include "movegen.c"

const int pawnValue = 100;
const int knightValue = 300;
const int bishopValue = 300;
const int rookValue = 500;
const int queenValue = 900;

int pieceCount(Board board, int color, int piece) {
    int count = 0;
    U64 pieceBB = 0ULL;
    switch (piece) {
        case PAWN:   pieceBB = board.pawnBB; break;
        case KNIGHT: pieceBB = board.knightBB; break;
        case BISHOP: pieceBB = board.bishopBB; break;
        case ROOK:   pieceBB = board.rookBB; break;
        case QUEEN:  pieceBB = board.queenBB; break;
        default: return 0;
    }

    pieceBB &= color ? board.whiteBB : board.blackBB;

    while (pieceBB) {
        pieceBB &= pieceBB - 1;
        count++;
    }
    
    return count;
}

int countMaterial(Board board, int color) {
    int material = 0;
    material += pieceCount(board, color, PAWN) * pawnValue;
    material += pieceCount(board, color, KNIGHT) * knightValue;
    material += pieceCount(board, color, BISHOP) * bishopValue;
    material += pieceCount(board, color, ROOK) * rookValue;
    material += pieceCount(board, color, QUEEN) * queenValue;
    return material;
}

int evaluate(Board board) {
    int whiteEval = countMaterial(board, WHITE);
    int blackEval = countMaterial(board, BLACK);

    int evaluation = whiteEval - blackEval;
    int perspective = board.turn ? 1 : -1;
    return evaluation * perspective;
}

int search(Board board, int depth) {
    if (depth == 0) {
        return evaluate(board);
    }

    Move moves;
    int moveCount = legalMoves(&board, &moves);

    if (moveCount == 0) {
        if (0) {
            return -999999;
        }
        return 0;
    }

    int bestEval = -999999;

    for (;;) {
        // makeMove(board, move);
        int eval = -search(board, depth - 1);
        bestEval = eval > bestEval ? eval : bestEval;
        // unmakeMove(board, move);
    }
    return bestEval;
}
