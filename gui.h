#ifndef GUI_H 
#define GUI_H

#ifdef WHITE
#undef WHITE
#endif
#ifdef BLACK
#undef BLACK
#endif

#define WIDTH 800
#define HEIGHT 800

#define rgb(r,g,b,a) (Color){(r),(g),(b),(a)}

#endif
