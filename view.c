//
// Created by sepehr on 12/19/18.
//

#include <stdio.h>
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
#include "menu.h"
#include "powerup.h"
#include "maze_builder.h"

SDL_Window *window;
SDL_Renderer *renderer;

void init_window() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("ALTER_TANK", 125, 0, window_width, window_height, SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    return;
}

void quit_window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void draw_tank(Tank *tank_1, Tank *tank_2) {
    if (tank_1->is_dead == 0) {
        int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
        int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
        filledCircleRGBA(renderer, tank_1->x, tank_1->y, tank_radius, 255, 50, 100, 255);
        thickLineRGBA(renderer, tank_1->x, tank_1->y, X, Y, 6, 0, 0, 0, 255);
        if (tank_1->laser != 0)
            filledCircleRGBA(renderer, tank_1->x, tank_1->y, tank_radius / 3.0, 0, 0, 255, 255);
        if (tank_1->fbomb != 0)
            filledCircleRGBA(renderer, tank_1->x, tank_1->y, tank_radius / 3.0, 255, 0, 0, 255);
        if (tank_1->mine != 0)
            filledCircleRGBA(renderer, tank_1->x, tank_1->y, tank_radius / 3.0, 0, 255, 0, 255);
    }
    if (tank_2->is_dead == 0) {
        int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
        int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
        filledCircleRGBA(renderer, tank_2->x, tank_2->y, tank_radius, 0, 255, 0, 255);
        thickLineRGBA(renderer, tank_2->x, tank_2->y, X, Y, 6, 0, 0, 0, 255);
        if (tank_2->laser != 0)
            filledCircleRGBA(renderer, tank_2->x, tank_2->y, tank_radius / 3.0, 0, 0, 255, 255);
        if (tank_2->fbomb != 0)
            filledCircleRGBA(renderer, tank_2->x, tank_2->y, tank_radius / 3.0, 255, 0, 0, 255);
        if (tank_2->mine != 0)
            filledCircleRGBA(renderer, tank_2->x, tank_2->y, tank_radius / 3.0, 0, 255, 0, 255);
    }
}

void draw_bullet(Tank *tank_1, Tank *tank_2, Mine *mines) {
    int i;
    for (i = 0; i < bullet_limit; i++) {
        if ((tank_1->bullet + i)->is_active == 1)
            filledCircleRGBA(renderer, (tank_1->bullet + i)->x, (tank_1->bullet + i)->y, bullet_radius, 0, 0, 0, 255);
        if ((tank_2->bullet + i)->is_active == 1)
            filledCircleRGBA(renderer, (tank_2->bullet + i)->x, (tank_2->bullet + i)->y, bullet_radius, 0, 0, 0, 255);
    }
    if (tank_1->fragbomb->is_active == 1)
        filledCircleRGBA(renderer, tank_1->fragbomb->x, tank_1->fragbomb->y, fragbomb_radius, 255, 0, 0, 255);
    if (tank_1->fragbomb->is_active == 1)
        filledCircleRGBA(renderer, tank_1->fragbomb->x, tank_1->fragbomb->y, fragbomb_radius, 255, 0, 0, 255);
    for (i = 0; i < frag_count; i++) {
        if ((tank_1->fragbomb->frags + i)->is_active == 1)
            filledCircleRGBA(renderer, (tank_1->fragbomb->frags + i)->x, (tank_1->fragbomb->frags + i)->y, frag_radius,
                             255, 0, 0, 255);
        if ((tank_2->fragbomb->frags + i)->is_active == 1)
            filledCircleRGBA(renderer, (tank_2->fragbomb->frags + i)->x, (tank_2->fragbomb->frags + i)->y, frag_radius,
                             255, 0, 0, 255);
        if ((mines->frags + i)->is_active == 1)
            filledCircleRGBA(renderer, (mines->frags + i)->x, (mines->frags + i)->y, frag_radius, 0, 0, 0, 255);
    }
}

void draw_mine(Mine *mines) {
    int i;
    for (i = 0; i < mine_limit; i++) {
        if ((mines + i)->is_set == 1) {
            if ((mines + i)->trigger_time < mine_set_time)
                filledCircleRGBA(renderer, (mines + i)->x, (mines + i)->y, mine_radius, 0, 0, 0, 255);
            if ((mines + i)->trigger_time > 10000) {
                filledCircleRGBA(renderer, (mines + i)->x, (mines + i)->y, mine_radius, 0, 0, 0, 255);
                filledCircleRGBA(renderer, (mines + i)->x, (mines + i)->y, mine_radius / 3.0, 255, 0, 0, 255);
            }
        }
    }
}

