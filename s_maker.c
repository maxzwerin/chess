#include "FPToolkit.c"

int window_size = 800;
int grid_size = 10;

void draw_grid() {
    G_rgb(0.3, 0.3, 0.3); 
    for (int i = 0; i <= window_size; i += grid_size) {
        G_line(i, 0, i, window_size - 40);
        G_line(0, i, window_size, i);      
    }
    G_rgb(1, 1, 1);
    G_rectangle(300, 300, 200, 200);
}

void clear_graphics() {
    G_rgb(0, 0, 0);
    G_clear();
    draw_grid(); 

    G_rgb(0, 1, 0);
    G_fill_rectangle(0, window_size - 40, window_size / 2, 40);
    G_rgb(0, 1, 1);
    G_fill_rectangle(window_size / 2, window_size - 40, window_size, 40);
    G_rgb(1, 0, 0);
    G_fill_rectangle(0, 0, window_size, 40);
    
    
}

void snap_to_grid(double *x, double *y) {
    *x = round(*x / grid_size) * grid_size;
    *y = round(*y / grid_size) * grid_size;
}

int click_and_save(double x[], double y[]) {
    int n = 0;
    double p[2];
    while (1) {
        G_wait_click(p);

        if (p[1] > window_size - 40 && p[0] < window_size / 2) break;
        if (p[1] > window_size - 40 && p[0] > window_size / 2) return -1;

        snap_to_grid(&p[0], &p[1]); 

        x[n] = p[0];
        y[n] = p[1];
        n++;

        G_rgb(1, 1, 1);
        G_fill_circle(p[0], p[1], 4); 
    }
    return n;
}

int copy_arr(double tempx[], double tempy[], int ntemp, double x[], double y[], int nxy)
{
    int i;
    for (i = 0; i < ntemp; i++) {
        x[nxy] = tempx[i];
        y[nxy] = tempy[i];
        nxy++;
    }
    return nxy;
}

int main() {
    FILE *g;
    printf("pawn   : 0  rook  : 1  knight : 2\n");
    printf("bishop : 3  queen : 4  king   : 5\n");
    int i, j, out;
    printf("which file to output to : ");
    scanf("%d", &out);

    if      (out == 0) g = fopen("sprites/pawn.xy", "w");
    else if (out == 1) g = fopen("sprites/rook.xy", "w");
    else if (out == 2) g = fopen("sprites/knight.xy", "w");
    else if (out == 3) g = fopen("sprites/bishop.xy", "w");
    else if (out == 4) g = fopen("sprites/queen.xy", "w");
    else if (out == 5) g = fopen("sprites/king.xy", "w");

    if (g == NULL) {
        printf("Failed to open output file.\n");
        exit(0);
    }

    G_init_graphics(window_size, window_size);
    clear_graphics();
  
    int n[1000], nxy = 0;
    double x[1000], y[1000];
    for (i = 0; 1; i++) {
        double xt[1000], yt[1000];
        n[i] = click_and_save(xt, yt);
        if (n[i] == -1) break;
        else if (n[i] == 0) {i--; continue;}
        G_rgb(0.1, 0.1, 0.3);
        G_fill_polygon(xt, yt, n[i]);
        nxy = copy_arr(xt, yt, n[i], x, y, nxy);
    }

    // FILE OUTPUT:
    // 4 <- number of points
    //     x0 y0
    //     x1 y1
    //     x2 y2
    //     x3 y3
    // 2 <- number of polygons
    // 3   0 1 2    <- number of points in polygon, which points (from above) are in polygon 
    // 4   0 1 2 3
    
    int polycount = i;
    fprintf(g, "%d\n", nxy); // how many polygons
    for (i = 0; i < nxy; i++) {
        fprintf(g, "    %lf %lf\n", x[i], y[i]); // coordinates 
    }

    int count = 0;
    fprintf(g, "%d\n", polycount);
    for (i = 0; i < polycount; i++) {
        fprintf(g, "%d   ", n[i]);
        
        for (j = 0; j < n[i]; j++) {
           fprintf(g, "%d ", count);
           count++;
        }
        fprintf(g, "\n");
    }
    
    fclose(g);
    if      (out == 0) printf("Model saved to pawn.xy\n"); 
    else if (out == 1) printf("Model saved to rook.xy\n"); 
    else if (out == 2) printf("Model saved to knight.xy\n"); 
    else if (out == 3) printf("Model saved to bishop.xy\n"); 
    else if (out == 4) printf("Model saved to queen.xy\n"); 
    else if (out == 5) printf("Model saved to king.xy\n"); 

    return 0;
}
