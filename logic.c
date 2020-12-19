//
// Created by sepehr on 12/19/18.
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
#include "maze_builder.h"
#include "powerup.h"

void tank_hit_wall(Tank* tank,Wall* wall) {
    if (wall->does_exist == 0)
        return;
    if (wall->x1 == wall->x2) {
        if (tank->y >= wall->y1 - 7 && tank->y <= wall->y2 + 7 && tank->x<wall->x1) {
            double d = tank->x - wall->x1;
            d = d > 0 ? d : -d;
            if (d <= tank_radius / 2.0 + wall_size / 2.0 + 8)
                tank->x=wall->x1-(tank_radius/2.0+10);
        }
        if (tank->y >= wall->y1 - 7 && tank->y <= wall->y2 + 7 && tank->x>wall->x1) {
            double d = tank->x - wall->x1;
            d = d > 0 ? d : -d;
            if (d <= tank_radius / 2.0 + wall_size / 2.0 + 8)
                tank->x=wall->x1+(tank_radius/2.0+10);
        }
        if (tank->y >= wall->y1 - tank_radius / 2.0 - 7 && tank->y <= wall->y1 - 7) {
            double d = ptpt_dis(tank->x,tank->y,wall->x1,wall->y1);
            d = d > 0 ? d : -d;
            if (d <= tank_radius / 2.0 + 8) {
                double sn=(wall->y1-tank->y)/ptpt_dis(tank->x,tank->y,wall->x1,wall->y1);
                double cs=(tank->x-wall->x1)/ptpt_dis(tank->x,tank->y,wall->x1,wall->y1);
                tank->x=wall->x1+cs*(tank_radius/2.0+10);
                tank->y=wall->y1-sn*(tank_radius/2.0+10);
            }
        }
        if (tank->y >= wall->y2 + 7 && tank->y <= wall->y2 + tank_radius / 2.0 + 7) {
            double d = ptpt_dis(tank->x,tank->y,wall->x2,wall->y2);
            d = d > 0 ? d : -d;
            if (d <= tank_radius / 2.0 + 8) {
                double sn=(tank->y-wall->y2)/ptpt_dis(tank->x,tank->y,wall->x2,wall->y2);
                double cs=(tank->x-wall->x2)/ptpt_dis(tank->x,tank->y,wall->x2,wall->y2);
                tank->x=wall->x2+cs*(tank_radius/2.0+10);
                tank->y=wall->y2+sn*(tank_radius/2.0+10);
            }
        }
    }
    else {
        if (tank->x >= wall->x1 - 7 && tank->x <= wall->x2 + 7 && tank->y<wall->y1) {
            double d = tank->y - wall->y1;
            d = d > 0 ? d : -d;
            if (d <= tank_radius / 2.0 + wall_size / 2.0 + 8)
                tank->y=wall->y1-(tank_radius/2.0+10);
        }
        if (tank->x >= wall->x1 - 7 && tank->x <= wall->x2 + 7 && tank->y>wall->y1) {
            double d = tank->y - wall->y1;
            d = d > 0 ? d : -d;
            if (d <= tank_radius / 2.0 + wall_size / 2.0 + 8)
                tank->y=wall->y1+(tank_radius/2.0+10);
        }
        if (tank->x >= wall->x1 - tank_radius / 2.0 - 7 && tank->x <= wall->x1 - 7) {
            double d = ptpt_dis(tank->x,tank->y,wall->x1,wall->y1);
            d = d > 0 ? d : -d;
            if (d <= tank_radius / 2.0 + 8) {
                double sn=(wall->y1-tank->y)/ptpt_dis(tank->x,tank->y,wall->x1,wall->y1);
                double cs=(tank->x-wall->x1)/ptpt_dis(tank->x,tank->y,wall->x1,wall->y1);
                tank->x=wall->x1+cs*(tank_radius/2.0+10);
                tank->y=wall->y1-sn*(tank_radius/2.0+10);
            }
        }
        if (tank->x >= wall->x2 + 7 && tank->x <= wall->x2 + tank_radius / 2.0 + 7) {
            double d = ptpt_dis(tank->x,tank->y,wall->x2,wall->y2);
            d = d > 0 ? d : -d;
            if (d <= tank_radius / 2.0 + 8) {
                double sn=(wall->y2-tank->y)/ptpt_dis(tank->x,tank->y,wall->x2,wall->y2);
                double cs=(tank->x-wall->x2)/ptpt_dis(tank->x,tank->y,wall->x2,wall->y2);
                tank->x=wall->x2+cs*(tank_radius/2.0+10);
                tank->y=wall->y2-sn*(tank_radius/2.0+10);
            }
        }
    }
}

