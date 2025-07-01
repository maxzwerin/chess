#ifndef M2D_MATRIX_TOOLS_H_
#define M2D_MATRIX_TOOLS_H_

#include <math.h>

// Prints a 3x3 matrix
int M2d_print_mat(double a[3][3]);

// Copies matrix b into matrix a
int M2d_copy_mat(double a[3][3], double b[3][3]);

// Sets matrix a to the identity matrix
int M2d_make_identity(double a[3][3]);

// Creates a translation matrix in a
int M2d_make_translation(double a[3][3], double dx, double dy);

// Creates a scaling matrix in a
int M2d_make_scaling(double a[3][3], double sx, double sy);

// Creates a rotation matrix using precomputed cosine and sine
int M2d_make_rotation_cs(double a[3][3], double cs, double sn);

// Creates a rotation matrix using angle in radians
int M2d_make_rotation(double a[3][3], double theta);

// Matrix multiplication: res = a * b
int M2d_mat_mult(double res[3][3], double a[3][3], double b[3][3]);

// Multiply point: P = m * Q
int M2d_mat_mult_pt(double P[2], double m[3][3], double Q[2]);

// Multiply a list of points: X, Y = m * x, y
int M2d_mat_mult_points(double *X, double *Y, double m[3][3],
                        double *x, double *y, int numpoints);

#endif // M2D_MATRIX_TOOLS_H_
