#ifndef GUI_H 
#define GUI_H

#include <time.h>
#include <unistd.h>
#include "raylib.h"

#ifdef WHITE
#undef WHITE
#endif
#ifdef BLACK
#undef BLACK
#endif

#include "movegen.c"

#define WIDTH 800
#define HEIGHT 800

#define rgb(r,g,b,a) (Color){(r),(g),(b),(a)}

#endif
