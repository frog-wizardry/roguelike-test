#include <stdlib.h>

#include "position.h"
#include "tile.h"

typedef struct _room {
    position pos;
    int height;
    int width;
    position doors[4];
} room;

int drawRoom(room * room) {

    /* draw floor tiles*/
    for(int i = 0; i < room->height; i++) {
        for(int j = 0; j < room->width; j++) {
            mvprintw(room->pos.y + i, room->pos.x + j, ".");
        }
    }

    return 0;
}

int drawWalls() {
    int win_row, win_col;
    position tile_pos;

    /* find window size */
    getmaxyx(stdscr, win_row, win_col);

    /* check for every solid tile in view */
    for(tile_pos.y = 0; tile_pos.y < win_row; tile_pos.y++) {
        for(tile_pos.x = 0; tile_pos.x < win_col; tile_pos.x++) {
            
            /* only test if its a solid tile */
            if(mvinch(tile_pos.y, tile_pos.x) == ' ') {

                if(checkTiles(tile_pos, '.') == 0) {
                    mvprintw(tile_pos.y, tile_pos.x, "X");
                }
            }
        }
    }

    return 0;
}

room * createRoom(position pos, int height, int width) {
    room * new_room;
    new_room = malloc(sizeof(room));

    new_room->pos = pos;
    new_room->height = height;
    new_room->width = width;

    return new_room;
}

int createDoors(room * room) {
    int has_door;
    int door_generated = 0;

    for(int side = 0; side < 4; side++) {
        has_door = rand() % 2;
        
        if(has_door == 1) {
            switch(side) {
                case 0:
                    room->doors[side] = posShift(room->pos, -1, rand() % room->width);
                    break;
                
                case 1:
                    room->doors[side] = posShift(room->pos, rand() % room->height, -1);
                    break;

                case 2:
                    room->doors[side] = posShift(room->pos, room->height, rand() % room->width);
                    break;
                case 3:
                    room->doors[side] = posShift(room->pos, rand() % room->height, room->width);
                    break;
            }

            door_generated++;
        }

        else {
            room->doors[0].y = NULL;
            room->doors[0].x = NULL;
        }
    }

    if(door_generated == 0) {
        createDoors(room);
    }
}

int drawDoors(room ** rooms, int room_number) {
    for(int i = 0; i < room_number; i++) {
        for(int j = 0; j < 4; j++) {
            if(rooms[i]->doors[j].y != NULL || rooms[i]->doors[j].x != NULL) {
                mvprintw(rooms[i]->doors[j].y, rooms[i]->doors[j].x, "+");
            }
        }
    }

    return 0;
}