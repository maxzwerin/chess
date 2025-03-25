#include "../FPToolkit.c"
#include "../M2d_matrix_tools.c"
#include "piece.c"
#include "sprites.c"
#include "board.c"
#include "valid_moves.c"

#define MODULES 8
#define WINDOW_SIZE 800

// defined in board.c
extern const double offset;      // 100.0
extern const double cell_size;   // 50.00

// track en passant target square
// reset each turn.
int en_passant_row = -1;
int en_passant_col = -1;

// track castling possibilities
// { LEFT ROOK, KING, RIGHT ROOK }
// 1 = piece has not moved
// 0 = piece HAS moved
int white_castle[3] = {1, 1, 1};
int black_castle[3] = {1, 1, 1};

int white_has_castled_short = 0;
int black_has_castled_short = 0;
int white_has_castled_long = 0;
int black_has_castled_long = 0;

enum Color current_turn = WHITE; 

struct Piece board[8][8];

void init_chess_board() 
{
    double move = 0;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            board[row][col].type = NONE;
        }
    }
    // Black pieces (top of board)
    // Row 0: Rook, Knight, Bishop, Queen, King, Bishop, Knight, Rook.
    board[0][0] = (struct Piece){ROOK,   BLACK, 0, 0, move, move};
    board[0][1] = (struct Piece){KNIGHT, BLACK, 1, 0, move, move};
    board[0][2] = (struct Piece){BISHOP, BLACK, 2, 0, move, move};
    board[0][3] = (struct Piece){QUEEN,  BLACK, 3, 0, move, move};
    board[0][4] = (struct Piece){KING,   BLACK, 4, 0, move, move};
    board[0][5] = (struct Piece){BISHOP, BLACK, 5, 0, move, move};
    board[0][6] = (struct Piece){KNIGHT, BLACK, 6, 0, move, move};
    board[0][7] = (struct Piece){ROOK,   BLACK, 7, 0, move, move};
 
    // Row 1: Black pawns.
    for (int col = 0; col < 8; col++) {
        board[1][col] = (struct Piece){PAWN, BLACK, col, 1};
    }
 
    // White pieces (bottom of board)
    // Row 6: White pawns.
    for (int col = 0; col < 8; col++) {
        board[6][col] = (struct Piece){PAWN, WHITE, col, 6};
    }
    // Row 7: Rook, Knight, Bishop, Queen, King, Bishop, Knight, Rook.
    board[7][0] = (struct Piece){ROOK,   WHITE, 0, 7, move, move};
    board[7][1] = (struct Piece){KNIGHT, WHITE, 1, 7, move, move};
    board[7][2] = (struct Piece){BISHOP, WHITE, 2, 7, move, move};
    board[7][3] = (struct Piece){QUEEN,  WHITE, 3, 7, move, move};
    board[7][4] = (struct Piece){KING,   WHITE, 4, 7, move, move};
    board[7][5] = (struct Piece){BISHOP, WHITE, 5, 7, move, move};
    board[7][6] = (struct Piece){KNIGHT, WHITE, 6, 7, move, move};
    board[7][7] = (struct Piece){ROOK,   WHITE, 7, 7, move, move};
}

void clear_position(int row, int col) {
    board[row][col] = (struct Piece){NONE, WHITE, row, col, 0, 0};
}

void revoke_white_castle() {
    white_castle[0] = 0;
    white_castle[1] = 0;
    white_castle[2] = 0;
}
void revoke_black_castle() {
    black_castle[0] = 0;
    black_castle[1] = 0;
    black_castle[2] = 0;
}
void revoke_castle(enum Color color) {
    if (color == WHITE) revoke_white_castle();
    else if (color == BLACK) revoke_black_castle();
}
void revoke_white_short_castle() {
    white_castle[0] = 0;
    if (!white_castle[0] && !white_castle[2]) revoke_castle(WHITE);
}
void revoke_black_short_castle() {
    black_castle[2] = 0;
    if (!black_castle[0] && !black_castle[2]) revoke_castle(BLACK);
}
void revoke_white_long_castle() {
    white_castle[2] = 0;
    if (!white_castle[0] && !white_castle[2]) revoke_castle(WHITE);
}
void revoke_black_long_castle() {
    black_castle[0] = 0;
    if (!black_castle[0] && !black_castle[2]) revoke_castle(BLACK);
}
void revoke_castle_left(enum Color color) {
    if (color == WHITE) revoke_white_short_castle();
    else if (color == BLACK) revoke_black_long_castle();
}
void revoke_castle_right(enum Color color) {
    if (color == WHITE) revoke_black_short_castle();
    else if (color == BLACK) revoke_white_long_castle();
}


