#ifndef VALID_MOVES_H_
#define VALID_MOVES_H_

#include "piece.h"

int is_valid_move(int start_row, int start_col, int end_row, int end_col, struct Piece piece);
int is_valid_move_pawn(int start_row, int start_col, int end_row, int end_col);
int is_valid_move_rook(int start_row, int start_col, int end_row, int end_col);
int is_valid_move_bishop(int start_row, int start_col, int end_row, int end_col);
int is_valid_move_knight(int start_row, int start_col, int end_row, int end_col);
int is_valid_move_queen(int start_row, int start_col, int end_row, int end_col);
int is_valid_move_king(int start_row, int start_col, int end_row, int end_col);

#endif // VALID_MOVES_H_