void draw_wall(Wall *walls) {
    for (int i = 0; i < count_columns * count_rows * 2 + count_columns + count_rows; i++) {
        if ((walls + i)->does_exist == 1)
            thickLineRGBA(renderer, (walls + i)->x1, (walls + i)->y1, (walls + i)->x2, (walls + i)->y2, wall_size, 50,
                          50, 50, 255);
    }
}

void top_info(Tank *tank_1, Tank *tank_2, Wall *walls, Info *info) {
    srand(time(NULL));
    char *buffer = malloc(sizeof(char) * 50);
    sprintf(buffer, "point to win: %d p1 %d p2 %d frame ticks: %d |TEST| ", win_point, info->p1pt, info->p2pt,
            passed_ticks);
    //printf("%s", buffer);
    //SDL_RenderSetScale(renderer,1.5,1.5);
    stringRGBA(renderer, maze_start_x, maze_start_y - 20, buffer, 0, 0, 0, 255);
    //SDL_RenderSetScale(renderer,1,1);
}

void renderer_ready() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
}

void renderer_present() {
    SDL_RenderPresent(renderer);
}

int exit_check() {
    return end_program;
}

void handle_events(Tank *tank_1, Tank *tank_2, int *maze, Wall *walls, Info *info, Mine *mines) {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            end_program = 1;
        }
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_0:
                    if (tank_1->left_key == '0') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == '0') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == '0') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == '0') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == '0') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == '0') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == '0') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == '0') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == '0') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == '0') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_1:
                    if (tank_1->left_key == '1') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == '1') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == '1') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == '1') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == '1') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == '1') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == '1') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == '1') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == '1') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == '1') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_2:
                    if (tank_1->left_key == '2') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == '2') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == '2') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == '2') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == '2') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == '2') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == '2') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == '2') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == '2') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == '2') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_3:
                    if (tank_1->left_key == '3') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == '3') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == '3') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == '3') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == '3') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == '3') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == '3') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == '3') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == '3') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == '3') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_4:
                    if (tank_1->left_key == '4') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == '4') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == '4') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == '4') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == '4') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == '4') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == '4') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == '4') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == '4') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == '4') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_5:
                    if (tank_1->left_key == '5') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == '5') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == '5') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == '5') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == '5') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == '5') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == '5') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == '5') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == '5') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == '5') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_6:
                    if (tank_1->left_key == '6') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == '6') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == '6') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == '6') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == '6') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == '6') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == '6') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == '6') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == '6') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == '6') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_7:
                    if (tank_1->left_key == '7') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == '7') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == '7') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == '7') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == '7') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == '7') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == '7') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == '7') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == '7') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == '7') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_8:
                    if (tank_1->left_key == '8') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == '8') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == '8') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == '8') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == '8') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == '8') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == '8') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == '8') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == '8') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == '8') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_9:
                    if (tank_1->left_key == '9') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == '9') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == '9') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == '9') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == '9') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == '9') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == '9') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == '9') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == '9') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == '9') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_LEFT:
                    if (tank_1->left_key == '<') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == '<') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == '<') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == '<') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == '<') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == '<') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == '<') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == '<') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == '<') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == '<') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_RIGHT:
                    if (tank_1->left_key == '>') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == '>') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == '>') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == '>') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == '>') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == '>') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == '>') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == '>') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == '>') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == '>') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_UP:
                    if (tank_1->left_key == '^') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == '^') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == '^') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == '^') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == '^') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == '^') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == '^') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == '^') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == '^') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == '^') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_a:
                    if (tank_1->left_key == 'a') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == 'a') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == 'a') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == 'a') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == 'a') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == 'a') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == 'a') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == 'a') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == 'a') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == 'a') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_b:
                    if (tank_1->left_key == 'b') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == 'b') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == 'b') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == 'b') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == 'b') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == 'b') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == 'b') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == 'b') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == 'b') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == 'b') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_c:
                    if (tank_1->left_key == 'c') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == 'c') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == 'c') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == 'c') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == 'c') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == 'c') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == 'c') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == 'c') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == 'c') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == 'c') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_d:
                    if (tank_1->left_key == 'd') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == 'd') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == 'd') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == 'd') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == 'd') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == 'd') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == 'd') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == 'd') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == 'd') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == 'd') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_e:
                    if (tank_1->left_key == 'e') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == 'e') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == 'e') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == 'e') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == 'e') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == 'e') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == 'e') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == 'e') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == 'e') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == 'e') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_f:
                    if (tank_1->left_key == 'f') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == 'f') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == 'f') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == 'f') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == 'f') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == 'f') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == 'f') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == 'f') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == 'f') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == 'f') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_g:
                    if (tank_1->left_key == 'g') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == 'g') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == 'g') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == 'g') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == 'g') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == 'g') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == 'g') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == 'g') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == 'g') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == 'g') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_h:
                    if (tank_1->left_key == 'h') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == 'h') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == 'h') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == 'h') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == 'h') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == 'h') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == 'h') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == 'h') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == 'h') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == 'h') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_i:
                    if (tank_1->left_key == 'i') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == 'i') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == 'i') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == 'i') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == 'i') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == 'i') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == 'i') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == 'i') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == 'i') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == 'i') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_j:
                    if (tank_1->left_key == 'j') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == 'j') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == 'j') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == 'j') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == 'j') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == 'j') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == 'j') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == 'j') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == 'j') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == 'j') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_k:
                    if (tank_1->left_key == 'k') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == 'k') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == 'k') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == 'k') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == 'k') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == 'k') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == 'k') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == 'k') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == 'k') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == 'k') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_l:
                    if (tank_1->left_key == 'l') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == 'l') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == 'l') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == 'l') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == 'l') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == 'l') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == 'l') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == 'l') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == 'l') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == 'l') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_m:
                    if (tank_1->left_key == 'm') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == 'm') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == 'm') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == 'm') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == 'm') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == 'm') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == 'm') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == 'm') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == 'm') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == 'm') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_n:
                    if (tank_1->left_key == 'n') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == 'n') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == 'n') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == 'n') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == 'n') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == 'n') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == 'n') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == 'n') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == 'n') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == 'n') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_o:
                    if (tank_1->left_key == 'o') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == 'o') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == 'o') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == 'o') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == 'o') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == 'o') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == 'o') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == 'o') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == 'o') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == 'o') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_p:
                    if (tank_1->left_key == 'p') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == 'p') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == 'p') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == 'p') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == 'p') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == 'p') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == 'p') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == 'p') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == 'p') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == 'p') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_q:
                    if (tank_1->left_key == 'q') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == 'q') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == 'q') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == 'q') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == 'q') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == 'q') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == 'q') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == 'q') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == 'q') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == 'q') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_r:
                    if (tank_1->left_key == 'r') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == 'r') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == 'r') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == 'r') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == 'r') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == 'r') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == 'r') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == 'r') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == 'r') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == 'r') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_s:
                    if (tank_1->left_key == 's') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == 's') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == 's') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == 's') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == 's') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == 's') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == 's') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == 's') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == 's') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == 's') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_t:
                    if (tank_1->left_key == 't') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == 't') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == 't') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == 't') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == 't') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == 't') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == 't') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == 't') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == 't') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == 't') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_u:
                    if (tank_1->left_key == 'u') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == 'u') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == 'u') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == 'u') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == 'u') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == 'u') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == 'u') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == 'u') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == 'u') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == 'u') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_v:
                    if (tank_1->left_key == 'v') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == 'v') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == 'v') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == 'v') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == 'v') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == 'v') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == 'v') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == 'v') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == 'v') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == 'v') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_w:
                    if (tank_1->left_key == 'w') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == 'w') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == 'w') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == 'w') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == 'w') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == 'w') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == 'w') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == 'w') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == 'w') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == 'w') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_x:
                    if (tank_1->left_key == 'x') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == 'x') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == 'x') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == 'x') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == 'x') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == 'x') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == 'x') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == 'x') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == 'x') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == 'x') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_y:
                    if (tank_1->left_key == 'y') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == 'y') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == 'y') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == 'y') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == 'y') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == 'y') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == 'y') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == 'y') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == 'y') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == 'y') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_z:
                    if (tank_1->left_key == 'z') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == 'z') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == 'z') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == 'z') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == 'z') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == 'z') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == 'z') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == 'z') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == 'z') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == 'z') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_DOWN:
                    if (tank_1->left_key == '|') {
                        tank_1->turn_status = tank_1->move_status != 0 ? tank_1->move_status : 1;
                        break;
                    }
                    if (tank_1->right_key == '|') {
                        tank_1->turn_status = tank_1->move_status != 0 ? -tank_1->move_status : -1;
                        break;
                    }
                    if (tank_1->down_key == '|') {
                        tank_1->move_status = -1;
                        break;
                    }
                    if (tank_1->up_key == '|') {
                        tank_1->move_status = 1;
                        break;
                    }
                    if (tank_1->fire_key == '|') {
                        if (tank_1->laser == 3000) {
                            tank_1->laser = 2999;
                            break;
                        }
                        if (tank_1->mine != 0) {
                            (tank_1->mine)--;
                            set_mine(mines, tank_1->x, tank_1->y);
                            break;
                        }
                        if (tank_1->fbomb != 0) {
                            if (tank_1->fbomb == 1) {
                                tank_1->fbomb = 2;

                            }
                            if (tank_1->fbomb == 3) {
                                fragbomb_explode(tank_1->fragbomb);
                                tank_1->fbomb = 0;
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_1->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_1->bullet);
                            (tank_1->bullet + temp)->life_time = 0;
                            (tank_1->bullet + temp)->is_active = 1;
                            int X = (tank_1->x) + tank_radius * 1.1 * cos(tank_1->angle);
                            int Y = (tank_1->y) - tank_radius * 1.1 * sin(tank_1->angle);
                            (tank_1->bullet + temp)->x = X;
                            (tank_1->bullet + temp)->y = Y;
                            (tank_1->bullet + temp)->angle = tank_1->angle;
                        }
                        break;
                    }
                    if (tank_2->left_key == '|') {
                        tank_2->turn_status = tank_2->move_status != 0 ? tank_2->move_status : 1;
                        break;
                    }
                    if (tank_2->right_key == '|') {
                        tank_2->turn_status = tank_2->move_status != 0 ? -tank_2->move_status : -1;
                        break;
                    }
                    if (tank_2->down_key == '|') {
                        tank_2->move_status = -1;
                        break;
                    }
                    if (tank_2->up_key == '|') {
                        tank_2->move_status = 1;
                        break;
                    }
                    if (tank_2->fire_key == '|') {
                        if (tank_2->laser == 3000) {
                            tank_2->laser = 2999;
                            break;
                        }
                        if (tank_2->mine != 0) {
                            (tank_2->mine)--;
                            set_mine(mines, tank_2->x, tank_2->y);
                            break;
                        }
                        if (tank_2->fbomb != 0) {
                            if (tank_2->fbomb == 1) {
                                tank_2->fbomb = 2;
                            }
                            if (tank_2->fbomb == 3) {
                                tank_2->fbomb = 0;
                                fragbomb_explode(tank_2->fragbomb);
                            }
                            break;
                        }
                        if (tell_ready_bullet(tank_2->bullet) != -1) {
                            int temp = tell_ready_bullet(tank_2->bullet);
                            (tank_2->bullet + temp)->life_time = 0;
                            (tank_2->bullet + temp)->is_active = 1;
                            int X = (tank_2->x) + tank_radius * 1.1 * cos(tank_2->angle);
                            int Y = (tank_2->y) - tank_radius * 1.1 * sin(tank_2->angle);
                            (tank_2->bullet + temp)->x = X;
                            (tank_2->bullet + temp)->y = Y;
                            (tank_2->bullet + temp)->angle = tank_2->angle;
                        }
                        break;
                    }
                    break;
                case SDLK_ESCAPE:
                    is_pause = 1;
                    which_page = 1;
                    menu(tank_1, tank_2, maze, walls, info);
                    break;
            }
        }
        if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
                case SDLK_0:
                    if (tank_1->left_key == '0') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == '0') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == '0') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == '0') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == '0') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == '0') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == '0') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == '0') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_1:
                    if (tank_1->left_key == '1') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == '1') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == '1') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == '1') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == '1') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == '1') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == '1') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == '1') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_2:
                    if (tank_1->left_key == '2') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == '2') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == '2') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == '2') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == '2') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == '2') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == '2') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == '2') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_3:
                    if (tank_1->left_key == '3') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == '3') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == '3') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == '3') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == '3') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == '3') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == '3') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == '3') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_4:
                    if (tank_1->left_key == '4') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == '4') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == '4') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == '4') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == '4') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == '4') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == '4') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == '4') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_5:
                    if (tank_1->left_key == '5') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == '5') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == '5') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == '5') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == '5') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == '5') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == '5') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == '5') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_6:
                    if (tank_1->left_key == '6') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == '6') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == '6') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == '6') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == '6') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == '6') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == '6') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == '6') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_7:
                    if (tank_1->left_key == '7') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == '7') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == '7') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == '7') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == '7') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == '7') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == '7') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == '7') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_8:
                    if (tank_1->left_key == '8') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == '8') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == '8') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == '8') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == '8') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == '8') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == '8') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == '8') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_9:
                    if (tank_1->left_key == '9') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == '9') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == '9') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == '9') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == '9') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == '9') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == '9') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == '9') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_LEFT:
                    if (tank_1->left_key == '<') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == '<') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == '<') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == '<') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == '<') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == '<') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == '<') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == '<') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_RIGHT:
                    if (tank_1->left_key == '>') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == '>') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == '>') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == '>') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == '>') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == '>') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == '>') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == '>') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_UP:
                    if (tank_1->left_key == '^') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == '^') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == '^') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == '^') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == '^') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == '^') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == '^') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == '^') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_a:
                    if (tank_1->left_key == 'a') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == 'a') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == 'a') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == 'a') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == 'a') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == 'a') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == 'a') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == 'a') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_b:
                    if (tank_1->left_key == 'b') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == 'b') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == 'b') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == 'b') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == 'b') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == 'b') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == 'b') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == 'b') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_c:
                    if (tank_1->left_key == 'c') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == 'c') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == 'c') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == 'c') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == 'c') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == 'c') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == 'c') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == 'c') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_d:
                    if (tank_1->left_key == 'd') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == 'd') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == 'd') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == 'd') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == 'd') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == 'd') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == 'd') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == 'd') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_e:
                    if (tank_1->left_key == 'e') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == 'e') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == 'e') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == 'e') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == 'e') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == 'e') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == 'e') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == 'e') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_f:
                    if (tank_1->left_key == 'f') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == 'f') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == 'f') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == 'f') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == 'f') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == 'f') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == 'f') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == 'f') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_g:
                    if (tank_1->left_key == 'g') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == 'g') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == 'g') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == 'g') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == 'g') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == 'g') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == 'g') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == 'g') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_h:
                    if (tank_1->left_key == 'h') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == 'h') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == 'h') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == 'h') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == 'h') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == 'h') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == 'h') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == 'h') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_i:
                    if (tank_1->left_key == 'i') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == 'i') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == 'i') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == 'i') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == 'i') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == 'i') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == 'i') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == 'i') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_j:
                    if (tank_1->left_key == 'j') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == 'j') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == 'j') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == 'j') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == 'j') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == 'j') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == 'j') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == 'j') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_k:
                    if (tank_1->left_key == 'k') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == 'k') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == 'k') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == 'k') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == 'k') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == 'k') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == 'k') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == 'k') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_l:
                    if (tank_1->left_key == 'l') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == 'l') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == 'l') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == 'l') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == 'l') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == 'l') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == 'l') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == 'l') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_m:
                    if (tank_1->left_key == 'm') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == 'm') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == 'm') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == 'm') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == 'm') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == 'm') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == 'm') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == 'm') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_n:
                    if (tank_1->left_key == 'n') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == 'n') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == 'n') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == 'n') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == 'n') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == 'n') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == 'n') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == 'n') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_o:
                    if (tank_1->left_key == 'o') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == 'o') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == 'o') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == 'o') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == 'o') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == 'o') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == 'o') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == 'o') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_p:
                    if (tank_1->left_key == 'p') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == 'p') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == 'p') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == 'p') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == 'p') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == 'p') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == 'p') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == 'p') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_q:
                    if (tank_1->left_key == 'q') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == 'q') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == 'q') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == 'q') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == 'q') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == 'q') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == 'q') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == 'q') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_r:
                    if (tank_1->left_key == 'r') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == 'r') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == 'r') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == 'r') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == 'r') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == 'r') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == 'r') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == 'r') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_s:
                    if (tank_1->left_key == 's') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == 's') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == 's') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == 's') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == 's') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == 's') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == 's') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == 's') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_t:
                    if (tank_1->left_key == 't') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == 't') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == 't') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == 't') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == 't') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == 't') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == 't') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == 't') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_u:
                    if (tank_1->left_key == 'u') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == 'u') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == 'u') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == 'u') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == 'u') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == 'u') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == 'u') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == 'u') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_v:
                    if (tank_1->left_key == 'v') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == 'v') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == 'v') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == 'v') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == 'v') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == 'v') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == 'v') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == 'v') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_w:
                    if (tank_1->left_key == 'w') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == 'w') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == 'w') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == 'w') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == 'w') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == 'w') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == 'w') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == 'w') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_x:
                    if (tank_1->left_key == 'x') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == 'x') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == 'x') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == 'x') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == 'x') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == 'x') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == 'x') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == 'x') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_y:
                    if (tank_1->left_key == 'y') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == 'y') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == 'y') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == 'y') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == 'y') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == 'y') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == 'y') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == 'y') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_z:
                    if (tank_1->left_key == 'z') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == 'z') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == 'z') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == 'z') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == 'z') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == 'z') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == 'z') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == 'z') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_DOWN:
                    if (tank_1->left_key == '|') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == '|') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == '|') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == '|') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == '|') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == '|') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == '|') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == '|') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
                case SDLK_SPACE:
                    if (tank_1->left_key == ' ') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->right_key == ' ') {
                        tank_1->turn_status = 0;
                        break;
                    }
                    if (tank_1->down_key == ' ') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_1->up_key == ' ') {
                        tank_1->move_status = 0;
                        break;
                    }
                    if (tank_2->left_key == ' ') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->right_key == ' ') {
                        tank_2->turn_status = 0;
                        break;
                    }
                    if (tank_2->down_key == ' ') {
                        tank_2->move_status = 0;
                        break;
                    }
                    if (tank_2->up_key == ' ') {
                        tank_2->move_status = 0;
                        break;
                    }
                    break;
            }
        }
    }
}

