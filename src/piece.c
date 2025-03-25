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

int get_sprite_index(struct Piece p)
// return sprite index based on piece type 
{
    switch (p.type) {
        case PAWN:
            return 0;  
        case ROOK:
            return 1; 
        case BISHOP:
            return 2;
        case KNIGHT:
            return 3;
        case QUEEN:
            return 4;
        case KING:
            return 5;
        default:
            return -1;
    }
}

const char* get_piece_symbol(enum Type type, enum Color color) 
{
    if (type == NONE) return ".";
    if (color == WHITE) {
        switch(type) {
            case PAWN:    return "P";
            case ROOK:    return "R";
            case BISHOP:  return "B";
            case KNIGHT:  return "N";  
            case QUEEN:   return "Q";
            case KING:    return "K";
            default:      return "?";
        }
    } else {
        switch(type) {
            case PAWN:    return "p";
            case ROOK:    return "r";
            case BISHOP:  return "b";
            case KNIGHT:  return "n";
            case QUEEN:   return "q";
            case KING:    return "k";
            default:      return "?";
        }
    }
}
