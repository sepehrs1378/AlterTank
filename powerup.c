//
// Created by sepehr on 1/27/19.
//

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

void spawn_powerups(Powerup *powerup) {
    //if ((rand() % 3) == 0) {
    int temp = tell_ready_powerup(powerup);
    if (temp != -1) {
        (powerup + temp)->is_full = 1;
        (powerup + temp)->type = rand() % 3 + 1;
        (powerup + temp)->x = maze_start_x + (rand() % count_columns) * cell_size + cell_size / 2.0;
        (powerup + temp)->y = maze_start_y + (rand() % count_rows) * cell_size + cell_size / 2.0;
    }
    //}
}

void take_powerup(Tank *tank_1, Tank *tank_2, Powerup *powerup) {
    int i;
    double d;
    for (i = 0; i < powerup_limit; i++) {
        double X, Y;
        X = (powerup + i)->x;
        Y = (powerup + i)->y;
        d = ptpt_dis(X, Y, tank_1->x, tank_1->y);
        if (d < (tank_radius + powerup_radius) / 2.0 + 4 && tank_1->laser == 0 && tank_1->mine == 0 &&
            tank_1->fbomb == 0) {
            switch ((powerup + i)->type) {
                case 1:
                    tank_1->fbomb = 1;
                    break;
                case 2:
                    tank_1->laser = 3000;
                    break;
                case 3:
                    tank_1->mine = 3;
                    break;
            }
            (powerup + i)->is_full = 0;
        }
        d = ptpt_dis(X, Y, tank_2->x, tank_2->y);
        if (d < (tank_radius + powerup_radius) / 2.0 + 4 && tank_2->laser == 0 && tank_2->mine == 0 &&
            tank_2->fbomb == 0) {
            switch ((powerup + i)->type) {
                case 1:
                    tank_2->fbomb = 1;
                    break;
                case 2:
                    tank_2->laser = 3000;
                    break;
                case 3:
                    tank_2->mine = 3;
                    break;
            }
            (powerup + i)->is_full = 0;
        }
    }
}

int tell_ready_powerup(Powerup *powerup) {
    int i;
    for (i = 0; i < powerup_limit; i++) {
        if ((powerup + i)->is_full == 0) {
            return i;
        }
    }
    return -1;
}

void laser(Tank *tank_1, Tank *tank_2, Wall *walls, Info *info) {
    int step = 5;
    double x2, y2;
    if (tank_1->laser == 3000) {
        x2 = tank_1->x;
        y2 = tank_1->y;
        while (1) {
            x2 = x2 + step * cos(tank_1->angle);
            y2 = y2 - step * sin(tank_1->angle);
            if (ptpt_dis(x2, y2, tank_2->x, tank_2->y) < tank_radius / 2.0 + 3 && tank_2->is_dead == 0)
                break;
            if (x2 < maze_start_x || x2 > maze_start_x + count_columns * cell_size || y2 < maze_start_y ||
                y2 > maze_start_y + count_rows * cell_size)
                break;
        }
        draw_prelaser(tank_1->x, tank_1->y, x2, y2);
    }
    if (tank_1->laser < 3000 && tank_1->laser > 0) {
        tank_1->move_status = 0;
        tank_1->turn_status = 0;
        tank_1->laser -= passed_ticks;
        if (tank_1->laser < 2000) {
            double x2 = tank_1->x, y2 = tank_1->y;
            while (1) {
                x2 = x2 + step * cos(tank_1->angle);
                y2 = y2 - step * sin(tank_1->angle);
                if (x2 < maze_start_x || x2 > maze_start_x + count_columns * cell_size || y2 < maze_start_y ||
                    y2 > maze_start_y + count_rows * cell_size)
                    break;
            }
            draw_laser(tank_1->x, tank_1->y, x2, y2);
            double dis = ptline_dis(tank_1->x, tank_1->y, x2, y2, tank_2->x, tank_2->y);
            if (dis < (tank_radius + laser_radius) / 2.0 + 3 && tank_2->x < max(tank_1->x, x2) &&
                tank_2->x > min(tank_1->x, x2) && tank_2->y < max(tank_1->y, y2) && tank_2->y > min(tank_1->y, y2)) {
                tank_2->is_dead = 1;
            }
        }
        if (tank_1->laser < 0)
            tank_1->laser = 0;
    }
}

