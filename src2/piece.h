#ifndef PIECE_H_
#define PIECE_H_

enum Type {
    NONE = 0,
    PAWN = 1,
    ROOK = 2,
    BISHOP = 3,
    KNIGHT = 4,
    QUEEN = 5,
    KING = 6,
};

enum Color {
    WHITE = 0,
    BLACK = 1,
};

struct Piece {
    enum Type type;
    enum Color color;
    double x, y;
    double is_moving;
};

extern struct Piece board[8][8];

#endif // PIECE_H_

