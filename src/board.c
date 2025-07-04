#include "piece.h"
#include "board.h"
#include "sprites.h"
#include "FPToolkit.h"
#include "perlin.h"
#include <stdlib.h> // drand48, atoi
#include <unistd.h> // usleep

const double offset = 100;
const double cell_size = (WINDOW_SIZE - 2 * offset) / MODULES;
extern double t;
extern int seed;
extern int flipped;

// ----- rand pts ----- //
double b_pts[B_PTS][2];
double s_pts[S_PTS][2];
double l_pts[L_PTS][2];


// ----- display ----- //
void initialize_rand_pts() {
    for (int i = 0; i < B_PTS; i++) {
        b_pts[i][0] = drand48() * WINDOW_SIZE;
        b_pts[i][1] = drand48() * WINDOW_SIZE;
    }
 
    for (int i = 0; i < S_PTS; i++) {
        s_pts[i][0] = drand48() * cell_size;
        s_pts[i][1] = drand48() * cell_size;
    }
 
    for (int i = 0; i < L_PTS; i++) {
        l_pts[i][0] = drand48();
        l_pts[i][1] = drand48();
    }
}

void background() {
    G_rgb(0.1, 0.2, 0.0);
    G_clear();
    G_rgb(0.1, 0.3, 0.0);
    for (int i = 0; i < B_PTS; i++) {
        G_point(b_pts[i][0], b_pts[i][1]);
    }
}

void perlin() {
    int step = 1;
    double nx, ny, e, m;
    double fill = offset - 10;

    for (int y = 0; y < WINDOW_SIZE; y += step) {
        for (int x = 0; x < WINDOW_SIZE; x += step) {
            if (x > fill && x < WINDOW_SIZE - fill && 
                y > fill && y < WINDOW_SIZE - fill) continue;

            nx = (double)x / WINDOW_SIZE * GRID_SIZE;
            ny = (double)y / WINDOW_SIZE * GRID_SIZE;
            e = FBM(nx + t, ny + t, t + t, OCTAVES, PERSISTENCE, LACUNARITY, seed);

            e = fabs(e);

            int r, g, b;
            if (e < 0.2) {
                Gi_rgb(11, 60, 11);
            } else if (e < 0.4) {
                Gi_rgb(26, 76, 26);
            } else if (e < 0.6) {
                Gi_rgb(32, 86, 32);
            } else {
                Gi_rgb(42, 106, 42);
            }

            G_point(x,y);
        }
    }
}

void single_box(int x, int y) {
    if ((x + y) % 2 == 0) Gi_rgb(165,117,80);
    else                  Gi_rgb(235,209,166);

    G_fill_rectangle(x, y, cell_size, cell_size);
}

void single_box_shadow(int x, int y) {
    Gi_rgb(112,134,87);

    G_fill_rectangle(x, y, cell_size, cell_size);
}


void draw_object(int object, double pos_x, double pos_y, struct Piece piece) {
    int h, i, j, np;
    double xp[100], yp[100];
    double tx, ty;
    double center = cell_size / 2;
    double c[2];

    if (pos_x < 8 && pos_y < 8) {
        int fx = flipped ? (MODULES - 1 - (int)pos_x) : (int)pos_x;
        int fy = flipped ? (int)pos_y : (MODULES - 1 - (int)pos_y);

        tx = offset + fx * cell_size + center;
        ty = offset + fy * cell_size + center;

    } else {
        tx = pos_x;
        ty = pos_y;
    }

    if (piece.is_moving == 1) {    
        if      (piece.color == WHITE) { c[0] = 0.6; c[1] = 0.4; } 
        else if (piece.color == BLACK) { c[0] = 0.4; c[1] = 0.6; }
    }  else {
        if      (piece.color == WHITE) { c[0] = 1; c[1] = 0; } 
        else if (piece.color == BLACK) { c[0] = 0; c[1] = 1; }
    }

    if (piece.is_moving) {
        single_box_shadow(tx - center, ty - center);
    }

    for (i = 0; i < numpolys[object]; i++) {
        np = psize[object][i];
        for (j = 0; j < np; j++) {
            h = con[object][i][j];
            xp[j] = x[object][h] + tx;
            yp[j] = y[object][h] + ty;
        }

        G_rgb(c[0],c[0],c[0]);
        G_fill_polygon(xp, yp, np);
        G_rgb(c[1],c[1],c[1]);
        G_polygon(xp, yp, np);
    }
    G_display_image();
}

