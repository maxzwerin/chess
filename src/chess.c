#include "../FPToolkit.c"
#include "../M2d_matrix_tools.c"
#include "piece.c"
#include "board.c"
#include "valid_moves.c"
#include "sprites.c"

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

void draw_object(int object, double pos_x, double pos_y, enum Color piece_color) 
{
    int h, i, j, np;
    double xp[100], yp[100];
    double tx, ty;
    double center = cell_size / 2;

    if (pos_x < 8 && pos_y < 8) {
        tx = offset + pos_x * cell_size + center;
        ty = offset + ((MODULES - 1) - pos_y) * cell_size + center;
    } else {
        tx = pos_x;
        ty = pos_y;
    }

    double c[2];
    if      (piece_color == WHITE) { c[0] = 1; c[1] = 0; } 
    else if (piece_color == BLACK) { c[0] = 0; c[1] = 1; }

    for (i = 0; i < numpolys[object]; i++) {
        np = psize[object][i];
        for (j = 0; j < np; j++) {
            h = con[object][i][j];
            xp[j] = x[object][h] + tx;
            yp[j] = y[object][h] + ty;
        }

        G_rgb(c[0], c[0], c[0]);
        G_fill_polygon(xp, yp, np);
        G_rgb(c[1], c[1], c[1]);
        G_polygon(xp, yp, np);
    }
    G_display_image();
}

void draw_shadow(int object, double pos_x, double pos_y, enum Color piece_color) 
{
    int h, i, j, np;
    double xp[100], yp[100];
    double tx, ty;
    double center = cell_size / 2;

    tx = offset + pos_x * cell_size + center;
    ty = offset + ((MODULES - 1) - pos_y) * cell_size + center;

    single_box_shadow(tx - center, ty - center);

    double c[2];
    if      (piece_color == WHITE) { c[0] = 0.6; c[1] = 0.4; } 
    else if (piece_color == BLACK) { c[0] = 0.4; c[1] = 0.6; }

    for (i = 0; i < numpolys[object]; i++) {
        np = psize[object][i];
        for (j = 0; j < np; j++) {
            h = con[object][i][j];
            xp[j] = x[object][h] + tx;
            yp[j] = y[object][h] + ty;
        }

        G_rgb(c[0], c[0], c[0]);
        G_fill_polygon(xp, yp, np);
        G_rgb(c[1], c[1], c[1]);
        G_polygon(xp, yp, np);
    }
    G_display_image();
}

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

void draw_all_pieces() 
{
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {

            if (board[row][col].type != NONE) {
                int sprite_index = get_sprite_index(board[row][col]);
                if (board[row][col].is_moving == 0) {
                    draw_object(sprite_index, col, row, board[row][col].color);
                } else {
                    draw_shadow(sprite_index, col, row, board[row][col].color);
                }
            }

        }
    }
}



int is_valid_move(int start_row, int start_col, int end_row, int end_col, struct Piece piece) 
{
    if (piece.type == NONE) return 0;
    if (board[end_row][end_col].type != NONE && piece.color == board[end_row][end_col].color) return 0;
    if (start_row == end_row && start_col == end_col) return 0;

    switch (piece.type) {
        case PAWN:
            return is_valid_move_pawn(start_row, start_col, end_row, end_col);
        case ROOK:
            return is_valid_move_rook(start_row, start_col, end_row, end_col);
        case BISHOP:
            return is_valid_move_bishop(start_row, start_col, end_row, end_col);
        case KNIGHT:
            return is_valid_move_knight(start_row, start_col, end_row, end_col);
        case QUEEN:
            return is_valid_move_queen(start_row, start_col, end_row, end_col);
        case KING:
            return is_valid_move_king(start_row, start_col, end_row, end_col);
        default:
            return 0;
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

            printf("attempting to move to (%d, %d)\n", new_row, new_col);

            if (is_valid_move(old_row, old_col, new_row, new_col, moving_piece)) {
                board[new_row][new_col] = moving_piece;
                clear_position(row, col); // clear old position

                // if en passant occurred, clear captured pawn
                if (en_passant_row == -99) clear_position(old_row, new_col);
                if (moving_piece.type != PAWN) { en_passant_row = -1; en_passant_col = -1; }

                // cannot castle if king moves 
                if (moving_piece.type == KING) revoke_castle(current_turn);
                if (moving_piece.type == ROOK) {
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
