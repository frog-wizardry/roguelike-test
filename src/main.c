#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

typedef struct _position {
    int y;
    int x;
} position;

typedef struct _player {
    position pos;
    int health;
} player;

player * startPlayer();
int movePlayer(position new_pos, player * user);

int main() {
    player * user;
    char user_input;

    /* starting game screen (terminal) */
    initscr();
    noecho();

    /* setting up the game */
    user = startPlayer();

    /* game loop */
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
    /* Prints user character in correct position */
    mvprintw(user->pos.y, user->pos.x, ".");

    mvprintw(1, 1, "test");

    /* Changes user position in code*/
    user->pos.y = new_pos.y;
    user->pos.x = new_pos.x;

    printf("%d, %d", user->pos.y, user->pos.x);

    mvprintw(user->pos.y, user->pos.x, "@");

    /* Puts cursor over user */
    move(user->pos.y, user->pos.x);
    
    return 0;
}