void tanks_move_check(Tank* tank_1,Tank* tank_2,Wall* walls) {
    int i;
    for (i = 0; i < count_rows*(2*count_columns+1)+count_columns; i++) {
        tank_hit_wall(tank_1,walls+i);
        tank_hit_wall(tank_2,walls+i);
    }
}

void bullet_hit_wall(Bullet* bullet,Wall* wall){
    if (wall->does_exist == 0)
        return;
    if (wall->x1 == wall->x2) {
        if (bullet->y >= wall->y1 - 4 && bullet->y <= wall->y2 + 4 && bullet->x<wall->x1 && (bullet->angle<M_PI/2 || bullet->angle>M_PI*3/2.0)) {
            double d = bullet->x - wall->x1;
            d = d > 0 ? d : -d;
            if (d <= bullet_radius / 2.0 + wall_size / 2.0 + 4) {
                bullet->angle = M_PI - bullet->angle;
                if ((bullet->angle) > 2 * M_PI) (bullet->angle) -= 2 * M_PI;
                if ((bullet->angle) < 0) (bullet->angle) += 2 * M_PI;
            }
        }
        if (bullet->y >= wall->y1 - 4 && bullet->y <= wall->y2 + 4 && bullet->x>wall->x1 && (bullet->angle>M_PI/2 && bullet->angle<M_PI*3/2.0)) {
            double d = bullet->x - wall->x1;
            d = d > 0 ? d : -d;
            if (d <= bullet_radius / 2.0 + wall_size / 2.0 + 4) {
                bullet->angle = M_PI - bullet->angle;
                if ((bullet->angle) > 2 * M_PI) (bullet->angle) -= 2 * M_PI;
                if ((bullet->angle) < 0) (bullet->angle) += 2 * M_PI;
            }
        }
    }
    else {
        if (bullet->x >= wall->x1 - 4 && bullet->x <= wall->x2 + 4 && bullet->y<wall->y1 && bullet->angle>M_PI) {
            double d = bullet->y - wall->y1;
            d = d > 0 ? d : -d;
            if (d <= bullet_radius / 2.0 + wall_size / 2.0 + 4){
                bullet->angle =2*M_PI - bullet->angle;
                if ((bullet->angle) > 2 * M_PI) (bullet->angle) -= 2 * M_PI;
                if ((bullet->angle) < 0) (bullet->angle) += 2 * M_PI;
            }
        }
        if (bullet->x >= wall->x1 - 4 && bullet->x <= wall->x2 + 4 && bullet->y>wall->y1 && bullet->angle<M_PI) {
            double d = bullet->y - wall->y1;
            d = d > 0 ? d : -d;
            if (d <= bullet_radius / 2.0 + wall_size / 2.0 + 4){
                bullet->angle =2*M_PI - bullet->angle;
                if ((bullet->angle) > 2 * M_PI) (bullet->angle) -= 2 * M_PI;
                if ((bullet->angle) < 0) (bullet->angle) += 2 * M_PI;
            }
        }
    }
}