void draw_all_pieces() {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {

            if (board[row][col].type != NONE) {
                int sprite_index = board[row][col].type - 1;
                draw_object(sprite_index, col, row, board[row][col]);
            }

        }
    }
}

void animate_flip() {
    grid_squares();
    labels();

    int indices[64];
    for (int i = 0; i < 64; i++) {
        indices[i] = i;
    }

    // shuffle
    for (int i = 63; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }

    for (int i = 0; i < 64; i++) {
        int index = indices[i];
        int row = index / 8;
        int col = index % 8;

        if (board[row][col].type != NONE) {
            int sprite_index = board[row][col].type - 1;
            draw_object(sprite_index, col, row, board[row][col]);
            perlin();
            G_display_image();
            t += 0.001;
            usleep(5000);
        }
    }
}

void grid_lines() {
    int gridLength = cell_size * MODULES; 
    int step = 3;  

    G_rgb(0, 0, 0);
 
    for (int i = 1; i < MODULES; i++) {
        double x = offset + i * cell_size;  
        for (int y = offset; y <= offset + gridLength; y += step) {
            int index = (y - (int)offset) % L_PTS;
            double offsetX = l_pts[index][0] * 2 - 1; 
            double offsetY = l_pts[index][1] * 2 - 1;
            G_fill_rectangle(x + offsetX, y + offsetY, 2, 2);
        }
    }

    for (int j = 1; j < MODULES; j++) {
        double y = offset + j * cell_size; 
        for (int x = offset; x <= offset + gridLength; x += step) {
            int index = (x - (int)offset) % L_PTS;
            double offsetX = l_pts[index][0] * 2 - 1;
            double offsetY = l_pts[index][1] * 2 - 1;
            G_fill_rectangle(x + offsetX, y + offsetY, 2, 2);
        }
    }
}

void labels() {
    char c[2], s[2];
    int flag = 1;

    for (int i = 0; i < MODULES; i++) {
        c[0] = flipped ? 'H' - i : 'A' + i;
        c[1] = '\0';

        s[0] = flipped ? '8' - i : '1' + i;
        s[1] = '\0';

        double A = offset + i * cell_size + 60;
        double B = offset + 3;

        if (flag < 0) Gi_rgb(0,0,0);
        else          Gi_rgb(255,255,255);

        G_draw_string(c, A, B);
        G_draw_string(c, A + 1, B);

        G_draw_string(s, B, A);
        G_draw_string(s, B + 1, A);

        flag = -flag;
    }
}

void grid_squares() {
    double fill = offset - 10;
    double size = 20 + cell_size * MODULES;
    G_rgb(0,0,0);
    G_fill_rectangle(fill, fill, size, size);
    for (int i = 0; i < MODULES; i++) {
        for (int j = 0; j < MODULES; j++) {
            double x = offset + i * cell_size;
            double y = offset + j * cell_size;

            single_box(x, y); 
        }
    }
}

// ----- logic ----- //
int click_in_board(int p[2])
// if click is inside board return 1 
// else return 0
{
    double min = offset, max = WINDOW_SIZE - offset;
    return (p[0] > min && p[0] < max && p[1] > min && p[1] < max);
}

void get_board_coords(int p[2], double *x, double *y)
// returns -pointers- coords to
// represent [8][8] board array:
//  0,0  1,0  2,0  3,0  4,0  5,0  6,0  7,0 
//  0,1  1,1  2,1  3,1  4,1  5,1  6,1  7,1 
//  0,2  1,2  2,2  3,2  4,2  5,2  6,2  7,2 
//  0,3  1,3  2,3  3,3  4,3  5,3  6,3  7,3 
//  0,4  1,4  2,4  3,4  4,4  5,4  6,4  7,4 
//  0,5  1,5  2,5  3,5  4,5  5,5  6,5  7,5 
//  0,6  1,6  2,6  3,6  4,6  5,6  6,6  7,6 
//  0,7  1,7  2,7  3,7  4,7  5,7  6,7  7,7 
{
    double X = (p[0] - offset) / cell_size;
    double Y = (p[1] - offset) / cell_size;

    if (flipped) {
        *x = MODULES - 1 - (int)X;  // flip column
        *y = (int)Y;                // flip row
    } else {
        *x = (int)X;
        *y = MODULES - 1 - (int)Y;
    }
}

void draw_board() {
    // background();
    perlin();
    grid_squares();
    labels();
    // grid_lines();
}
