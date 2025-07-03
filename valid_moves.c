#include <stdio.h>

extern struct Piece board[8][8];
extern int en_passant_row;
extern int en_passant_col;
extern int white_castle[3];
extern int black_castle[3];

extern int white_has_castled_short;
extern int black_has_castled_short;
extern int white_has_castled_long;
extern int black_has_castled_long;
extern enum Color current_turn;

int is_valid_move_pawn(int start_row, int start_col, int end_row, int end_col);
int is_valid_move_rook(int start_row, int start_col, int end_row, int end_col);
int is_valid_move_bishop(int start_row, int start_col, int end_row, int end_col);
int is_valid_move_knight(int start_row, int start_col, int end_row, int end_col);
int is_valid_move_queen(int start_row, int start_col, int end_row, int end_col);
int is_valid_move_king(int start_row, int start_col, int end_row, int end_col);


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
    struct Piece pawn = board[start_row][start_col];

    int direction;
    if (pawn.color == WHITE) direction = -1;
    else if (pawn.color == BLACK) direction = 1;
 
    // ----- single forward move -----
    if (end_col == start_col && end_row == start_row + direction) {
        if (board[end_row][end_col].type == NONE)
            en_passant_row = -1;
            en_passant_col = -1;
            return 1;
    }
 
    // ----- double forward move -----
    if (end_col == start_col && end_row == start_row + 2 * direction) {
        if ((pawn.color == WHITE && start_row == 6) || (pawn.color == BLACK && start_row == 1)) {
            // check that both the intermediate square and destination square are empty.
            if (board[start_row + direction][start_col].type == NONE &&
                board[end_row][end_col].type == NONE) {
                // set en passant target to the square the pawn passed over.
                en_passant_row = start_row + direction;
                en_passant_col = start_col;
                return 1;
            }
        }
    }
 
    // ----- diagonal capture move -----
    if (abs(end_col - start_col) == 1 && end_row == start_row + direction) {
        if (board[end_row][end_col].type != NONE && 
            board[end_row][end_col].color != pawn.color)
            return 1;
    }

    // ----- en passant capture -----
    if (end_row == en_passant_row && end_col == en_passant_col) {
        en_passant_row = -99;
        en_passant_col = -99;
        return 1;
    }

    return 0;
}


int is_valid_move_rook(int start_row, int start_col, int end_row, int end_col) {
    if (start_row != end_row && start_col != end_col) return 0;
 
    if (start_row == end_row) {
        int min_col = start_col < end_col ? start_col : end_col;
        int max_col = start_col > end_col ? start_col : end_col;

        for (int col = min_col + 1; col < max_col; col++) {
            if (board[start_row][col].type != NONE) return 0;
        }
    } else {
        int min_row = start_row < end_row ? start_row : end_row;
        int max_row = start_row > end_row ? start_row : end_row;

        for (int row = min_row + 1; row < max_row; row++) {
            if (board[row][start_col].type != NONE) return 0;
        }
    }
 
    return 1;
}


int is_valid_move_bishop(int start_row, int start_col, int end_row, int end_col) {
    int delta_row = end_row - start_row;
    int delta_col = end_col - start_col;
 
    if (abs(delta_row) != abs(delta_col))
        return 0;
 
    int step_row = (delta_row > 0) ? 1 : -1;
    int step_col = (delta_col > 0) ? 1 : -1;
 
    int current_row = start_row + step_row;
    int current_col = start_col + step_col;
    while (current_row != end_row && current_col != end_col) {
        if (board[current_row][current_col].type != NONE) return 0;
        current_row += step_row;
        current_col += step_col;
    }
 
    return 1;
}


int is_valid_move_knight(int start_row, int start_col, int end_row, int end_col) {
    int row_diff = abs(end_row - start_row);
    int col_diff = abs(end_col - start_col);
 
    if ((row_diff == 2 && col_diff == 1) || (row_diff == 1 && col_diff == 2)) return 1;
 
    return 0;
}


int is_valid_move_queen(int start_row, int start_col, int end_row, int end_col) {
    return (is_valid_move_rook(start_row, start_col, end_row, end_col) + 
            is_valid_move_bishop(start_row, start_col, end_row, end_col));
}


int can_castle_left() {
    if (current_turn == WHITE && white_castle[0] && white_castle[1]) { 
        white_has_castled_long = 1; 
        return 1; 
    }
    if (current_turn == BLACK && black_castle[0] && black_castle[1]) {
        black_has_castled_long = 1; 
        return 1; 
    }
    return 0;
}

int can_castle_right() {
    if (current_turn == WHITE && white_castle[1] && white_castle[2]) { 
        white_has_castled_short = 1; 
        return 1; 
    }
    if (current_turn == BLACK && black_castle[1] && black_castle[2]) {
        black_has_castled_short = 1; 
        return 1; 
    }
    return 0;
}

int is_valid_move_king(int start_row, int start_col, int end_row, int end_col) {
    int row_diff = abs(end_row - start_row);
    int col_diff = abs(end_col - start_col);


    if (row_diff <= 1 && col_diff <= 1)
        return 1;

    // ----- castling left -----
    if ((end_row == 7 || end_row == 0) && end_col == 2) {
        if (board[end_row][1].type != NONE ||
            board[end_row][2].type != NONE ||
            board[end_row][3].type != NONE ) return 0;

        if (can_castle_left()) { 
            return 1;
        }
    }

    // ----- castling right -----
    if ((end_row == 7 || end_row == 0) && end_col == 6) {
        if (board[end_row][5].type != NONE ||
            board[end_row][6].type != NONE ) return 0;

        if (can_castle_right()) {
            return 1;
        }
    }

    return 0;
}