void frag_hit_wall(Frag* frag,Wall* wall){
    if (wall->does_exist == 0)
        return;
    if (wall->x1 == wall->x2) {
        if (frag->y >= wall->y1 - 4 && frag->y <= wall->y2 + 4 && frag->x<wall->x1 && (frag->angle<M_PI/2 || frag->angle>M_PI*3/2.0)) {
            double d = frag->x - wall->x1;
            d = d > 0 ? d : -d;
            if (d <= frag_radius / 2.0 + wall_size / 2.0 + 4) {
                frag->angle = M_PI - frag->angle;
                if ((frag->angle) > 2 * M_PI) (frag->angle) -= 2 * M_PI;
                if ((frag->angle) < 0) (frag->angle) += 2 * M_PI;
            }
        }
        if (frag->y >= wall->y1 - 4 && frag->y <= wall->y2 + 4 && frag->x>wall->x1 && (frag->angle>M_PI/2 && frag->angle<M_PI*3/2.0)) {
            double d = frag->x - wall->x1;
            d = d > 0 ? d : -d;
            if (d <= frag_radius / 2.0 + wall_size / 2.0 + 4) {
                frag->angle = M_PI - frag->angle;
                if ((frag->angle) > 2 * M_PI) (frag->angle) -= 2 * M_PI;
                if ((frag->angle) < 0) (frag->angle) += 2 * M_PI;
            }
        }
    }
    else {
        if (frag->x >= wall->x1 - 4 && frag->x <= wall->x2 + 4 && frag->y<wall->y1 && frag->angle>M_PI) {
            double d = frag->y - wall->y1;
            d = d > 0 ? d : -d;
            if (d <= frag_radius / 2.0 + wall_size / 2.0 + 4){
                frag->angle =2*M_PI - frag->angle;
                if ((frag->angle) > 2 * M_PI) (frag->angle) -= 2 * M_PI;
                if ((frag->angle) < 0) (frag->angle) += 2 * M_PI;
            }
        }
        if (frag->x >= wall->x1 - 4 && frag->x <= wall->x2 + 4 && frag->y>wall->y1 && frag->angle<M_PI) {
            double d = frag->y - wall->y1;
            d = d > 0 ? d : -d;
            if (d <= frag_radius / 2.0 + wall_size / 2.0 + 4){
                frag->angle =2*M_PI - frag->angle;
                if ((frag->angle) > 2 * M_PI) (frag->angle) -= 2 * M_PI;
                if ((frag->angle) < 0) (frag->angle) += 2 * M_PI;
            }
        }
    }
}

