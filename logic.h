//
// Created by sepehr on 12/19/18.
//

#include "structs.h"

#ifndef ALTER_TANK_LOGIC_H
#define ALTER_TANK_LOGIC_H

void tanks_move_check(Tank*,Tank*,Wall*);
int tell_ready_bullet(Bullet*);
void check_bullet_time(Tank*,Tank*,Mine*);
void spawn_tanks(Tank*,Tank*);
void tank_hit_wall(Tank*,Wall*);
void bullet_hit_wall(Bullet*,Wall*);
void bullets_move_check(Bullet*,Bullet*,Wall*);
void new_game(Tank*,Tank*,Info*,int*,Wall*);
void new_round(Tank*,Tank*,Info*,int*,Wall*);
void check_round(Tank*,Tank*,Info*,int*,Wall*,Mine*);
void check_tanks_dead(Tank*,Tank*,Mine*);
double ptline_dis(double,double,double,double,double,double);
double min(double,double);
double max(double,double);
double ptpt_dis(double,double,double,double);
void frags_move_check(Fragbomb*,Fragbomb*,Mine*,Wall*);
void frag_hit_wall(Frag*,Wall*);
void end_of_game(Info*);

#endif //ALTER_TANK_LOGIC_H