void draw_page_main(int keynum, int win_point_temp) {
    char buffer[10];
    sprintf(buffer, "%02d", win_point_temp);
    roundedBoxRGBA(renderer, 0, 0, window_width, window_height, 0, 255, 180, 180, 255);
    rectangleRGBA(renderer, 55, 90, 195, 150, keynum == 1 ? 255 : 0, 0, 0, 255);
    stringRGBA(renderer, 65, 100, "New Game", keynum == 1 ? 255 : 0, 0, 0, 255);
    stringRGBA(renderer, 220, 115, "Point To Win:", 0, 0, 0, 255);
    rectangleRGBA(renderer, 340, 95, 380, 145, keynum == 5 ? 255 : 0, 0, 0, 255);
    stringRGBA(renderer, 352, 115, buffer, keynum == 5 ? 255 : 0, 0, 0, 255);
    rectangleRGBA(renderer, 55, 185, 195, 245, keynum == 2 ? 255 : 0, 0, 0, 255);
    stringRGBA(renderer, 65, 195, "Save & Load", keynum == 2 ? 255 : 0, 0, 0, 255);
    stringRGBA(renderer, 65, 305, "Control Options:", 0, 0, 0, 255);
    rectangleRGBA(renderer, 220, 280, 360, 340, keynum == 3 ? 255 : 0, 0, 0, 255);
    stringRGBA(renderer, 230, 290, "S1", keynum == 3 ? 255 : 0, 0, 0, 255);
    rectangleRGBA(renderer, 395, 280, 535, 340, keynum == 6 ? 255 : 0, 0, 0, 255);
    stringRGBA(renderer, 405, 290, "S2", keynum == 6 ? 255 : 0, 0, 0, 255);
    rectangleRGBA(renderer, 55, 375, 195, 435, keynum == 4 ? 255 : 0, 0, 0, 255);
    stringRGBA(renderer, 65, 385, "Quit", keynum == 4 ? 255 : 0, 0, 0, 255);
}