void fragbomb_hit_wall(Fragbomb* fragbomb,Wall* wall){
    if (wall->does_exist == 0)
        return;
    if (wall->x1 == wall->x2) {
        if (fragbomb->y >= wall->y1 - 4 && fragbomb->y <= wall->y2 + 4 && fragbomb->x<wall->x1 && (fragbomb->angle<M_PI/2 || fragbomb->angle>M_PI*3/2.0)) {
            double d = fragbomb->x - wall->x1;
            d = d > 0 ? d : -d;
            if (d <= bullet_radius / 2.0 + wall_size / 2.0 + 4) {
                fragbomb->angle = M_PI - fragbomb->angle;
                if ((fragbomb->angle) > 2 * M_PI) (fragbomb->angle) -= 2 * M_PI;
                if ((fragbomb->angle) < 0) (fragbomb->angle) += 2 * M_PI;
            }
        }
        if (fragbomb->y >= wall->y1 - 4 && fragbomb->y <= wall->y2 + 4 && fragbomb->x>wall->x1 && (fragbomb->angle>M_PI/2 && fragbomb->angle<M_PI*3/2.0)) {
            double d = fragbomb->x - wall->x1;
            d = d > 0 ? d : -d;
            if (d <= bullet_radius / 2.0 + wall_size / 2.0 + 4) {
                fragbomb->angle = M_PI - fragbomb->angle;
                if ((fragbomb->angle) > 2 * M_PI) (fragbomb->angle) -= 2 * M_PI;
                if ((fragbomb->angle) < 0) (fragbomb->angle) += 2 * M_PI;
            }
        }
    }
    else {
        if (fragbomb->x >= wall->x1 - 4 && fragbomb->x <= wall->x2 + 4 && fragbomb->y<wall->y1 && fragbomb->angle>M_PI) {
            double d = fragbomb->y - wall->y1;
            d = d > 0 ? d : -d;
            if (d <= bullet_radius / 2.0 + wall_size / 2.0 + 4){
                fragbomb->angle =2*M_PI - fragbomb->angle;
                if ((fragbomb->angle) > 2 * M_PI) (fragbomb->angle) -= 2 * M_PI;
                if ((fragbomb->angle) < 0) (fragbomb->angle) += 2 * M_PI;
            }
        }
        if (fragbomb->x >= wall->x1 - 4 && fragbomb->x <= wall->x2 + 4 && fragbomb->y>wall->y1 && fragbomb->angle<M_PI) {
            double d = fragbomb->y - wall->y1;
            d = d > 0 ? d : -d;
            if (d <= bullet_radius / 2.0 + wall_size / 2.0 + 4){
                fragbomb->angle =2*M_PI - fragbomb->angle;
                if ((fragbomb->angle) > 2 * M_PI) (fragbomb->angle) -= 2 * M_PI;
                if ((fragbomb->angle) < 0) (fragbomb->angle) += 2 * M_PI;
            }
        }
    }
}

void bullets_move_check(Bullet* bullet_1,Bullet* bullet_2,Wall* walls){
    int i,j;
    for(i=0;i<bullet_limit;i++){
        for(j=0;j<count_rows*(2*count_columns+1)+count_columns;j++){
            bullet_hit_wall(bullet_1+i,walls+j);
            bullet_hit_wall(bullet_2+i,walls+j);
        }
    }
}

void frags_move_check(Fragbomb* fragbomb_1,Fragbomb* fragbomb_2,Mine* mines,Wall* walls){
    int i,j,k;
    for(i=0;i<mine_limit;i++){
        for(j=0;j<frag_count;j++){
            for(k=0;k<count_rows*(2*count_columns+1)+count_columns;k++){
                frag_hit_wall((mines+i)->frags+j,walls+k);
            }
        }
    }
    for(i=0;i<frag_count;i++){
        for(j=0;j<count_rows*(2*count_columns+1)+count_columns;j++){
            frag_hit_wall(fragbomb_1->frags+i,walls+j);
            frag_hit_wall(fragbomb_2->frags+i,walls+j);
        }
    }
    for(i=0;i<count_rows*(2*count_columns+1)+count_columns;i++){
        fragbomb_hit_wall(fragbomb_1,walls+i);
        fragbomb_hit_wall(fragbomb_2,walls+i);
    }
}

int tell_ready_bullet(Bullet* bullet){
    int i;
    for(i=0;i<bullet_limit;i++){
        if((bullet+i)->is_active==0){
            return i;
        }
    }
    return -1;
}

