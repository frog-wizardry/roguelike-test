#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ncurses.h>

/* defining types */
typedef struct _position {
    int y;
    int x;
} position;

typedef struct _player {
    position pos;
    int health;
} player;

typedef struct _room {
    position pos;
    int height;
    int width;
} room;

/* defining functions */
player * startPlayer();
int movePlayer(position new_pos, player * user);

room * createRoom(position pos, int height, int width);

int drawRoom(room * room);
int drawWalls();

int checkTiles(position pos, char tile);
int checkArea(position top_left, position bottom_right, char tile);

int main() {
    /* starting randomization */
    srand(time(NULL));

    /* starting game screen (terminal) */
    initscr();
    noecho();
    
    int win_row, win_col;
    getmaxyx(stdscr, win_row, win_col);

    /* setting up the game */
    /* generating dungeon rooms */
    int room_number = 8;
    room * dungeon_rooms[room_number];

    position room_position, room_end_position; 
    int room_height, room_width;

    /* first room - hard coded */
    room_position.y = 3;
    room_position.x = 10;

    dungeon_rooms[0] = createRoom(room_position, 5, 8);
    drawRoom(dungeon_rooms[0]);

    for(int i = 1; i < room_number; i++) {
        room_height = (rand() % 7) + 4;
        room_width = (rand() % 7) + 4;

        room_position.y = rand() % (win_row - room_height - 2) + 1;
        room_position.x = rand() % (win_col - room_width - 2) + 1;

        room_end_position.y = room_position.y + room_height;
        room_end_position.x = room_position.x + room_width;

        if(checkArea(room_position, room_end_position, '.') == 1) {
            dungeon_rooms[i] = createRoom(room_position, room_height, room_width);
            drawRoom(dungeon_rooms[i]);
        }

        else {
            i--;
        }
    }

    /* draws map */
    drawWalls();

    /* create player character */
    player * user;
    user = startPlayer();

    /* game loop */
    char user_input;
    
    while(user_input != 'q') {

        user_input = getch();
        position new_pos = user->pos;

        /* use player input */
        switch(user_input) {

            /* movement (8-directions) */
            case '1':
                new_pos.y = user->pos.y + 1;
                new_pos.x = user->pos.x - 1;
                movePlayer(new_pos, user);
                break;
            
            case '2':
                new_pos.y = user->pos.y + 1;
                movePlayer(new_pos, user);
                break;
            
            case '3':
                new_pos.y = user->pos.y + 1;
                new_pos.x = user->pos.x + 1;
                movePlayer(new_pos, user);
                break;
            
            case '4':
                new_pos.x = user->pos.x - 1;
                movePlayer(new_pos, user);
                break;

            case '6':
                new_pos.x = user->pos.x + 1;
                movePlayer(new_pos, user);
                break;
            
            case '7':
                new_pos.y = user->pos.y - 1;
                new_pos.x = user->pos.x - 1;
                movePlayer(new_pos, user);
                break;
            
            case '8':
                new_pos.y = user->pos.y - 1;
                movePlayer(new_pos, user);
                break;

            case '9':
                new_pos.y = user->pos.y - 1;
                new_pos.x = user->pos.x + 1;
                movePlayer(new_pos, user);
                break;
        }

        refresh();
    }

    endwin();

    /* free all allocated memory */
    for(int i = 0; i < room_number; i++) { 
        free(dungeon_rooms[i]);
    }

    free(user);
}

player * startPlayer() {

    player * new_player;
    new_player = malloc(sizeof(player));

    new_player->pos.y = 5;
    new_player->pos.x = 10;
    new_player->health = 20;

    mvprintw(new_player->pos.y, new_player->pos.x, "@");
    move(new_player->pos.y, new_player->pos.x);

    return new_player;
}

int movePlayer(position new_pos, player * user) {

    /* checks if movement is possible */
    if(mvinch(new_pos.y, new_pos.x) != '.') {
        move(user->pos.y, user->pos.x);
        return 1;
    }

    /* prints user character in correct position */
    mvprintw(user->pos.y, user->pos.x, ".");

    /* Changes user position in code*/
    user->pos.y = new_pos.y;
    user->pos.x = new_pos.x;

    mvprintw(user->pos.y, user->pos.x, "@");

    /* puts cursor over user */
    move(user->pos.y, user->pos.x);
    
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
    for(int i = top_left.y; i < top_left.y + bottom_right.y; i++) {
        for(int j = top_left.x; j < top_left.x + bottom_right.x; j++) {
            if(mvinch(i, j) == tile) {

                /* returns 0 if tile found */
                return 0;
            }
        }
    }

    /* returns 1 if tile not found */
    return 1;
}