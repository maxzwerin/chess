#ifndef FPTOOLKIT_H_
#define FPTOOLKIT_H_

// Initializes the graphics system with the specified width and height.

#include <stdio.h>
#include <math.h>
#include <stdlib.h> // drand48, atoi
#include <unistd.h> // usleep
#include <time.h> // for the get_time stuff
#include <sys/time.h>
#include <string.h> // for strlen

//====================================================================
// X stuff :

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>// for XComposeStatus

int G_init_graphics(double width, double height);

int G_wait_key();
int G_no_wait_key();
int G_key_down(int KeyCheck);
int G_key_press();

int S_mouse_coord_window(int * Coords);

extern int (*G_display_image)();

extern int (*Gi_rgb)(int r, int g, int b);

extern int (*G_rgb)(double r, double g, double b);

extern int (*G_pixel)(double x, double y);

extern int (* G_get_pixel) (double x, double y) ;

extern int (*G_line)(double start_x, double start_y, double end_x, double end_y);

extern int (*G_clear)();

extern int (*G_circle)(double x, double y, double radius);
extern int (*G_fill_circle)(double x, double y, double radius);

extern int (* G_triangle) (double x0, double y0, double x1, double y1, double x2, double y2) ; 
extern int (* G_fill_rectangle) (double xleft, double yleft, double width, double height) ;

extern int (* G_polygon) (double *xx, double *yy, double n) ;
extern int (* G_fill_polygon) (double *xx, double *yy, double n) ;

extern int (*G_point)(double x, double y);

extern int (*G_draw_string)(const void *text, double x, double y);

#endif // FPTOOLKIT_H_