void check_bullet_time(Tank* tank_1,Tank* tank_2,Mine* mines){
    int i,j;
    for(i=0;i<bullet_limit;i++){
        if((tank_1->bullet+i)->life_time>bullet_life_time)
            (tank_1->bullet+i)->is_active=0;
        else
            (tank_1->bullet+i)->life_time+=passed_ticks;
        if((tank_2->bullet+i)->life_time>bullet_life_time)
            (tank_2->bullet+i)->is_active=0;
        else
            (tank_2->bullet+i)->life_time+=passed_ticks;
    }
    if(tank_1->fragbomb->life_time>bullet_life_time && tank_1->fragbomb->is_active==1){
        tank_1->fragbomb->is_active=0;
        tank_1->fbomb=0;
        fragbomb_explode(tank_1->fragbomb);
    }
    else
        tank_1->fragbomb->life_time+=passed_ticks;
    if(tank_2->fragbomb->life_time>bullet_life_time && tank_2->fragbomb->is_active==1){
        tank_2->fragbomb->is_active=0;
        tank_2->fbomb=0;
        fragbomb_explode(tank_2->fragbomb);
    }
    else
        tank_2->fragbomb->life_time+=passed_ticks;
    for(i=0;i<frag_count;i++){
        if((tank_1->fragbomb->frags+i)->life_time>frag_lifetime)
            (tank_1->fragbomb->frags+i)->is_active=0;
        else
            (tank_1->fragbomb->frags+i)->life_time+=passed_ticks;
        if((tank_2->fragbomb->frags+i)->life_time>frag_lifetime)
            (tank_2->fragbomb->frags+i)->is_active=0;
        else
            (tank_2->fragbomb->frags+i)->life_time+=passed_ticks;
    }
    for(i=0;i<mine_limit;i++){
        for(j=0;j<frag_count;j++){
            if(((mines+i)->frags+j)->life_time>frag_lifetime)
                ((mines+i)->frags+j)->is_active=0;
            else
                ((mines+i)->frags+j)->life_time+=passed_ticks;
        }
    }

}

void spawn_tanks(Tank* tank_1,Tank* tank_2){
    tank_1->x=maze_start_x+(rand()%count_columns)*cell_size+cell_size/2.0;
    tank_1->y=maze_start_y+(rand()%count_rows)*cell_size+cell_size/2.0;
    tank_1->angle=(rand()%1000)*2*M_PI/1000;
    tank_1->fbomb=0;
    tank_1->mine=0;
    tank_1->laser=0;
    tank_2->x=maze_start_x+(rand()%count_columns)*cell_size+cell_size/2.0;
    tank_2->y=maze_start_y+(rand()%count_rows)*cell_size+cell_size/2.0;
    tank_2->angle=(rand()%1000)*2*M_PI/1000;
    tank_2->fbomb=0;
    tank_2->mine=0;
    tank_2->laser=0;
}

double ptline_dis(double x1,double y1,double x2,double y2,double x,double y){
    if(x2==x1){
        double dis=x-x1;
        return dis>0?dis:-dis;
    }
    double m=(y2-y1)/(x2-x1);
    double b=y1-m*x1;
    double dis=(m*x-y+b)/sqrt(m*m+1);
    return dis>0?dis:-dis;
}

double ptpt_dis(double x1,double y1,double x2,double y2){
    double dis=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
    return dis;
}

void new_game(Tank* tank_1,Tank* tank_2,Info* info,int* maze,Wall* walls){
    int i;
    for(i=0;i<bullet_limit;i++){
        (tank_1->bullet+i)->is_active=0;
        (tank_2->bullet+i)->is_active=0;
    }
    for(i=0;i<powerup_limit;i++){
        (info->powerup+i)->is_full=0;
    }
    maze_maker(maze,walls);
    spawn_tanks(tank_1,tank_2);
    info->p1pt=0;
    info->p2pt=0;
    error=0;
}

void new_round(Tank* tank_1,Tank* tank_2,Info* info,int* maze,Wall* walls) {
    int i;
    for (i = 0; i < bullet_limit; i++) {
        (tank_1->bullet + i)->is_active = 0;
        (tank_2->bullet + i)->is_active = 0;
    }
    for (i = 0; i < powerup_limit; i++) {
        (info->powerup + i)->is_full = 0;
    }
    maze_maker(maze,walls);
    spawn_tanks(tank_1,tank_2);
    error=0;
}

