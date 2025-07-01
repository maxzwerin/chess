#include "FPToolkit.h"
#include "M2d_matrix_tools.h"
#include "piece.h"
#include "board.h"
#include "sprites.h"
#include "valid_moves.h"
#include "perlin.h"

struct Piece board[8][8];

enum Color current_turn = WHITE; 

// 0 = white bottom, 1 = black bottom
int flipped = 0;

// perlin noise globals
double t = 0;
int seed;

void init_chess_board() {
    double move = 0;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            board[row][col].type = NONE;
        }
    }
    // Black pieces (top of board)
    // Row 0: Rook, Knight, Bishop, Queen, King, Bishop, Knight, Rook.
    board[0][0] = (struct Piece){ROOK,   BLACK, 0, 0, move};
    board[0][1] = (struct Piece){KNIGHT, BLACK, 1, 0, move};
    board[0][2] = (struct Piece){BISHOP, BLACK, 2, 0, move};
    board[0][3] = (struct Piece){QUEEN,  BLACK, 3, 0, move};
    board[0][4] = (struct Piece){KING,   BLACK, 4, 0, move};
    board[0][5] = (struct Piece){BISHOP, BLACK, 5, 0, move};
    board[0][6] = (struct Piece){KNIGHT, BLACK, 6, 0, move};
    board[0][7] = (struct Piece){ROOK,   BLACK, 7, 0, move};
 
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
    board[7][0] = (struct Piece){ROOK,   WHITE, 0, 7, move};
    board[7][1] = (struct Piece){KNIGHT, WHITE, 1, 7, move};
    board[7][2] = (struct Piece){BISHOP, WHITE, 2, 7, move};
    board[7][3] = (struct Piece){QUEEN,  WHITE, 3, 7, move};
    board[7][4] = (struct Piece){KING,   WHITE, 4, 7, move};
    board[7][5] = (struct Piece){BISHOP, WHITE, 5, 7, move};
    board[7][6] = (struct Piece){KNIGHT, WHITE, 6, 7, move};
    board[7][7] = (struct Piece){ROOK,   WHITE, 7, 7, move};
}

void clear_position(int row, int col) {
    board[row][col] = (struct Piece){NONE, WHITE, row, col, 0};
}

void make_move(int old_row, int old_col, int new_row, int new_col, struct Piece piece) {
    // ensure validity (for later when AI is implimented)
    printf("\n\nmove being made:\n");
    printf("+++ [%d, %d]\n", new_row, new_col);
    board[new_row][new_col] = piece;  // moves piece to new position
    printf("--- [%d, %d]\n", old_row, old_col);
    clear_position(old_row, old_col); // clears old position
}

void drag_piece(int object, double *nx, double *ny, 
                double old_board_x, double old_board_y, 
                struct Piece piece) 
{
    int p[2];

    do {

        S_mouse_coord_window(p);
        p[1] = WINDOW_SIZE - p[1];

        draw_board();
        draw_all_pieces();
        draw_object(object, p[0], p[1], piece.color);
        t += 0.001;

    } while (S_mouse_coord_window(p) == 1);

    if (click_in_board(p)) {
        get_board_coords(p, nx, ny);
        *ny = (int)(7 - *ny);
    } else {
        *nx = old_board_x;
        *ny = old_board_y;
    }

    draw_board();
    draw_all_pieces();
}

void process_piece_drag(){
    int p[2];
    int mouse_state = S_mouse_coord_window(p);
    p[1] = WINDOW_SIZE - p[1];

    if (mouse_state == 1 && click_in_board(p)) {

        double board_x, board_y;
        get_board_coords(p, &board_x, &board_y);
        int col = (int) board_x;
        int row = (int) board_y;

        if (board[row][col].type != NONE && board[row][col].color != current_turn) {
            return;
        }

        if (board[row][col].type != NONE) {
            struct Piece moving_piece = board[row][col];
            board[row][col].is_moving = 1;

            int old_row = row;
            int old_col = col;

            int sprite_index = moving_piece.type - 1;

            drag_piece(sprite_index, &moving_piece.x, &moving_piece.y, 
                       old_col, old_row, moving_piece);

            int new_col = (int) moving_piece.x;
            int new_row = (int) moving_piece.y;

            if (is_valid_move(old_row, old_col, new_row, new_col, moving_piece)) {

                make_move(old_row, old_col, new_row, new_col, moving_piece);

                current_turn = (current_turn == WHITE) ? BLACK : WHITE;
            } else if (old_row == new_row && old_col == new_col) {
                printf("piece is selected\n");
                board[row][col].is_moving = 2; // piece is selected
                // display valid moves for selected piece
                // if next click is on a valid square, move the piece
                // if the next click is on a different piece,
                // then select that new piece. if the next click is
                // on an invalid square, stop showing the valid squares
                // for said piece
            } else {
                board[old_row][old_col] = moving_piece;
                board[row][col].is_moving = 0;
            }

            // adding to t before and after allows for a smooth
            // animation when placing a piece on the board
            t += 0.001;
            draw_board();
            draw_all_pieces();
            G_display_image();
            t += 0.001;
        }

    } else if (mouse_state == 1) {

        if (p[0] > WINDOW_SIZE - 20 && p[1] > WINDOW_SIZE - 20) exit(0); 
        draw_board();
        draw_all_pieces();
    }
}


void init_functions() {
    // graphical 
    initialize_rand_pts(); 
    // sprite stuff 
    read_all_objects(); 
    scale_all_objects();
    // piece initializer
    init_chess_board();
}

void update() {
    process_piece_drag();
    draw_board();
    draw_all_pieces();
    G_display_image();
    t += 0.001;
    usleep(5000);
}


int main() {
    srand(time(NULL));
    seed = rand() % 10000;

    G_init_graphics(WINDOW_SIZE, WINDOW_SIZE);
    init_functions();

    draw_board();
    perlin();
    draw_all_pieces();

    while (1) {
        if (G_key_down('q') == 'q') exit(0);
        if (G_key_press() == 'f') flipped = !flipped;
        update();
    }

    return 0;
}
