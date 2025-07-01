#ifndef SPRITES_H_
#define SPRITES_H_

#include "M2d_matrix_tools.h"

// Constants
#define MAXOBJS 20
#define MAXPTS 1000
#define MAXPOLYS 10000

// Global variables
extern int numobjects;
extern int numpoints[MAXOBJS];
extern double x[MAXOBJS][MAXPTS];
extern double y[MAXOBJS][MAXPTS];
extern int numpolys[MAXOBJS];
extern int psize[MAXOBJS][MAXPOLYS];
extern int con[MAXOBJS][MAXPOLYS][20];

// Function declarations
void read_object(int onum, char *fname);
void read_all_objects(void);
void scale_all_objects(void);
void translate_object(int object, double dx, double dy);
void scale_object(int object, double mag);

#endif // SPRITES_H_