void check_tanks_dead(Tank* tank_1,Tank* tank_2,Mine* mines){
    double d;
    int i,j;
    for(i=0;i<bullet_limit;i++){
        d=sqrt((tank_1->x-(tank_1->bullet+i)->x)*(tank_1->x-(tank_1->bullet+i)->x)+(tank_1->y-(tank_1->bullet+i)->y)*(tank_1->y-(tank_1->bullet+i)->y));
        if(d<(tank_radius+bullet_radius)/2.0+2 && tank_1->is_dead==0 && (tank_1->bullet+i)->is_active==1) {
            tank_1->is_dead = 1;
            (tank_1->bullet+i)->is_active=0;
        }
        d=sqrt((tank_2->x-(tank_2->bullet+i)->x)*(tank_2->x-(tank_2->bullet+i)->x)+(tank_2->y-(tank_2->bullet+i)->y)*(tank_2->y-(tank_2->bullet+i)->y));
        if(d<(tank_radius+bullet_radius)/2.0+2 && tank_2->is_dead==0 && (tank_2->bullet+i)->is_active==1) {
            tank_2->is_dead = 1;
            (tank_2->bullet+i)->is_active=0;
        }
        d=sqrt((tank_1->x-(tank_2->bullet+i)->x)*(tank_1->x-(tank_2->bullet+i)->x)+(tank_1->y-(tank_2->bullet+i)->y)*(tank_1->y-(tank_2->bullet+i)->y));
        if(d<(tank_radius+bullet_radius)/2.0+2 && tank_1->is_dead==0 && (tank_2->bullet+i)->is_active==1) {
            tank_1->is_dead = 1;
            (tank_2->bullet+i)->is_active=0;
        }
        d=sqrt((tank_2->x-(tank_1->bullet+i)->x)*(tank_2->x-(tank_1->bullet+i)->x)+(tank_2->y-(tank_1->bullet+i)->y)*(tank_2->y-(tank_1->bullet+i)->y));
        if(d<(tank_radius+bullet_radius)/2.0+2 && tank_2->is_dead==0 && (tank_1->bullet+i)->is_active==1) {
            tank_2->is_dead = 1;
            (tank_1->bullet+i)->is_active=0;
        }
    }
    d=ptpt_dis(tank_1->fragbomb->x,tank_1->fragbomb->y,tank_1->x,tank_1->y);
    if(d<(tank_radius+bullet_radius)/2.0 && tank_1->fragbomb->is_active==1){
        tank_1->is_dead=1;
        tank_1->fragbomb->is_active=0;
        tank_1->fbomb=0;
        fragbomb_explode(tank_1->fragbomb);
    }
    d=ptpt_dis(tank_1->fragbomb->x,tank_1->fragbomb->y,tank_2->x,tank_2->y);
    if(d<(tank_radius+bullet_radius)/2.0 && tank_1->fragbomb->is_active==1){
        tank_2->is_dead=1;
        tank_1->fragbomb->is_active=0;
        tank_1->fbomb=0;
        fragbomb_explode(tank_1->fragbomb);
    }
    d=ptpt_dis(tank_2->fragbomb->x,tank_2->fragbomb->y,tank_1->x,tank_1->y);
    if(d<(tank_radius+bullet_radius)/2.0 && tank_2->fragbomb->is_active==1){
        tank_1->is_dead=1;
        tank_2->fragbomb->is_active=0;
        tank_2->fbomb=0;
        fragbomb_explode(tank_2->fragbomb);
    }
    d=ptpt_dis(tank_2->fragbomb->x,tank_2->fragbomb->y,tank_2->x,tank_2->y);
    if(d<(tank_radius+bullet_radius)/2.0 && tank_2->fragbomb->is_active==1){
        tank_2->is_dead=1;
        tank_2->fragbomb->is_active=0;
        tank_2->fbomb=0;
        fragbomb_explode(tank_2->fragbomb);
    }
    for(i=0;i<mine_limit;i++){
        for(j=0;j<frag_count;j++){
            d=ptpt_dis(tank_1->x,tank_1->y,((mines+i)->frags+j)->x,((mines+i)->frags+j)->y);
            if(d<(tank_radius+frag_radius)/2.0 && ((mines+i)->frags+j)->is_active==1){
                tank_1->is_dead=1;
                ((mines+i)->frags+j)->is_active=0;
            }
            d=ptpt_dis(tank_2->x,tank_2->y,((mines+i)->frags+j)->x,((mines+i)->frags+j)->y);
            if(d<(tank_radius+frag_radius)/2.0 && ((mines+i)->frags+j)->is_active==1){
                tank_2->is_dead=1;
                ((mines+i)->frags+j)->is_active=0;
            }
        }
    }
    for(i=0;i<frag_count;i++){
        d=ptpt_dis(tank_1->x,tank_1->y,(tank_1->fragbomb->frags+i)->x,(tank_1->fragbomb->frags+i)->y);
        if(d<(tank_radius+frag_radius)/2.0 && (tank_1->fragbomb->frags+i)->is_active==1){
            tank_1->is_dead=1;
            (tank_1->fragbomb->frags+i)->is_active=0;
        }
        d=ptpt_dis(tank_1->x,tank_1->y,(tank_2->fragbomb->frags+i)->x,(tank_2->fragbomb->frags+i)->y);
        if(d<(tank_radius+frag_radius)/2.0 && (tank_2->fragbomb->frags+i)->is_active==1){
            tank_1->is_dead=1;
            (tank_2->fragbomb->frags+i)->is_active=0;
        }
        d=ptpt_dis(tank_2->x,tank_2->y,(tank_1->fragbomb->frags+i)->x,(tank_1->fragbomb->frags+i)->y);
        if(d<(tank_radius+frag_radius)/2.0 && (tank_1->fragbomb->frags+i)->is_active==1){
            tank_2->is_dead=1;
            (tank_1->fragbomb->frags+i)->is_active=0;
        }
        d=ptpt_dis(tank_2->x,tank_2->y,(tank_2->fragbomb->frags+i)->x,(tank_2->fragbomb->frags+i)->y);
        if(d<(tank_radius+frag_radius)/2.0 && (tank_1->fragbomb->frags+i)->is_active==1){
            tank_2->is_dead=1;
            (tank_2->fragbomb->frags+i)->is_active=0;
        }
    }
}

