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
#include "structs.h"
#include "view.h"
#include "logic.h"
#include "physics.h"

#ifndef ALTER_TANK_POWERUP_H
#define ALTER_TANK_POWERUP_H

void powerups(Tank*,Tank*,Info*,Wall*,Mine*);
int tell_ready_powerup(Powerup*);
void spawn_powerups(Powerup*);
void take_powerup(Tank*,Tank*,Powerup*);
void laser(Tank*,Tank*,Wall*,Info*);
void fragbomb_explode(Fragbomb*);
void check_fragbombs(Tank*,Tank*);
int tell_ready_mine(Mine*);
void set_mine(Mine*,double,double);
void mine_explode(Mine*);
void check_mine_time(Mine*);

#endif //ALTER_TANK_POWERUP_H