void draw_page_SL(int keynum, int selected) {
    roundedBoxRGBA(renderer, 0, 0, window_width, window_height, 0, 255, 180, 180, 255);
    rectangleRGBA(renderer, 75, 60, 450, 260, keynum == 1 ? 255 : 0, 0, selected == 1 && keynum != 1 ? 255 : 0, 255);
    stringRGBA(renderer, 85, 70, "Save 1", keynum == 1 ? 255 : 0, 0, selected == 1 && keynum != 1 ? 255 : 0, 255);
    rectangleRGBA(renderer, 525, 60, 900, 260, keynum == 5 ? 255 : 0, 0, selected == 5 && keynum != 5 ? 255 : 0, 255);
    stringRGBA(renderer, 535, 70, "Save 2", keynum == 5 ? 255 : 0, 0, selected == 5 && keynum != 5 ? 255 : 0, 255);
    rectangleRGBA(renderer, 75, 280, 450, 480, keynum == 2 ? 255 : 0, 0, selected == 2 && keynum != 2 ? 255 : 0, 255);
    stringRGBA(renderer, 85, 290, "Save 3", keynum == 2 ? 255 : 0, 0, selected == 2 && keynum != 2 ? 255 : 0, 255);
    rectangleRGBA(renderer, 525, 280, 900, 480, keynum == 6 ? 255 : 0, 0, selected == 6 && keynum != 6 ? 255 : 0, 255);
    stringRGBA(renderer, 535, 290, "Save 4", keynum == 6 ? 255 : 0, 0, selected == 6 && keynum != 6 ? 255 : 0, 255);
    rectangleRGBA(renderer, 75, 500, 450, 700, keynum == 3 ? 255 : 0, 0, selected == 3 && keynum != 3 ? 255 : 0, 255);
    stringRGBA(renderer, 85, 510, "Save 5", keynum == 3 ? 255 : 0, 0, selected == 3 && keynum != 3 ? 255 : 0, 255);
    rectangleRGBA(renderer, 525, 500, 900, 700, keynum == 7 ? 255 : 0, 0, selected == 7 && keynum != 7 ? 255 : 0, 255);
    stringRGBA(renderer, 535, 510, "Save 6", keynum == 7 ? 255 : 0, 0, selected == 7 && keynum != 7 ? 255 : 0, 255);
    rectangleRGBA(renderer, 250, 750, 400, 800, keynum == 4 && game_start == 1 ? 255 : 0, 0, 0,
                  game_start == 1 ? 255 : 100);
    stringRGBA(renderer, 260, 760, "Save", keynum == 4 && game_start == 1 ? 255 : 0, 0, 0, game_start == 1 ? 255 : 100);
    rectangleRGBA(renderer, 575, 750, 725, 800, keynum == 8 ? 255 : 0, 0, 0, 255);
    stringRGBA(renderer, 585, 760, "Load", keynum == 8 ? 255 : 0, 0, 0, 255);
}