void check_mine_time(Mine *mines) {
    int i;
    for (i = 0; i < mine_limit; i++) {
        if ((mines + i)->trigger_time != mine_set_time) {
            (mines + i)->trigger_time += passed_ticks;
            if ((mines + i)->trigger_time > mine_set_time && (mines + i)->trigger_time < 10000)
                (mines + i)->trigger_time = mine_set_time;
            if ((mines + i)->trigger_time > 10000 + mine_explode_time) {
                (mines + i)->is_set = 0;
                (mines + i)->trigger_time = 0;
                mine_explode(mines + i);
            }
        }
    }
}

int tell_ready_mine(Mine *mines) {
    int i;
    for (i = 0; i < mine_limit; i++) {
        if ((mines + i)->is_set == 0) {
            return i;
        }
    }
    return -1;
}

void set_mine(Mine *mines, double x, double y) {
    int temp = tell_ready_mine(mines);
    (mines + temp)->is_set = 1;
    (mines + temp)->trigger_time = 0;
    (mines + temp)->x = x;
    (mines + temp)->y = y;
}

void check_mines(Tank *tank_1, Tank *tank_2, Mine *mines) {
    int i;
    double dis;
    for (i = 0; i < mine_limit; i++) {
        dis = ptpt_dis(tank_1->x, tank_1->y, (mines + i)->x, (mines + i)->y);
        if (dis < (tank_radius + mine_radius) / 2.0 + 4 && (mines + i)->trigger_time == mine_set_time &&
            (mines + i)->is_set == 1)
            (mines + i)->trigger_time = 10000;
        dis = ptpt_dis(tank_2->x, tank_2->y, (mines + i)->x, (mines + i)->y);
        if (dis < (tank_radius + mine_radius) / 2.0 + 4 && (mines + i)->trigger_time == mine_set_time &&
            (mines + i)->is_set == 1)
            (mines + i)->trigger_time = 10000;
    }
}

void mine_explode(Mine *mine) {
    int i;
    mine->trigger_time = 0;
    mine->is_set = 0;
    for (i = 0; i < frag_count; i++) {
        (mine->frags + i)->x = mine->x;
        (mine->frags + i)->y = mine->y;
        (mine->frags + i)->is_active = 1;
        (mine->frags + i)->life_time = 0;
        (mine->frags + i)->angle = 2 * M_PI / frag_count * i;
    }
}

void check_fragbombs(Tank *tank_1, Tank *tank_2) {
    if (tank_1->fbomb == 2) {
        tank_1->fbomb = 3;
        (tank_1->fragbomb)->life_time = 0;
        (tank_1->fragbomb)->is_active = 1;
        double X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
        double Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
        (tank_1->fragbomb)->x = X;
        (tank_1->fragbomb)->y = Y;
        (tank_1->fragbomb)->angle = tank_1->angle;
    }
    if (tank_2->fbomb == 2) {
        tank_2->fbomb = 3;
        (tank_2->fragbomb)->life_time = 0;
        (tank_2->fragbomb)->is_active = 1;
        double X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
        double Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
        (tank_2->fragbomb)->x = X;
        (tank_2->fragbomb)->y = Y;
        (tank_2->fragbomb)->angle = tank_2->angle;
    }
}

void fragbomb_explode(Fragbomb *fragbomb) {
    int i;
    fragbomb->is_active = 0;
    for (i = 0; i < frag_count; i++) {
        (fragbomb->frags + i)->life_time = 0;
        (fragbomb->frags + i)->is_active = 1;
        (fragbomb->frags + i)->x = fragbomb->x;
        (fragbomb->frags + i)->y = fragbomb->y;
        (fragbomb->frags + i)->angle = i * 2 * M_PI / frag_count;
    }
}

void powerups(Tank *tank_1, Tank *tank_2, Info *info, Wall *walls, Mine *mines) {
    spawn_powerups(info->powerup);
    draw_powerups(info->powerup);
    take_powerup(tank_1, tank_2, info->powerup);
    laser(tank_1, tank_2, walls, info);
    check_fragbombs(tank_1, tank_2);
    check_mines(tank_1, tank_2, mines);
    check_mine_time(mines);
    draw_mine(mines);
}
