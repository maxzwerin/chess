#include <stdio.h>

extern struct Piece board[8][8];
extern int en_passant_row;
extern int en_passant_col;

int is_valid_move_pawn(int start_row, int start_col, int end_row, int end_col) {
    struct Piece pawn = board[start_row][start_col];
    
    int direction;
    if (pawn.color == WHITE) direction = -1;
    else if (pawn.color == BLACK) direction = 1;
    
    // ----- single forward move -----
    if (end_col == start_col && end_row == start_row + direction) {
        if (board[end_row][end_col].type == NONE)
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
                printf("en passant target set at (%d, %d)\n", en_passant_row, en_passant_col);
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


int is_valid_move_king(int start_row, int start_col, int end_row, int end_col) {
    int row_diff = abs(end_row - start_row);
    int col_diff = abs(end_col - start_col);
    
    if (row_diff <= 1 && col_diff <= 1)
        return 1;
    
    return 0;
}