void draw_page_s1(int keynum) {
    char buffer1[10], buffer2[10];
    sprintf(buffer1, "%d", p1_keyset);
    sprintf(buffer2, "%d", p2_keyset);
    roundedBoxRGBA(renderer, 0, 0, window_width, window_height, 0, 255, 180, 180, 255);
    stringRGBA(renderer, 100, 100, "Player 1 set of keys:", keynum == 1 ? 255 : 0, 0, 0, 255);
    rectangleRGBA(renderer, 300, 75, 360, 125, keynum == 1 ? 255 : 0, 0, 0, 255);
    stringRGBA(renderer, 325, 95, buffer1, keynum == 1 ? 255 : 0, 0, 0, 255);
    stringRGBA(renderer, 450, 100, "Player 2 set of keys:", keynum == 2 ? 255 : 0, 0, 0, 255);
    rectangleRGBA(renderer, 650, 75, 710, 125, keynum == 2 ? 255 : 0, 0, 0, 255);
    stringRGBA(renderer, 675, 95, buffer2, keynum == 2 ? 255 : 0, 0, 0, 255);
    rectangleRGBA(renderer, 800, 75, 925, 125, keynum == 3 ? 255 : 0, 0, 0, 255);
    stringRGBA(renderer, 820, 95, "Set Changes", keynum == 3 ? 255 : 0, 0, 0, 255);
    stringRGBA(renderer, 100, 200, "1.", 0, 0, 0, 255);
    stringRGBA(renderer, 100, 220, "UP: UP_KEY", 0, 0, 0, 255);
    stringRGBA(renderer, 100, 240, "DOWN: DOWN_KEY", 0, 0, 0, 255);
    stringRGBA(renderer, 100, 260, "RIGHT: RIGHT_KEY", 0, 0, 0, 255);
    stringRGBA(renderer, 100, 280, "LEFT: LEFT_KEY", 0, 0, 0, 255);
    stringRGBA(renderer, 100, 300, "FIRE: P", 0, 0, 0, 255);
    stringRGBA(renderer, 400, 200, "2.", 0, 0, 0, 255);
    stringRGBA(renderer, 400, 220, "UP: H", 0, 0, 0, 255);
    stringRGBA(renderer, 400, 240, "DOWN: N", 0, 0, 0, 255);
    stringRGBA(renderer, 400, 260, "RIGHT: M", 0, 0, 0, 255);
    stringRGBA(renderer, 400, 280, "LEFT: B", 0, 0, 0, 255);
    stringRGBA(renderer, 400, 300, "FIRE: G", 0, 0, 0, 255);
    stringRGBA(renderer, 700, 200, "3.", 0, 0, 0, 255);
    stringRGBA(renderer, 700, 220, "UP: W", 0, 0, 0, 255);
    stringRGBA(renderer, 700, 240, "DOWN: S", 0, 0, 0, 255);
    stringRGBA(renderer, 700, 260, "RIGHT: D", 0, 0, 0, 255);
    stringRGBA(renderer, 700, 280, "LEFT: A", 0, 0, 0, 255);
    stringRGBA(renderer, 700, 300, "FIRE: Q", 0, 0, 0, 255);
}