void check_round(Tank* tank_1,Tank* tank_2,Info* info,int* maze,Wall* walls,Mine* mines){
    check_tanks_dead(tank_1,tank_2,mines);
    if(info->countdown>10000 && (tank_1->is_dead==1 || tank_2->is_dead==1))
        info->countdown=4000;
    if(info->countdown<0){
        if(tank_1->is_dead==1 && tank_2->is_dead==1){
            info->countdown=1000000;
            tank_1->is_dead=0;
            tank_2->is_dead=0;
            new_round(tank_1,tank_2,info,maze,walls);
            return ;
        }
        if(tank_1->is_dead==1 && tank_2->is_dead==0){
            info->p2pt+=1;
            info->countdown=1000000;
            tank_1->is_dead=0;
            new_round(tank_1,tank_2,info,maze,walls);
            return ;
        }
        if(tank_1->is_dead==0 && tank_2->is_dead==1){
            info->p1pt+=1;
            info->countdown=1000000;
            tank_2->is_dead=0;
            new_round(tank_1,tank_2,info,maze,walls);
            return ;
        }
    }
    if(info->countdown<10000)
        info->countdown-=passed_ticks;
}

void end_of_game(Info* info){
    if(info->p1pt==win_point || info->p2pt==win_point){
        while(1){
            SDL_Event event;
            if (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    end_program = 1;
                    return;
                }
                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        end_program=1;
                        return ;
                    }
                }
            }
            draw_endpage(info);
        }
    }
    return ;
}

double min(double a,double b){
    return a<b?a:b;
}

double max(double a,double b){
    return a>b?a:b;
}