#define MODULES 8
#define WINDOW_SIZE 800

#define B_PTS 100000
#define S_PTS 1000
#define L_PTS 500000

const double offset = 100;
const double cell_size = (int)((WINDOW_SIZE - 2 * offset) / MODULES);

double b_pts[B_PTS][2];
double s_pts[S_PTS][2];
double l_pts[L_PTS][2];

/*#####################################################*/
/*                       DISPLAY                       */
/*#####################################################*/

void initialize_rand_pts()
{
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

void background() 
{
    G_rgb(0.1, 0.2, 0.0);
    G_clear();
    G_rgb(0.1, 0.3, 0.0);
    for (int i = 0; i < B_PTS; i++) {
        G_point(b_pts[i][0], b_pts[i][1]);
    }
}

void single_box(int x, int y) 
{
    double r[2], g[2], b[2];
    if ((x + y) % 2 == 0) {
        r[0] = 0.42; r[1] = 0.52;
        g[0] = 0.18; g[1] = 0.28;
        b[0] = 0.05; b[1] = 0.05;
    } else {
        r[0] = 0.90; r[1] = 0.80;
        g[0] = 0.80; g[1] = 0.70;
        b[0] = 0.60; b[1] = 0.50;
    }

    G_rgb(r[0], g[0], b[0]);
    G_fill_rectangle(x, y, cell_size, cell_size);
//    G_rgb(r[1], g[1], b[1]);
//    for (int i = 0; i < S_PTS; i++) {
//        double xb = s_pts[i][0];
//        double yb = s_pts[i][1];
//        G_point(x + xb, y + yb);
//    }
}

void single_box_shadow(int x, int y)
{
    double r[2], g[2], b[2];
    r[0] = 0.52; r[1] = 0.52;
    g[0] = 0.59; g[1] = 0.28;
    b[0] = 0.43; b[1] = 0.05;

    G_rgb(r[0], g[0], b[0]);
    G_fill_rectangle(x, y, cell_size, cell_size);
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

void grid_squares() 
{
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

    char c[2], s[2];
    int flag = 1;
    
    // bolded letters + numbers
    for (int i = 0; i < MODULES; i++) { 
        c[0] = 'A' + i;
        s[0] = '1' + i;
        double A = offset + i * cell_size + 60;
        double B = offset + 5;

        if (flag < 0) G_rgb(0,0,0);
        else G_rgb(1,1,1);
        G_draw_string(c, A, B);
        G_draw_string(c, A + 1, B);
        G_draw_string(s, B, A);
        G_draw_string(s, B + 1, A);
        flag = -flag;
    }
}

void exit_button() {
    G_rgb(1,0,0);
    G_fill_rectangle(WINDOW_SIZE - 20, WINDOW_SIZE - 20, 20, 20);
    G_rgb(1,1,1);
    G_line(WINDOW_SIZE - 18, WINDOW_SIZE - 18, WINDOW_SIZE - 2, WINDOW_SIZE - 2);
    G_line(WINDOW_SIZE - 2, WINDOW_SIZE - 18, WINDOW_SIZE - 18, WINDOW_SIZE - 2);
}

/*#####################################################*/
/*                        LOGIC                        */
/*#####################################################*/

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
    
    *x = (int) X;
    *y = MODULES - 1 - (int) Y;
}

void draw_board()
{
    background();
    grid_squares();
//    grid_lines();
    exit_button();
}

/*  sudo main code { 

    draw_board();
    
    double p[2];
    double x, y;

    G_wait_click(p);
    if (click_in_board(p)) {
        get_board_coords(p, &x, &y); 
    } else click is not in board

    G_display_image();
*/
