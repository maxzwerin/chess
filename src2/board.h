#ifndef BOARD_H_
#define BOARD_H_

#include "piece.h" // for enum Color
#include "sprites.h" // for drawing objects

#define MODULES 8
#define WINDOW_SIZE 800

#define B_PTS 100000
#define S_PTS 1000
#define L_PTS 500000

extern const double offset;
extern const double cell_size;

extern double b_pts[B_PTS][2];
extern double s_pts[S_PTS][2];
extern double l_pts[L_PTS][2];

void initialize_rand_pts();

void background();

void perlin();

void single_box(int x, int y);

void single_box_shadow(int x, int y);

void draw_object(int object, double pos_x, double pos_y, struct Piece piece);

void draw_all_pieces();

void animate_flip();

void grid_lines();

void labels();

void grid_squares();

int click_in_board(int p[2]);

void get_board_coords(int p[2], double *x, double *y);

void draw_board();

#endif // BOARD_H_

