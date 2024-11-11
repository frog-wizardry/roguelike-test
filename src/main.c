#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>

#include "dungeongen.h"

/* defining types */
typedef struct _player {
    position pos;
    int health;
} player;

/* defining functions */
player * startPlayer();
int movePlayer(position new_pos, player * user);

room * createRoom(position pos, int height, int width);

int main() {
    /* starting randomization */
    srand(time(NULL));

    /* starting game screen (terminal) */
    initscr();
    noecho();
    
    int win_row = 30;
    int win_col = 100;

    /* setting up the game */
    /* generating dungeon rooms */
    int room_number = 8;
    room * dungeon_rooms[room_number];

    position room_position, room_end_position; 
    int room_height, room_width;

    /* first room - hard coded */
    room_position.y = 3;
    room_position.x = 5;

    dungeon_rooms[0] = createRoom(room_position, 5, 8);
    drawRoom(dungeon_rooms[0]);

    for(int i = 1; i < room_number; i++) {

        /* randomize room position and size */
        room_height = (rand() % 7) + 4;
        room_width = (rand() % 7) + 4;

        room_position.y = rand() % (win_row - room_height - 2) + 1;
        room_position.x = rand() % (win_col - room_width - 2) + 1;

        /* figures out position of room bottom right corner */
        room_end_position.y = room_position.y + room_height;
        room_end_position.x = room_position.x + room_width;

        /* checks for close rooms */
        if(checkArea(posShift(room_position, -3, -3), posShift(room_end_position, 3, 3), '.') == 1) {
            dungeon_rooms[i] = createRoom(room_position, room_height, room_width);
            drawRoom(dungeon_rooms[i]);

            createDoors(dungeon_rooms[i]);
        }

        else {
            i--;
        }
    }

    /* draws map */
    drawWalls();
    drawDoors(dungeon_rooms, room_number);

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