void draw_page_s2(int keynum) {
    roundedBoxRGBA(renderer, 0, 0, window_width, window_height, 0, 255, 180, 180, 255);
    stringRGBA(renderer, 50, 75, "Player 1 UP_KEY:", keynum == 1 ? 255 : 0, 0, 0, 255);
    rectangleRGBA(renderer, 210, 50, 270, 100, keynum == 1 ? 255 : 0, 0, 0, 255);
    stringRGBA(renderer, 50, 150, "Player 1 DOWN_KEY:", keynum == 2 ? 255 : 0, 0, 0, 255);
    rectangleRGBA(renderer, 210, 125, 270, 175, keynum == 2 ? 255 : 0, 0, 0, 255);
    stringRGBA(renderer, 50, 225, "Player 1 RIGHT_KEY:", keynum == 3 ? 255 : 0, 0, 0, 255);
    rectangleRGBA(renderer, 210, 200, 270, 250, keynum == 3 ? 255 : 0, 0, 0, 255);
    stringRGBA(renderer, 50, 300, "Player 1 LEFT_KEY:", keynum == 4 ? 255 : 0, 0, 0, 255);
    rectangleRGBA(renderer, 210, 275, 270, 325, keynum == 4 ? 255 : 0, 0, 0, 255);
    stringRGBA(renderer, 50, 375, "Player 1 FIRE_KEY:", keynum == 5 ? 255 : 0, 0, 0, 255);
    rectangleRGBA(renderer, 210, 350, 270, 400, keynum == 5 ? 255 : 0, 0, 0, 255);
    stringRGBA(renderer, 50, 450, "Player 2 UP_KEY:", keynum == 6 ? 255 : 0, 0, 0, 255);
    rectangleRGBA(renderer, 210, 425, 270, 475, keynum == 6 ? 255 : 0, 0, 0, 255);
    stringRGBA(renderer, 50, 525, "Player 2 DOWN_KEY:", keynum == 7 ? 255 : 0, 0, 0, 255);
    rectangleRGBA(renderer, 210, 500, 270, 550, keynum == 7 ? 255 : 0, 0, 0, 255);
    stringRGBA(renderer, 50, 600, "Player 2 RIGHT_KEY:", keynum == 8 ? 255 : 0, 0, 0, 255);
    rectangleRGBA(renderer, 210, 575, 270, 625, keynum == 8 ? 255 : 0, 0, 0, 255);
    stringRGBA(renderer, 50, 675, "Player 2 LEFT_KEY:", keynum == 9 ? 255 : 0, 0, 0, 255);
    rectangleRGBA(renderer, 210, 650, 270, 700, keynum == 9 ? 255 : 0, 0, 0, 255);
    stringRGBA(renderer, 50, 750, "Player 2 FIRE_KEY:", keynum == 10 ? 255 : 0, 0, 0, 255);
    rectangleRGBA(renderer, 210, 725, 270, 775, keynum == 10 ? 255 : 0, 0, 0, 255);
}

