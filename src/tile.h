#include <stdlib.h>

#include "position.h"

int checkTiles(position pos, char tile) {
    /* check for tile in near 3x3 area */
    for(int i = -1; i < 2; i++) {
        for(int j = -1; j < 2; j++) {
            if(mvinch(pos.y + i, pos.x + j) == tile) {

                /* returns 0 if tile found */
                return 0;
            }
        }
    }

    /* returns 1 if no tile found*/
    return 1;
}

int checkArea(position top_left, position bottom_right, char tile) {
    /* check for tile in general area */
    for(int i = top_left.y; i < bottom_right.y; i++) {
        for(int j = top_left.x; j < bottom_right.x; j++) {
            if(mvinch(i, j) == tile) {

                /* returns 0 if tile found */
                return 0;
            }
        }
    }

    /* returns 1 if tile not found */
    return 1;
}