//
// Created by sepehr on 12/19/18.
//

#ifndef ALTER_TANK_PHYSICS_H
#define ALTER_TANK_PHYSICS_H

#include "structs.h"

void move_tanks(Tank*,Tank*,Wall*);
void turn_tanks(Tank*,Tank*);
void move_bullets(Tank*,Tank*,Mine*);

#endif //ALTER_TANK_PHYSICS_H
