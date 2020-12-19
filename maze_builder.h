//
// Created by sepehr on 12/24/18.
//

#include "structs.h"

#ifndef ALTER_TANK_MAZE_BUILDER_H
#define ALTER_TANK_MAZE_BUILDER_H

void maze_maker(int*,Wall*);
void row_set(int*, int);
void row_random(int*, int,Wall*);
void vertical_random(int*, int,Wall*);
void walls_coordinate(Wall*);

#endif //ALTER_TANK_MAZE_BUILDER_H

