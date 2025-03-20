#include <stdio.h>

enum Type {
    NONE = 0,
    PAWN,
    ROOK,
    BISHOP,
    KNIGHT,
    QUEEN,
    KING,
};

enum Color {
    BLACK = 1,
    WHITE,
};

struct Piece {
    enum Type type;
    enum Color color;
    double x, y;
    double is_moving;
    double has_moved;
};

struct Piece board[8][8];

extern int en_passant_row;
extern int en_passant_col;
