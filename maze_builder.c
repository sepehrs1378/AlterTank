//
// Created by sepehr on 12/24/18.
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

int count_set = 1;

void maze_maker(int *maze, Wall *walls) {
    count_columns = (rand() % 4) + 7;
    count_rows = (rand() % 4) + 7;
    maze_start_x = (window_width - count_columns * cell_size) / 2;
    maze_start_y = (window_height - count_rows * cell_size) / 2;
    int i, j;
    for (i = 0; i <= count_rows * (2 * count_columns + 1) + count_columns - 1; i++) {
        (walls + i)->does_exist = 1;
    }
    for (i = 0; i <= count_rows * count_columns - 1; i++) {
        *(maze + i) = 0;
    }
    for (i = 1; i <= count_rows - 1; i++) {
        row_set(maze, i);
        row_random(maze, i, walls);
        vertical_random(maze, i, walls);
    }
    row_set(maze, count_rows);
    for (i = (count_rows - 1) * count_columns; i <= count_rows * count_columns - 2; i++) {
        if (*(maze + i) != *(maze + i + 1)) {
            (walls + ((i + 1) * 2 + count_rows - 1 + count_rows * count_columns - 1 - i))->does_exist = 0;
            int temp1 = *(maze + i);
            int temp2 = *(maze + i + 1);
            for (j = count_rows - 1 * count_columns; j <= count_rows * count_columns - 2; j++) {
                if (*(maze + j) == temp2) *(maze + j) = temp1;
            }
        }
    }
    walls_coordinate(walls);
}

void row_set(int *maze, int current_row) {
    for (int i = (current_row - 1) * count_columns; i <= current_row * count_columns - 1; i++) {
        if (*(maze + i) == 0) {
            *(maze + i) = count_set;
            count_set++;
        }
    }
}

void row_random(int *maze, int current_row, Wall *walls) {
    int i, j;
    for (i = (current_row - 1) * count_columns; i <= current_row * count_columns - 2; i++) {
        if (rand() % 2 == 0) {
            int temp1 = *(maze + i + 1);
            int temp2 = *(maze + i);
            for (j = (current_row - 1) * count_columns; j <= current_row * count_columns - 1; j++) {
                if (*(maze + j) == temp1) *(maze + j) = temp2;
            }
            (walls + ((i + 1) * 2 + current_row - 1 + current_row * count_columns - 1 - i))->does_exist = 0;
        }
    }
}

void vertical_random(int *maze, int current_row, Wall *walls) {
    int count = 1, random, i, j, temp;
    for (i = (current_row - 1) * count_columns; i <= current_row * count_columns - 1; i++) {
        if (*(maze + i) == *(maze + i + 1) && i != current_row * count_columns - 1) {
            count++;
        } else {
            temp = (rand() % count) + 1;
            for (j = 1; j <= temp; j++) {
                random = (rand() % count) + i + 1 - count + count_columns;
                *(maze + random) = *(maze + i);
                (walls + current_row * (2 * count_columns + 1) + random - current_row * count_columns)->does_exist = 0;
            }
            count = 1;
        }
    }
}

void walls_coordinate(Wall *walls) {
    int i, j;
    for (i = 1; i <= count_rows; i++) {
        for (j = 0; j <= count_columns - 1; j++) {
            (walls + ((i - 1) * (2 * count_columns + 1) + j))->x1 = maze_start_x + j * cell_size;
            (walls + ((i - 1) * (2 * count_columns + 1) + j))->x2 = maze_start_x + (j + 1) * cell_size;
            (walls + ((i - 1) * (2 * count_columns + 1) + j))->y1 = maze_start_y + (i - 1) * cell_size;
            (walls + ((i - 1) * (2 * count_columns + 1) + j))->y2 = maze_start_y + (i - 1) * cell_size;
        }
        for (j = count_columns; j <= 2 * count_columns; j++) {
            (walls + ((i - 1) * (2 * count_columns + 1) + j))->x1 = maze_start_x + (j - count_columns) * cell_size;
            (walls + ((i - 1) * (2 * count_columns + 1) + j))->x2 = maze_start_x + (j - count_columns) * cell_size;
            (walls + ((i - 1) * (2 * count_columns + 1) + j))->y1 = maze_start_y + (i - 1) * cell_size;
            (walls + ((i - 1) * (2 * count_columns + 1) + j))->y2 = maze_start_y + i * cell_size;
        }
    }
    for (j = 0; j <= count_columns - 1; j++) {
        (walls + (count_rows * (2 * count_columns + 1) + j))->x1 = maze_start_x + j * cell_size;
        (walls + (count_rows * (2 * count_columns + 1) + j))->x2 = maze_start_x + (j + 1) * cell_size;
        (walls + (count_rows * (2 * count_columns + 1) + j))->y1 = maze_start_y + count_rows * cell_size;
        (walls + (count_rows * (2 * count_columns + 1) + j))->y2 = maze_start_y + count_rows * cell_size;
    }
}