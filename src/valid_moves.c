#include "valid_moves.h"

int is_valid_move(int start_row, int start_col, int end_row, int end_col, struct Piece piece) 
{
    if (piece.type == NONE) return 0;
    if (board[end_row][end_col].type != NONE && piece.color == board[end_row][end_col].color) return 0;
    if (start_row == end_row && start_col == end_col) return 0;

    switch (piece.type) {
        case PAWN:   return is_valid_move_pawn(start_row, start_col, end_row, end_col);
        case ROOK:   return is_valid_move_rook(start_row, start_col, end_row, end_col);
        case BISHOP: return is_valid_move_bishop(start_row, start_col, end_row, end_col);
        case KNIGHT: return is_valid_move_knight(start_row, start_col, end_row, end_col);
        case QUEEN:  return is_valid_move_queen(start_row, start_col, end_row, end_col);
        case KING:   return is_valid_move_king(start_row, start_col, end_row, end_col);
        default:     return 0;
    }
}

int is_valid_move_pawn(int start_row, int start_col, int end_row, int end_col) {
    return 1;
}


int is_valid_move_rook(int start_row, int start_col, int end_row, int end_col) {
    return 1;
}


int is_valid_move_bishop(int start_row, int start_col, int end_row, int end_col) {
    return 1;
}


int is_valid_move_knight(int start_row, int start_col, int end_row, int end_col) {
    return 1;
}


int is_valid_move_queen(int start_row, int start_col, int end_row, int end_col) {
    return (is_valid_move_rook(start_row, start_col, end_row, end_col) + 
            is_valid_move_bishop(start_row, start_col, end_row, end_col));
}


int can_castle_left() {
    return 1;
}

int can_castle_right() {
    return 1;
}

int is_valid_move_king(int start_row, int start_col, int end_row, int end_col) {
    return 1;
}