void draw_powerups(Powerup *powerup) {
    int i;
    for (i = 0; i < powerup_limit; i++) {
        if ((powerup + i)->is_full == 1) {
            int t = (powerup + i)->type;
            filledCircleRGBA(renderer, (powerup + i)->x, (powerup + i)->y, powerup_radius, t == 1 ? 255 : 0,
                             t == 3 ? 255 : 0, t == 2 ? 255 : 0, 255);
        }
    }
}

void draw_laser(double x1, double y1, double x2, double y2) {
    int random = rand() % 100;
    thickLineRGBA(renderer, x1, y1, x2, y2, laser_radius, random, random, 255, 255);
}

void draw_prelaser(double x1, double y1, double x2, double y2) {
    thickLineRGBA(renderer, x1, y1, x2, y2, 5, 150, 255, 150, 255);
}

void draw_endpage(Info *info) {
    renderer_ready();
    rectangleRGBA(renderer, 0, 0, window_width, window_height, 255, 180, 180, 255);
    if (info->p1pt == win_point) {
        char buffer[10];
        stringRGBA(renderer, 50, 50, "Player 1:", 0, 0, 0, 255);
        sprintf(buffer, "%d", info->p1pt);
        stringRGBA(renderer, 150, 50, buffer, 0, 0, 0, 255);
        stringRGBA(renderer, 50, 125, "Player 2:", 0, 0, 0, 255);
        sprintf(buffer, "%d", info->p2pt);
        stringRGBA(renderer, 150, 125, buffer, 0, 0, 0, 255);
    } else {
        char buffer[10];
        stringRGBA(renderer, 50, 50, "Player 2:", 0, 0, 0, 255);
        sprintf(buffer, "%d", info->p2pt);
        stringRGBA(renderer, 150, 50, buffer, 0, 0, 0, 255);
        stringRGBA(renderer, 50, 125, "Player 1:", 0, 0, 0, 255);
        sprintf(buffer, "%d", info->p1pt);
        stringRGBA(renderer, 150, 125, buffer, 0, 0, 0, 255);
    }
    renderer_present();
}