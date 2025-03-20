#define MAXOBJS 20
#define MAXPTS 1000
#define MAXPOLYS 10000

int numobjects;
int numpoints[MAXOBJS];
double x[MAXOBJS][MAXPTS], y[MAXOBJS][MAXPTS];
int numpolys[MAXOBJS];
int psize[MAXOBJS][MAXPOLYS];
int con[MAXOBJS][MAXPOLYS][20];

void read_object(int onum, char **fname) 
{
    FILE *f;
    f = fopen(fname[onum], "r");
    if (f == NULL) {
        printf("File not found : %s.\n", fname[onum]);
        exit(1);
    }
    
    fscanf(f, "%d", &numpoints[onum]);
    if (numpoints[onum] > MAXPTS) {
        printf("Too many points.\n");
        exit(1);
    }

    for (int i=0; i<numpoints[onum]; i++) {
        fscanf(f, "%lf %lf", &x[onum][i], &y[onum][i]);
    }
    
    fscanf(f, "%d", &numpolys[onum]);
    if (numpolys[onum] > MAXPOLYS) {
        printf("Too many polygons.\n");
        exit(1);
    }

    for (int i=0 ; i<numpolys[onum]; i++) {
        fscanf(f, "%d", &psize[onum][i]);
        for (int j=0; j<psize[onum][i]; j++) {
            fscanf(f, "%d", &con[onum][i][j]);
        }
    }
}

void read_all_objects()
{
    numobjects = 6; 
    char *filenames[] = {
      "/Users/maxzwerin/Desktop/viruses/chess/sprites/pawn.xy", 
      "/Users/maxzwerin/Desktop/viruses/chess/sprites/rook.xy", 
      "/Users/maxzwerin/Desktop/viruses/chess/sprites/bishop.xy", 
      "/Users/maxzwerin/Desktop/viruses/chess/sprites/knight.xy",
      "/Users/maxzwerin/Desktop/viruses/chess/sprites/queen.xy", 
      "/Users/maxzwerin/Desktop/viruses/chess/sprites/king.xy",
    };

    for (int onum = 0; onum < numobjects; onum++) {
        read_object(onum, filenames);
    }
}

void scale_all_objects()
// Initial scaling of all sprites
{
    for (int onum = 0; onum < numobjects; onum++) {
        double minx = x[onum][0], maxx = x[onum][0], miny = y[onum][0], maxy = y[onum][0];
        for (int i = 1; i < numpoints[onum]; i++) {
            minx = fmin(minx, x[onum][i]);
            maxx = fmax(maxx, x[onum][i]);
            miny = fmin(miny, y[onum][i]);
            maxy = fmax(maxy, y[onum][i]);
        }
        double centerx = (minx + maxx)/2, centery = (miny + maxy)/2;
        double width = maxx - minx, height = maxy - miny;
        double square_size = fmax(width, height);
        double mag = 50.0 / square_size; 

        double A[3][3], B[3][3];
        M2d_make_translation(A, -centerx, -centery);
        M2d_make_scaling(B, mag, mag);
        M2d_mat_mult(B, B, A);
        M2d_mat_mult_points(x[onum], y[onum], B, x[onum], y[onum], numpoints[onum]);
    }
}

void translate_object(int object, double dx, double dy)
{
    double T[3][3];
    M2d_make_translation(T, dx, dy);
    M2d_mat_mult_points(x[object], y[object], T, x[object], y[object], numpoints[object]);
}

void scale_object(int object, double mag)
{
    double cx = 0, cy = 0;
    for (int i = 0; i < numpoints[object]; i++) {
        cx += x[object][i];
        cy += y[object][i];
    }
    cx /= numpoints[object];
    cy /= numpoints[object];

    double T1[3][3], S[3][3], T2[3][3], M[3][3], Temp[3][3];
    
    M2d_make_translation(T1, -cx, -cy);
    M2d_make_scaling(S, mag, mag);
    M2d_mat_mult(Temp, S, T1);
    
    M2d_make_translation(T2, cx, cy);
    M2d_mat_mult(M, T2, Temp);
    
    M2d_mat_mult_points(x[object], y[object], M, x[object], y[object], numpoints[object]);
}