void make_move(int old_row, int old_col, int new_row, int new_col, struct Piece piece) {
    // ensure validity (for later when AI is implimented)
    board[new_row][new_col] = piece;  // moves piece to new position
    clear_position(old_row, old_col); // clears old position
}

void move_castled_rook() {
    if (current_turn == WHITE) {
        if (white_has_castled_short) {
            board[7][5] = board[7][7];
            clear_position(7,7);
            white_has_castled_short = 0;
        } else if (white_has_castled_long) {
            board[7][3] = board[7][0];
            clear_position(7,0);
            white_has_castled_long = 0; 
        }
    } else if (current_turn == BLACK) {
        if (black_has_castled_short) {
            board[0][5] = board[0][7];
            clear_position(0,7);
            black_has_castled_short = 0;
        } else if (black_has_castled_long) {
            board[0][3] = board[0][0];
            clear_position(0,0);
            black_has_castled_long = 0; 
        }
    }
}

void drag_piece(int object, double *nx, double *ny, double old_board_x, double old_board_y, struct Piece piece) 
{
    int p[2];

    do {
        S_mouse_coord_window(p);
        p[1] = WINDOW_SIZE - p[1];

        draw_board();
        draw_all_pieces();
        draw_object(object, p[0], p[1], piece.color);

    } while (S_mouse_coord_window(p) == 1);

    if (click_in_board(p)) {
        get_board_coords(p, nx, ny);
        *nx = (int)*nx;
        *ny = (int) fabs(*ny - 7);
    } else {
        *nx = old_board_x;
        *ny = old_board_y;
    }

    draw_board();
    draw_all_pieces();
}

void process_piece_drag() {
    int p[2];
    int mouse_state = S_mouse_coord_window(p);
    p[1] = WINDOW_SIZE - p[1];

    if (mouse_state == 1 && click_in_board(p)) {

        double board_x, board_y;
        get_board_coords(p, &board_x, &board_y);
        int col = (int) board_x;
        int row = (int) board_y;

        if (board[row][col].type != NONE && board[row][col].color != current_turn) {
            usleep(5000);
            return;
        }

        if (board[row][col].type != NONE) {
            struct Piece moving_piece = board[row][col];
            board[row][col].is_moving = 1;

            int old_row = row;
            int old_col = col;

            int sprite_index = get_sprite_index(moving_piece);
            //print_board();

            drag_piece(sprite_index, &moving_piece.x, &moving_piece.y, old_col, old_row, moving_piece);

            int new_col = (int) moving_piece.x;
            int new_row = (int) moving_piece.y;

            if (is_valid_move(old_row, old_col, new_row, new_col, moving_piece)) {

                make_move(old_row, old_col, new_row, new_col, moving_piece);

                // if en passant occurred, clear captured pawn
                if (en_passant_row == -99) clear_position(old_row, new_col);
                if (moving_piece.type != PAWN) { en_passant_row = -1; en_passant_col = -1; }

                // cannot castle if king moves 
                if (moving_piece.type == KING) { 
                    revoke_castle(current_turn);
                    if (white_has_castled_long || white_has_castled_short) move_castled_rook(); 
                    if (black_has_castled_long || black_has_castled_short) move_castled_rook();
                } else if (moving_piece.type == ROOK) {
                    if (old_col == 0) revoke_castle_left(current_turn);
                    else if (old_col == 7) revoke_castle_right(current_turn);
                }


                current_turn = (current_turn == WHITE) ? BLACK : WHITE;
            } else {
                board[old_row][old_col] = moving_piece;
                board[row][col].is_moving = 0;
            }

            draw_board();
            draw_all_pieces();
            G_display_image();
        }

    } else if (mouse_state == 1) {

       if (p[0] > WINDOW_SIZE - 20 && p[1] > WINDOW_SIZE - 20) exit(0); 

    }
}


void init_functions() 
{
    // graphical 
    initialize_rand_pts(); 
    // sprite stuff 
    read_all_objects(); 
    scale_all_objects();
    // piece initializer
    init_chess_board();
}


int main() {
    G_init_graphics(WINDOW_SIZE, WINDOW_SIZE);
    init_functions();

    draw_board();
    draw_all_pieces();

    while (1) {
        process_piece_drag();
        usleep(5000);
    }

    return 0;
}
