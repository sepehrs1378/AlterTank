//
// Created by sepehr on 12/19/18.
//

#ifndef ALTER_TANK_VIEW_H
#define ALTER_TANK_VIEW_H

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

void init_window();
void quit_window();
void draw_tank(Tank*,Tank*);
void renderer_ready();
void renderer_present();
void handle_events(Tank*,Tank*,int*,Wall*,Info*,Mine*);
int exit_check();
void top_info(Tank*,Tank*,Wall*,Info*);
void draw_bullet(Tank*,Tank*,Mine*);
void draw_wall(Wall*);
void draw_page_main(int,int);
void draw_page_SL(int,int);
void draw_page_s1(int);
void draw_page_s2(int);
void draw_powerups(Powerup*);
void draw_laser(double,double,double,double);
void draw_prelaser(double,double,double,double);
void draw_mine(Mine*);
void draw_endpage(Info*);

#endif //ALTER_TANK_VIEW_H
