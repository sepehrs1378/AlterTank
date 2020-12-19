//
// Created by sepehr on 12/19/18.
//

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "structs.h"
#include "view.h"
#include "logic.h"
#include "physics.h"

const double bullet_speed = 5.0, rotation_speed = 0.1;

void move_tanks(Tank *tank_1, Tank *tank_2, Wall *walls) {
    (tank_1->x) = (tank_1->x) + (tank_1->move_status) * move_speed * cos(tank_1->angle);
    (tank_1->y) = (tank_1->y) - (tank_1->move_status) * move_speed * sin(tank_1->angle);
    (tank_2->x) = (tank_2->x) + (tank_2->move_status) * move_speed * cos(tank_2->angle);
    (tank_2->y) = (tank_2->y) - (tank_2->move_status) * move_speed * sin(tank_2->angle);
    tanks_move_check(tank_1, tank_2, walls);
}

void turn_tanks(Tank *tank_1, Tank *tank_2) {
    double temp = (tank_1->turn_status) * rotation_speed;
    (tank_1->angle) += temp;
    if ((tank_1->angle) > 2 * M_PI) (tank_1->angle) -= 2 * M_PI;
    if ((tank_1->angle) < 0) (tank_1->angle) += 2 * M_PI;
    temp = (tank_2->turn_status) * rotation_speed;
    (tank_2->angle) += temp;
    if ((tank_2->angle) > 2 * M_PI) (tank_2->angle) -= 2 * M_PI;
    if ((tank_2->angle) < 0) (tank_2->angle) += 2 * M_PI;
}

void move_bullets(Tank *tank_1, Tank *tank_2, Mine *mines) {
    int i, j;
    for (i = 0; i < bullet_limit; i++) {
        (tank_1->bullet + i)->x = ((tank_1->bullet + i)->x) + bullet_speed * cos((tank_1->bullet + i)->angle);
        (tank_1->bullet + i)->y = ((tank_1->bullet + i)->y) - bullet_speed * sin((tank_1->bullet + i)->angle);
        (tank_2->bullet + i)->x = ((tank_2->bullet + i)->x) + bullet_speed * cos((tank_2->bullet + i)->angle);
        (tank_2->bullet + i)->y = ((tank_2->bullet + i)->y) - bullet_speed * sin((tank_2->bullet + i)->angle);
    }
    for (i = 0; i < mine_limit; i++) {
        for (j = 0; j < frag_count; j++) {
            if ((mines->frags + i)->is_active == 1) {
                ((mines + i)->frags + j)->x =
                        (((mines + i)->frags + j)->x) + frag_speed * cos(((mines + i)->frags + j)->angle);
                ((mines + i)->frags + j)->y =
                        (((mines + i)->frags + j)->y) - frag_speed * sin(((mines + i)->frags + j)->angle);
            }
        }
    }
    for (i = 0; i < frag_count; i++) {
        if ((tank_1->fragbomb->frags + i)->is_active == 1) {
            (tank_1->fragbomb->frags + i)->x =
                    (tank_1->fragbomb->frags + i)->x + frag_speed * cos((tank_1->fragbomb->frags + i)->angle);
            (tank_1->fragbomb->frags + i)->y =
                    (tank_1->fragbomb->frags + i)->y - frag_speed * sin((tank_1->fragbomb->frags + i)->angle);
        }
        if ((tank_2->fragbomb->frags + i)->is_active == 1) {
            (tank_2->fragbomb->frags + i)->x =
                    (tank_2->fragbomb->frags + i)->x + frag_speed * cos((tank_2->fragbomb->frags + i)->angle);
            (tank_2->fragbomb->frags + i)->y =
                    (tank_2->fragbomb->frags + i)->y - frag_speed * sin((tank_2->fragbomb->frags + i)->angle);
        }
    }
    tank_1->fragbomb->x = tank_1->fragbomb->x + bullet_speed * cos(tank_1->fragbomb->angle);
    tank_1->fragbomb->y = tank_1->fragbomb->y - bullet_speed * sin(tank_1->fragbomb->angle);
    tank_2->fragbomb->x = tank_1->fragbomb->x + bullet_speed * cos(tank_2->fragbomb->angle);
    tank_2->fragbomb->y = tank_1->fragbomb->y - bullet_speed * sin(tank_2->fragbomb->angle);
}