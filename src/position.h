#include <stdlib.h>

#ifndef POSITION_H
#define POSITION_H

typedef struct _position {
    int y;
    int x;
} position;

position posShift(position start_pos, int y_change, int x_change) {
    position new_pos;

    new_pos.y = start_pos.y + y_change;
    new_pos.x = start_pos.x + x_change;

    return new_pos;
}

#endif