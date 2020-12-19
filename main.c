//IN THE NAME OF GOD

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <time.h>
#include "structs.h"
#include "view.h"
#include "logic.h"
#include "physics.h"
#include "maze_builder.h"
#include "menu.h"
#include "powerup.h"

fps = 60;
bullet_limit = 5;
bullet_life_time = 13000;
bullet_radius = 4;
count_rows = 10;
count_columns = 10;
maze_start_x = 50;
maze_start_y = 50;
cell_size = 80;
is_pause = 1;
wall_size = 5;
move_speed = 3;
tank_radius = 20;
passed_ticks = 35;
window_width = 1000;
window_height = 950;
end_program = 0;
which_page = 1;
win_point = 5;
game_start = 0;
powerup_limit = 100;
powerup_radius = 9;
laser_radius = 20;
error = 0;
key_conflict = 0;
p1_keyset = 1;
p2_keyset = 1;
mine_limit = 30;
frag_speed = 8;
frag_count = 20;
frag_lifetime = 1500;
fragbomb_radius = 7;
fragbomb_speed = 3;
frag_radius = 2;
mine_set_time = 2000;
mine_explode_time = 1300;
mine_radius = 14;

void set_variables(Tank *tank_1, Tank *tank_2, Info *info, Wall *walls) {
    int i = 0;
    tank_1->move_status = 0;
    tank_1->turn_status = 0;
    tank_1->is_dead = 0;
    tank_1->laser = 0;
    tank_1->fbomb = 0;
    tank_1->mine = 0;
    tank_1->up_key = '^';
    tank_1->down_key = '|';
    tank_1->right_key = '>';
    tank_1->left_key = '<';
    tank_1->fire_key = 'p';
    tank_2->move_status = 0;
    tank_2->turn_status = 0;
    tank_2->is_dead = 0;
    tank_2->laser = 0;
    tank_2->fbomb = 0;
    tank_2->mine = 0;
    tank_2->up_key = 'e';
    tank_2->down_key = 'd';
    tank_2->right_key = 'f';
    tank_2->left_key = 's';
    tank_2->fire_key = 'q';
    info->p1pt = 0;
    info->p2pt = 0;
    info->countdown = 1000000;
    for (i = 0; i < powerup_limit; i++) {
        (info->powerup + i)->is_full = 0;
    }
    for (i = 0; i < 1000; i++) {
        (walls + i)->does_exist = 0;
    }
}

int main() {
    int i;
    int passed_ticks_temp;
    srand(time(NULL));
    int *maze = (int *) malloc(sizeof(int) * 1000);
    Wall *walls = malloc(sizeof(Wall) * 1000);
    Tank *tank_1 = malloc(sizeof(Tank) * 1);
    Tank *tank_2 = malloc(sizeof(Tank) * 1);
    tank_1->bullet = (Bullet *) malloc(sizeof(Bullet) * bullet_limit);
    tank_2->bullet = (Bullet *) malloc(sizeof(Bullet) * bullet_limit);
    tank_1->fragbomb = (Fragbomb *) malloc(sizeof(Fragbomb) * 1);
    tank_2->fragbomb = (Fragbomb *) malloc(sizeof(Fragbomb) * 1);
    tank_1->fragbomb->frags = (Frag *) malloc(sizeof(Frag) * frag_count);
    tank_2->fragbomb->frags = (Frag *) malloc(sizeof(Frag) * frag_count);
    Mine *mines = (Mine *) malloc(sizeof(Mine) * mine_limit);
    for (i = 0; i < mine_limit; i++) {
        (mines + i)->frags = (Frag *) malloc(sizeof(Frag) * frag_count);
    }
    Info *info = (Info *) malloc(sizeof(Info) * 1);
    info->powerup = (Powerup *) malloc(sizeof(Powerup) * powerup_limit);
    set_variables(tank_1, tank_2, info, walls);
    init_window();
    menu(tank_1, tank_2, maze, walls, info);
    while (1) {
        passed_ticks_temp = SDL_GetTicks();
        renderer_ready();
        check_round(tank_1, tank_2, info, maze, walls, mines);
        handle_events(tank_1, tank_2, maze, walls, info, mines);
        if (exit_check() == 1) break;
        if (is_pause == 1) continue;
        powerups(tank_1, tank_2, info, walls, mines);
        move_tanks(tank_1, tank_2, walls);
        tanks_move_check(tank_1, tank_2, walls);
        turn_tanks(tank_1, tank_2);
        bullets_move_check(tank_1->bullet, tank_2->bullet, walls);
        frags_move_check(tank_1->fragbomb, tank_2->fragbomb, mines, walls);
        move_bullets(tank_1, tank_2, mines);
        draw_tank(tank_1, tank_2);
        draw_wall(walls);
        check_bullet_time(tank_1, tank_2, mines);
        draw_bullet(tank_1, tank_2, mines);
        top_info(tank_1, tank_2, walls, info);
        end_of_game(info);
        renderer_present();
        SDL_Delay(1000 / fps);
        passed_ticks = SDL_GetTicks() - passed_ticks_temp;
    }
    quit_window();
    return 0;
}