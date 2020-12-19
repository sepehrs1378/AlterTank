//
// Created by sepehr on 1/29/19.
//

#include <stdio.h>
#include <stdlib.h>
#include "SaveLoad.h"
#include "structs.h"
#include "maze_builder.h"

void save(Info *info, Wall *walls, Tank *tank_1, Tank *tank_2, char name[]) {
    int i, j;
    FILE *fsave = fopen(name, "w");
    if (fsave == NULL) {
        end_program = 1;
        return;
    }
    fprintf(fsave, "%d %d %d %d\n", win_point, info->p1pt, info->p2pt, info->countdown);
    fprintf(fsave, "%d %d\n", count_rows, count_columns);
    for (i = 1; i <= count_rows; i++) {
        for (j = 0; j <= count_columns - 1; j++) {
            fprintf(fsave, "%d ", (walls + ((i - 1) * (2 * count_columns + 1) + j))->does_exist);
        }
        fprintf(fsave, "\n");
        for (j = count_columns; j <= 2 * count_columns; j++) {
            fprintf(fsave, "%d ", (walls + ((i - 1) * (2 * count_columns + 1) + j))->does_exist);
        }
        fprintf(fsave, "\n");
    }
    fprintf(fsave, "\n");
    for (j = 0; j <= count_columns - 1; j++) {
        fprintf(fsave, "%d ", (walls + (count_rows * (2 * count_columns + 1) + j))->does_exist);
    }
    fprintf(fsave, "\n");
    fprintf(fsave, "%lf %lf %lf %d %d %d %d\n", tank_1->angle, tank_1->x, tank_1->y, tank_1->is_dead, tank_1->fbomb,
            tank_1->laser, tank_1->mine);
    for (i = 0; i < bullet_limit; i++) {
        fprintf(fsave, "%lf %lf %lf %d %d\n", (tank_1->bullet + i)->x, (tank_1->bullet + i)->y,
                (tank_1->bullet + i)->angle, (tank_1->bullet + i)->life_time, (tank_1->bullet + i)->is_active);
    }
    fprintf(fsave, "%lf %lf %lf %d %d %d %d\n", tank_2->angle, tank_2->x, tank_2->y, tank_2->is_dead, tank_2->fbomb,
            tank_2->laser, tank_2->mine);
    for (i = 0; i < bullet_limit; i++) {
        fprintf(fsave, "%lf %lf %lf %d %d\n", (tank_2->bullet + i)->x, (tank_2->bullet + i)->y,
                (tank_2->bullet + i)->angle, (tank_2->bullet + i)->life_time, (tank_2->bullet + i)->is_active);
    }
    for (i = 0; i < powerup_limit; i++) {
        fprintf(fsave, "%lf %lf %d %d\n", (info->powerup + i)->x, (info->powerup + i)->y, (info->powerup + i)->type,
                (info->powerup + i)->is_full);
    }
    fclose(fsave);
}

void load(Info *info, Wall *walls, Tank *tank_1, Tank *tank_2, char name[]) {
    int i, j;
    FILE *fload = fopen(name, "r");
    if (fload == NULL) {
        end_program = 1;
        return;
    }
    fscanf(fload, "%d %d %d %d\n", &win_point, &info->p1pt, &info->p2pt, &info->countdown);
    fscanf(fload, "%d %d\n", &count_rows, &count_columns);
    maze_start_x = (window_width - count_columns * cell_size) / 2;
    maze_start_y = (window_height - count_rows * cell_size) / 2;
    walls_coordinate(walls);
    for (i = 1; i <= count_rows; i++) {
        for (j = 0; j <= count_columns - 1; j++) {
            fscanf(fload, "%d ", &(walls + ((i - 1) * (2 * count_columns + 1) + j))->does_exist);
        }
        fscanf(fload, "\n");
        for (j = count_columns; j <= 2 * count_columns; j++) {
            fscanf(fload, "%d ", &(walls + ((i - 1) * (2 * count_columns + 1) + j))->does_exist);
        }
        fscanf(fload, "\n");
    }
    fscanf(fload, "\n");
    for (j = 0; j <= count_columns - 1; j++) {
        fscanf(fload, "%d ", &(walls + (count_rows * (2 * count_columns + 1) + j))->does_exist);
    }
    fscanf(fload, "\n");
    fscanf(fload, "%lf %lf %lf %d %d %d %d\n", &tank_1->angle, &tank_1->x, &tank_1->y, &tank_1->is_dead, &tank_1->fbomb,
           &tank_1->laser, &tank_1->mine);
    for (i = 0; i < bullet_limit; i++) {
        fscanf(fload, "%lf %lf %lf %d %d\n", &(tank_1->bullet + i)->x, &(tank_1->bullet + i)->y,
               &(tank_1->bullet + i)->angle, &(tank_1->bullet + i)->life_time, &(tank_1->bullet + i)->is_active);
    }
    fscanf(fload, "%lf %lf %lf %d %d %d %d\n", &tank_2->angle, &tank_2->x, &tank_2->y, &tank_2->is_dead, &tank_2->fbomb,
           &tank_2->laser, &tank_2->mine);
    for (i = 0; i < bullet_limit; i++) {
        fscanf(fload, "%lf %lf %lf %d %d\n", &(tank_2->bullet + i)->x, &(tank_2->bullet + i)->y,
               &(tank_2->bullet + i)->angle, &(tank_2->bullet + i)->life_time, &(tank_2->bullet + i)->is_active);
    }
    for (i = 0; i < powerup_limit; i++) {
        fscanf(fload, "%lf %lf %d %d\n", &(info->powerup + i)->x, &(info->powerup + i)->y, &(info->powerup + i)->type,
               &(info->powerup + i)->is_full);
    }
    fclose(fload);
}