//
// Created by sepehr on 1/26/19.
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
#include "SaveLoad.h"

int selected = 1;
int keynum;
int win_point_temp = 5;

void menu(Tank *tank_1, Tank *tank_2, int *maze, Wall *walls, Info *info) {
    keynum = 1;
    win_point_temp = 5;
    while (1) {
        renderer_ready();
        handle_keys(tank_1, tank_2, maze, walls, info);
        if (is_pause == 0)
            break;
        switch (which_page) {
            case 1:
                draw_page_main(keynum, win_point_temp);
                break;
            case 2:
                draw_page_SL(keynum, selected);
                break;
            case 3:
                draw_page_s1(keynum);
                break;
            case 4:
                draw_page_s2(keynum);
                break;
        }
        renderer_present();
        SDL_Delay(1000 / fps);
    }
    return;
}

void handle_keys(Tank *tank_1, Tank *tank_2, int *maze, Wall *walls, Info *info) {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        switch (which_page) {
            case 1:
                if (event.type == SDL_QUIT) {
                    end_program = 1;
                    is_pause = 0;
                }
                if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            if (game_start == 0)
                                break;
                            is_pause = 0;
                            break;
                        case SDLK_UP:
                            switch (keynum) {
                                case 2:
                                    keynum = 1;
                                    break;
                                case 3:
                                    keynum = 2;
                                    break;
                                case 4:
                                    keynum = 3;
                                    break;
                                case 5:
                                    win_point_temp++;
                                    break;
                            }
                            break;
                        case SDLK_DOWN:
                            switch (keynum) {
                                case 1:
                                    keynum = 2;
                                    break;
                                case 2:
                                    keynum = 3;
                                    break;
                                case 3:
                                    keynum = 4;
                                    break;
                                case 5:
                                    win_point_temp = win_point_temp > 1 ? win_point_temp - 1 : 1;
                                    break;
                            }
                            break;
                        case SDLK_RIGHT:
                            switch (keynum) {
                                case 1:
                                    keynum = 5;
                                    break;
                                case 3:
                                    keynum = 6;
                                    break;
                            }
                            break;
                        case SDLK_LEFT:
                            switch (keynum) {
                                case 5:
                                    keynum = 1;
                                    break;
                                case 6:
                                    keynum = 3;
                                    break;
                            }
                            break;
                        case SDLK_RETURN:
                            switch (keynum) {
                                case 1:
                                    keynum = 1;
                                    win_point = win_point_temp;
                                    new_game(tank_1, tank_2, info, maze, walls);
                                    is_pause = 0;
                                    game_start = 1;
                                case 2:
                                    keynum = 1;
                                    which_page = 2;
                                    break;
                                case 3:
                                    keynum = 1;
                                    which_page = 3;
                                    break;
                                case 4:
                                    keynum = 1;
                                    is_pause = 0;
                                    end_program = 1;
                                    break;
                                case 6:
                                    keynum = 1;
                                    which_page = 4;
                                    break;
                            }
                            break;
                    }
                }
                if (event.type == SDL_KEYUP) {
                    switch (event.key.keysym.sym) {

                    }
                }
                break;
            case 2:
                if (event.type == SDL_QUIT) {
                    end_program = 1;
                    is_pause = 0;
                }
                if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            keynum = 1;
                            selected = 1;
                            which_page = 1;
                            break;
                        case SDLK_UP:
                            switch (keynum) {
                                case 2:
                                    keynum = 1;
                                    break;
                                case 3:
                                    keynum = 2;
                                    break;
                                case 4:
                                    keynum = 3;
                                    break;
                                case 6:
                                    keynum = 5;
                                    break;
                                case 7:
                                    keynum = 6;
                                    break;
                                case 8:
                                    keynum = 7;
                                    break;
                            }
                            break;
                        case SDLK_DOWN:
                            switch (keynum) {
                                case 1:
                                    keynum = 2;
                                    break;
                                case 2:
                                    keynum = 3;
                                    break;
                                case 3:
                                    if (game_start == 1)
                                        keynum = 4;
                                    break;
                                case 5:
                                    keynum = 6;
                                    break;
                                case 6:
                                    keynum = 7;
                                    break;
                                case 7:
                                    keynum = 8;
                                    break;
                            }
                            break;
                        case SDLK_RIGHT:
                            switch (keynum) {
                                case 1:
                                    keynum = 5;
                                    break;
                                case 2:
                                    keynum = 6;
                                    break;
                                case 3:
                                    keynum = 7;
                                    break;
                                case 4:
                                    keynum = 8;
                                    break;
                            }
                            break;
                        case SDLK_LEFT:
                            switch (keynum) {
                                case 5:
                                    keynum = 1;
                                    break;
                                case 6:
                                    keynum = 2;
                                    break;
                                case 7:
                                    keynum = 3;
                                    break;
                                case 8:
                                    if (game_start == 1)
                                        keynum = 4;
                                    break;
                            }
                            break;
                        case SDLK_RETURN:
                            if (keynum != 4 && keynum != 8)
                                selected = keynum;
                            if (keynum == 4) {
                                switch (selected) {
                                    case 1:
                                        save(info, walls, tank_1, tank_2, "../save1.txt");
                                        break;
                                    case 2:
                                        save(info, walls, tank_1, tank_2, "../save3.txt");
                                        break;
                                    case 3:
                                        save(info, walls, tank_1, tank_2, "../save5.txt");
                                        break;
                                    case 5:
                                        save(info, walls, tank_1, tank_2, "../save2.txt");
                                        break;
                                    case 6:
                                        save(info, walls, tank_1, tank_2, "../save4.txt");
                                        break;
                                    case 7:
                                        save(info, walls, tank_1, tank_2, "../save6.txt");
                                        break;
                                }
                            }
                            if (keynum == 8) {
                                switch (selected) {
                                    case 1:
                                        load(info, walls, tank_1, tank_2, "../save1.txt");
                                        break;
                                    case 2:
                                        load(info, walls, tank_1, tank_2, "../save3.txt");
                                        break;
                                    case 3:
                                        load(info, walls, tank_1, tank_2, "../save5.txt");
                                        break;
                                    case 5:
                                        load(info, walls, tank_1, tank_2, "../save2.txt");
                                        break;
                                    case 6:
                                        load(info, walls, tank_1, tank_2, "../save4.txt");
                                        break;
                                    case 7:
                                        load(info, walls, tank_1, tank_2, "../save6.txt");
                                        break;
                                }
                                game_start = 1;
                            }
                            break;
                    }
                }
                break;
            case 3:
                if (event.type == SDL_QUIT) {
                    end_program = 1;
                    is_pause = 0;
                }
                if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            if (key_conflict == 1)
                                break;
                            keynum = 1;
                            which_page = 1;
                            p1_keyset = 1;
                            p2_keyset = 1;
                            break;
                        case SDLK_UP:
                            switch (keynum) {
                                case 1:
                                    p1_keyset = p1_keyset == 3 ? 3 : p1_keyset + 1;
                                    break;
                                case 2:
                                    p2_keyset = p2_keyset == 3 ? 3 : p2_keyset + 1;
                                    break;
                            }
                            break;
                        case SDLK_DOWN:
                            switch (keynum) {
                                case 1:
                                    p1_keyset = p1_keyset == 1 ? 1 : p1_keyset - 1;
                                    break;
                                case 2:
                                    p2_keyset = p2_keyset == 1 ? 1 : p2_keyset - 1;
                                    break;
                            }
                            break;
                        case SDLK_RIGHT:
                            keynum = keynum != 3 ? keynum + 1 : 3;
                            break;
                        case SDLK_LEFT:
                            keynum = keynum != 1 ? keynum - 1 : 1;
                            break;
                        case SDLK_RETURN:
                            if (keynum == 3) {
                                set_keysets(tank_1, tank_2);
                            }
                            break;
                    }
                }
                break;
            case 4:
                if (event.type == SDL_QUIT) {
                    end_program = 1;
                    is_pause = 0;
                }
                if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            if (key_conflict == 1)
                                break;
                            which_page = 1;
                            keynum = 1;
                            break;
                        case SDLK_a:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = 'a';
                                    break;
                                case 2:
                                    tank_1->down_key = 'a';
                                    break;
                                case 3:
                                    tank_1->right_key = 'a';
                                    break;
                                case 4:
                                    tank_1->left_key = 'a';
                                    break;
                                case 5:
                                    tank_1->fire_key = 'a';
                                    break;
                                case 6:
                                    tank_2->up_key = 'a';
                                    break;
                                case 7:
                                    tank_2->down_key = 'a';
                                    break;
                                case 8:
                                    tank_2->right_key = 'a';
                                    break;
                                case 9:
                                    tank_2->left_key = 'a';
                                    break;
                                case 10:
                                    tank_2->fire_key = 'a';
                                    break;
                            }
                            break;
                        case SDLK_b:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = 'b';
                                    break;
                                case 2:
                                    tank_1->down_key = 'b';
                                    break;
                                case 3:
                                    tank_1->right_key = 'b';
                                    break;
                                case 4:
                                    tank_1->left_key = 'b';
                                    break;
                                case 5:
                                    tank_1->fire_key = 'b';
                                    break;
                                case 6:
                                    tank_2->up_key = 'b';
                                    break;
                                case 7:
                                    tank_2->down_key = 'b';
                                    break;
                                case 8:
                                    tank_2->right_key = 'b';
                                    break;
                                case 9:
                                    tank_2->left_key = 'b';
                                    break;
                                case 10:
                                    tank_2->fire_key = 'b';
                                    break;
                            }
                            break;
                        case SDLK_c:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = 'c';
                                    break;
                                case 2:
                                    tank_1->down_key = 'c';
                                    break;
                                case 3:
                                    tank_1->right_key = 'c';
                                    break;
                                case 4:
                                    tank_1->left_key = 'c';
                                    break;
                                case 5:
                                    tank_1->fire_key = 'c';
                                    break;
                                case 6:
                                    tank_2->up_key = 'c';
                                    break;
                                case 7:
                                    tank_2->down_key = 'c';
                                    break;
                                case 8:
                                    tank_2->right_key = 'c';
                                    break;
                                case 9:
                                    tank_2->left_key = 'c';
                                    break;
                                case 10:
                                    tank_2->fire_key = 'c';
                                    break;
                            }
                            break;
                        case SDLK_d:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = 'd';
                                    break;
                                case 2:
                                    tank_1->down_key = 'd';
                                    break;
                                case 3:
                                    tank_1->right_key = 'd';
                                    break;
                                case 4:
                                    tank_1->left_key = 'd';
                                    break;
                                case 5:
                                    tank_1->fire_key = 'd';
                                    break;
                                case 6:
                                    tank_2->up_key = 'd';
                                    break;
                                case 7:
                                    tank_2->down_key = 'd';
                                    break;
                                case 8:
                                    tank_2->right_key = 'd';
                                    break;
                                case 9:
                                    tank_2->left_key = 'd';
                                    break;
                                case 10:
                                    tank_2->fire_key = 'd';
                                    break;
                            }
                            break;
                        case SDLK_e:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = 'e';
                                    break;
                                case 2:
                                    tank_1->down_key = 'e';
                                    break;
                                case 3:
                                    tank_1->right_key = 'e';
                                    break;
                                case 4:
                                    tank_1->left_key = 'e';
                                    break;
                                case 5:
                                    tank_1->fire_key = 'e';
                                    break;
                                case 6:
                                    tank_2->up_key = 'e';
                                    break;
                                case 7:
                                    tank_2->down_key = 'e';
                                    break;
                                case 8:
                                    tank_2->right_key = 'e';
                                    break;
                                case 9:
                                    tank_2->left_key = 'e';
                                    break;
                                case 10:
                                    tank_2->fire_key = 'e';
                                    break;
                            }
                            break;
                        case SDLK_f:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = 'f';
                                    break;
                                case 2:
                                    tank_1->down_key = 'f';
                                    break;
                                case 3:
                                    tank_1->right_key = 'f';
                                    break;
                                case 4:
                                    tank_1->left_key = 'f';
                                    break;
                                case 5:
                                    tank_1->fire_key = 'f';
                                    break;
                                case 6:
                                    tank_2->up_key = 'f';
                                    break;
                                case 7:
                                    tank_2->down_key = 'f';
                                    break;
                                case 8:
                                    tank_2->right_key = 'f';
                                    break;
                                case 9:
                                    tank_2->left_key = 'f';
                                    break;
                                case 10:
                                    tank_2->fire_key = 'f';
                                    break;
                            }
                            break;
                        case SDLK_g:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = 'g';
                                    break;
                                case 2:
                                    tank_1->down_key = 'g';
                                    break;
                                case 3:
                                    tank_1->right_key = 'g';
                                    break;
                                case 4:
                                    tank_1->left_key = 'g';
                                    break;
                                case 5:
                                    tank_1->fire_key = 'g';
                                    break;
                                case 6:
                                    tank_2->up_key = 'g';
                                    break;
                                case 7:
                                    tank_2->down_key = 'g';
                                    break;
                                case 8:
                                    tank_2->right_key = 'g';
                                    break;
                                case 9:
                                    tank_2->left_key = 'g';
                                    break;
                                case 10:
                                    tank_2->fire_key = 'g';
                                    break;
                            }
                            break;
                        case SDLK_h:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = 'h';
                                    break;
                                case 2:
                                    tank_1->down_key = 'h';
                                    break;
                                case 3:
                                    tank_1->right_key = 'h';
                                    break;
                                case 4:
                                    tank_1->left_key = 'h';
                                    break;
                                case 5:
                                    tank_1->fire_key = 'h';
                                    break;
                                case 6:
                                    tank_2->up_key = 'h';
                                    break;
                                case 7:
                                    tank_2->down_key = 'h';
                                    break;
                                case 8:
                                    tank_2->right_key = 'h';
                                    break;
                                case 9:
                                    tank_2->left_key = 'h';
                                    break;
                                case 10:
                                    tank_2->fire_key = 'h';
                                    break;
                            }
                            break;
                        case SDLK_i:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = 'i';
                                    break;
                                case 2:
                                    tank_1->down_key = 'i';
                                    break;
                                case 3:
                                    tank_1->right_key = 'i';
                                    break;
                                case 4:
                                    tank_1->left_key = 'i';
                                    break;
                                case 5:
                                    tank_1->fire_key = 'i';
                                    break;
                                case 6:
                                    tank_2->up_key = 'i';
                                    break;
                                case 7:
                                    tank_2->down_key = 'i';
                                    break;
                                case 8:
                                    tank_2->right_key = 'i';
                                    break;
                                case 9:
                                    tank_2->left_key = 'i';
                                    break;
                                case 10:
                                    tank_2->fire_key = 'i';
                                    break;
                            }
                            break;
                        case SDLK_j:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = 'j';
                                    break;
                                case 2:
                                    tank_1->down_key = 'j';
                                    break;
                                case 3:
                                    tank_1->right_key = 'j';
                                    break;
                                case 4:
                                    tank_1->left_key = 'j';
                                    break;
                                case 5:
                                    tank_1->fire_key = 'j';
                                    break;
                                case 6:
                                    tank_2->up_key = 'j';
                                    break;
                                case 7:
                                    tank_2->down_key = 'j';
                                    break;
                                case 8:
                                    tank_2->right_key = 'j';
                                    break;
                                case 9:
                                    tank_2->left_key = 'j';
                                    break;
                                case 10:
                                    tank_2->fire_key = 'j';
                                    break;
                            }
                            break;
                        case SDLK_k:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = 'k';
                                    break;
                                case 2:
                                    tank_1->down_key = 'k';
                                    break;
                                case 3:
                                    tank_1->right_key = 'k';
                                    break;
                                case 4:
                                    tank_1->left_key = 'k';
                                    break;
                                case 5:
                                    tank_1->fire_key = 'k';
                                    break;
                                case 6:
                                    tank_2->up_key = 'k';
                                    break;
                                case 7:
                                    tank_2->down_key = 'k';
                                    break;
                                case 8:
                                    tank_2->right_key = 'k';
                                    break;
                                case 9:
                                    tank_2->left_key = 'k';
                                    break;
                                case 10:
                                    tank_2->fire_key = 'k';
                                    break;
                            }
                            break;
                        case SDLK_l:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = 'l';
                                    break;
                                case 2:
                                    tank_1->down_key = 'l';
                                    break;
                                case 3:
                                    tank_1->right_key = 'l';
                                    break;
                                case 4:
                                    tank_1->left_key = 'l';
                                    break;
                                case 5:
                                    tank_1->fire_key = 'l';
                                    break;
                                case 6:
                                    tank_2->up_key = 'l';
                                    break;
                                case 7:
                                    tank_2->down_key = 'l';
                                    break;
                                case 8:
                                    tank_2->right_key = 'l';
                                    break;
                                case 9:
                                    tank_2->left_key = 'l';
                                    break;
                                case 10:
                                    tank_2->fire_key = 'l';
                                    break;
                            }
                            break;
                        case SDLK_m:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = 'm';
                                    break;
                                case 2:
                                    tank_1->down_key = 'm';
                                    break;
                                case 3:
                                    tank_1->right_key = 'm';
                                    break;
                                case 4:
                                    tank_1->left_key = 'm';
                                    break;
                                case 5:
                                    tank_1->fire_key = 'm';
                                    break;
                                case 6:
                                    tank_2->up_key = 'm';
                                    break;
                                case 7:
                                    tank_2->down_key = 'm';
                                    break;
                                case 8:
                                    tank_2->right_key = 'm';
                                    break;
                                case 9:
                                    tank_2->left_key = 'm';
                                    break;
                                case 10:
                                    tank_2->fire_key = 'm';
                                    break;
                            }
                            break;
                        case SDLK_n:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = 'n';
                                    break;
                                case 2:
                                    tank_1->down_key = 'n';
                                    break;
                                case 3:
                                    tank_1->right_key = 'n';
                                    break;
                                case 4:
                                    tank_1->left_key = 'n';
                                    break;
                                case 5:
                                    tank_1->fire_key = 'n';
                                    break;
                                case 6:
                                    tank_2->up_key = 'n';
                                    break;
                                case 7:
                                    tank_2->down_key = 'n';
                                    break;
                                case 8:
                                    tank_2->right_key = 'n';
                                    break;
                                case 9:
                                    tank_2->left_key = 'n';
                                    break;
                                case 10:
                                    tank_2->fire_key = 'n';
                                    break;
                            }
                            break;
                        case SDLK_o:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = 'o';
                                    break;
                                case 2:
                                    tank_1->down_key = 'o';
                                    break;
                                case 3:
                                    tank_1->right_key = 'o';
                                    break;
                                case 4:
                                    tank_1->left_key = 'o';
                                    break;
                                case 5:
                                    tank_1->fire_key = 'o';
                                    break;
                                case 6:
                                    tank_2->up_key = 'o';
                                    break;
                                case 7:
                                    tank_2->down_key = 'o';
                                    break;
                                case 8:
                                    tank_2->right_key = 'o';
                                    break;
                                case 9:
                                    tank_2->left_key = 'o';
                                    break;
                                case 10:
                                    tank_2->fire_key = 'o';
                                    break;
                            }
                            break;
                        case SDLK_p:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = 'p';
                                    break;
                                case 2:
                                    tank_1->down_key = 'p';
                                    break;
                                case 3:
                                    tank_1->right_key = 'p';
                                    break;
                                case 4:
                                    tank_1->left_key = 'p';
                                    break;
                                case 5:
                                    tank_1->fire_key = 'p';
                                    break;
                                case 6:
                                    tank_2->up_key = 'p';
                                    break;
                                case 7:
                                    tank_2->down_key = 'p';
                                    break;
                                case 8:
                                    tank_2->right_key = 'p';
                                    break;
                                case 9:
                                    tank_2->left_key = 'p';
                                    break;
                                case 10:
                                    tank_2->fire_key = 'p';
                                    break;
                            }
                            break;
                        case SDLK_q:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = 'q';
                                    break;
                                case 2:
                                    tank_1->down_key = 'q';
                                    break;
                                case 3:
                                    tank_1->right_key = 'q';
                                    break;
                                case 4:
                                    tank_1->left_key = 'q';
                                    break;
                                case 5:
                                    tank_1->fire_key = 'q';
                                    break;
                                case 6:
                                    tank_2->up_key = 'q';
                                    break;
                                case 7:
                                    tank_2->down_key = 'q';
                                    break;
                                case 8:
                                    tank_2->right_key = 'q';
                                    break;
                                case 9:
                                    tank_2->left_key = 'q';
                                    break;
                                case 10:
                                    tank_2->fire_key = 'q';
                                    break;
                            }
                            break;
                        case SDLK_r:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = 'r';
                                    break;
                                case 2:
                                    tank_1->down_key = 'r';
                                    break;
                                case 3:
                                    tank_1->right_key = 'r';
                                    break;
                                case 4:
                                    tank_1->left_key = 'r';
                                    break;
                                case 5:
                                    tank_1->fire_key = 'r';
                                    break;
                                case 6:
                                    tank_2->up_key = 'r';
                                    break;
                                case 7:
                                    tank_2->down_key = 'r';
                                    break;
                                case 8:
                                    tank_2->right_key = 'r';
                                    break;
                                case 9:
                                    tank_2->left_key = 'r';
                                    break;
                                case 10:
                                    tank_2->fire_key = 'r';
                                    break;
                            }
                            break;
                        case SDLK_s:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = 's';
                                    break;
                                case 2:
                                    tank_1->down_key = 's';
                                    break;
                                case 3:
                                    tank_1->right_key = 's';
                                    break;
                                case 4:
                                    tank_1->left_key = 's';
                                    break;
                                case 5:
                                    tank_1->fire_key = 's';
                                    break;
                                case 6:
                                    tank_2->up_key = 's';
                                    break;
                                case 7:
                                    tank_2->down_key = 's';
                                    break;
                                case 8:
                                    tank_2->right_key = 's';
                                    break;
                                case 9:
                                    tank_2->left_key = 's';
                                    break;
                                case 10:
                                    tank_2->fire_key = 's';
                                    break;
                            }
                            break;
                        case SDLK_t:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = 't';
                                    break;
                                case 2:
                                    tank_1->down_key = 't';
                                    break;
                                case 3:
                                    tank_1->right_key = 't';
                                    break;
                                case 4:
                                    tank_1->left_key = 't';
                                    break;
                                case 5:
                                    tank_1->fire_key = 't';
                                    break;
                                case 6:
                                    tank_2->up_key = 't';
                                    break;
                                case 7:
                                    tank_2->down_key = 't';
                                    break;
                                case 8:
                                    tank_2->right_key = 't';
                                    break;
                                case 9:
                                    tank_2->left_key = 't';
                                    break;
                                case 10:
                                    tank_2->fire_key = 't';
                                    break;
                            }
                            break;
                        case SDLK_u:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = 'u';
                                    break;
                                case 2:
                                    tank_1->down_key = 'u';
                                    break;
                                case 3:
                                    tank_1->right_key = 'u';
                                    break;
                                case 4:
                                    tank_1->left_key = 'u';
                                    break;
                                case 5:
                                    tank_1->fire_key = 'u';
                                    break;
                                case 6:
                                    tank_2->up_key = 'u';
                                    break;
                                case 7:
                                    tank_2->down_key = 'u';
                                    break;
                                case 8:
                                    tank_2->right_key = 'u';
                                    break;
                                case 9:
                                    tank_2->left_key = 'u';
                                    break;
                                case 10:
                                    tank_2->fire_key = 'u';
                                    break;
                            }
                            break;
                        case SDLK_v:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = 'v';
                                    break;
                                case 2:
                                    tank_1->down_key = 'v';
                                    break;
                                case 3:
                                    tank_1->right_key = 'v';
                                    break;
                                case 4:
                                    tank_1->left_key = 'v';
                                    break;
                                case 5:
                                    tank_1->fire_key = 'v';
                                    break;
                                case 6:
                                    tank_2->up_key = 'v';
                                    break;
                                case 7:
                                    tank_2->down_key = 'v';
                                    break;
                                case 8:
                                    tank_2->right_key = 'v';
                                    break;
                                case 9:
                                    tank_2->left_key = 'v';
                                    break;
                                case 10:
                                    tank_2->fire_key = 'v';
                                    break;
                            }
                            break;
                        case SDLK_w:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = 'w';
                                    break;
                                case 2:
                                    tank_1->down_key = 'w';
                                    break;
                                case 3:
                                    tank_1->right_key = 'w';
                                    break;
                                case 4:
                                    tank_1->left_key = 'w';
                                    break;
                                case 5:
                                    tank_1->fire_key = 'w';
                                    break;
                                case 6:
                                    tank_2->up_key = 'w';
                                    break;
                                case 7:
                                    tank_2->down_key = 'w';
                                    break;
                                case 8:
                                    tank_2->right_key = 'w';
                                    break;
                                case 9:
                                    tank_2->left_key = 'w';
                                    break;
                                case 10:
                                    tank_2->fire_key = 'w';
                                    break;
                            }
                            break;
                        case SDLK_x:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = 'x';
                                    break;
                                case 2:
                                    tank_1->down_key = 'x';
                                    break;
                                case 3:
                                    tank_1->right_key = 'x';
                                    break;
                                case 4:
                                    tank_1->left_key = 'x';
                                    break;
                                case 5:
                                    tank_1->fire_key = 'x';
                                    break;
                                case 6:
                                    tank_2->up_key = 'x';
                                    break;
                                case 7:
                                    tank_2->down_key = 'x';
                                    break;
                                case 8:
                                    tank_2->right_key = 'x';
                                    break;
                                case 9:
                                    tank_2->left_key = 'x';
                                    break;
                                case 10:
                                    tank_2->fire_key = 'x';
                                    break;
                            }
                            break;
                        case SDLK_y:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = 'y';
                                    break;
                                case 2:
                                    tank_1->down_key = 'y';
                                    break;
                                case 3:
                                    tank_1->right_key = 'y';
                                    break;
                                case 4:
                                    tank_1->left_key = 'y';
                                    break;
                                case 5:
                                    tank_1->fire_key = 'y';
                                    break;
                                case 6:
                                    tank_2->up_key = 'y';
                                    break;
                                case 7:
                                    tank_2->down_key = 'y';
                                    break;
                                case 8:
                                    tank_2->right_key = 'y';
                                    break;
                                case 9:
                                    tank_2->left_key = 'y';
                                    break;
                                case 10:
                                    tank_2->fire_key = 'y';
                                    break;
                            }
                            break;
                        case SDLK_z:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = 'z';
                                    break;
                                case 2:
                                    tank_1->down_key = 'z';
                                    break;
                                case 3:
                                    tank_1->right_key = 'z';
                                    break;
                                case 4:
                                    tank_1->left_key = 'z';
                                    break;
                                case 5:
                                    tank_1->fire_key = 'z';
                                    break;
                                case 6:
                                    tank_2->up_key = 'z';
                                    break;
                                case 7:
                                    tank_2->down_key = 'z';
                                    break;
                                case 8:
                                    tank_2->right_key = 'z';
                                    break;
                                case 9:
                                    tank_2->left_key = 'z';
                                    break;
                                case 10:
                                    tank_2->fire_key = 'z';
                                    break;
                            }
                            break;
                        case SDLK_1:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = '1';
                                    break;
                                case 2:
                                    tank_1->down_key = '1';
                                    break;
                                case 3:
                                    tank_1->right_key = '1';
                                    break;
                                case 4:
                                    tank_1->left_key = '1';
                                    break;
                                case 5:
                                    tank_1->fire_key = '1';
                                    break;
                                case 6:
                                    tank_2->up_key = '1';
                                    break;
                                case 7:
                                    tank_2->down_key = '1';
                                    break;
                                case 8:
                                    tank_2->right_key = '1';
                                    break;
                                case 9:
                                    tank_2->left_key = '1';
                                    break;
                                case 10:
                                    tank_2->fire_key = '1';
                                    break;
                            }
                            break;
                        case SDLK_2:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = '2';
                                    break;
                                case 2:
                                    tank_1->down_key = '2';
                                    break;
                                case 3:
                                    tank_1->right_key = '2';
                                    break;
                                case 4:
                                    tank_1->left_key = '2';
                                    break;
                                case 5:
                                    tank_1->fire_key = '2';
                                    break;
                                case 6:
                                    tank_2->up_key = '2';
                                    break;
                                case 7:
                                    tank_2->down_key = '2';
                                    break;
                                case 8:
                                    tank_2->right_key = '2';
                                    break;
                                case 9:
                                    tank_2->left_key = '2';
                                    break;
                                case 10:
                                    tank_2->fire_key = '2';
                                    break;
                            }
                            break;
                        case SDLK_3:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = '3';
                                    break;
                                case 2:
                                    tank_1->down_key = '3';
                                    break;
                                case 3:
                                    tank_1->right_key = '3';
                                    break;
                                case 4:
                                    tank_1->left_key = '3';
                                    break;
                                case 5:
                                    tank_1->fire_key = '3';
                                    break;
                                case 6:
                                    tank_2->up_key = '3';
                                    break;
                                case 7:
                                    tank_2->down_key = '3';
                                    break;
                                case 8:
                                    tank_2->right_key = '3';
                                    break;
                                case 9:
                                    tank_2->left_key = '3';
                                    break;
                                case 10:
                                    tank_2->fire_key = '3';
                                    break;
                            }
                            break;
                        case SDLK_4:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = '4';
                                    break;
                                case 2:
                                    tank_1->down_key = '4';
                                    break;
                                case 3:
                                    tank_1->right_key = '4';
                                    break;
                                case 4:
                                    tank_1->left_key = '4';
                                    break;
                                case 5:
                                    tank_1->fire_key = '4';
                                    break;
                                case 6:
                                    tank_2->up_key = '4';
                                    break;
                                case 7:
                                    tank_2->down_key = '4';
                                    break;
                                case 8:
                                    tank_2->right_key = '4';
                                    break;
                                case 9:
                                    tank_2->left_key = '4';
                                    break;
                                case 10:
                                    tank_2->fire_key = '4';
                                    break;
                            }
                            break;
                        case SDLK_5:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = '5';
                                    break;
                                case 2:
                                    tank_1->down_key = '5';
                                    break;
                                case 3:
                                    tank_1->right_key = '5';
                                    break;
                                case 4:
                                    tank_1->left_key = '5';
                                    break;
                                case 5:
                                    tank_1->fire_key = '5';
                                    break;
                                case 6:
                                    tank_2->up_key = '5';
                                    break;
                                case 7:
                                    tank_2->down_key = '5';
                                    break;
                                case 8:
                                    tank_2->right_key = '5';
                                    break;
                                case 9:
                                    tank_2->left_key = '5';
                                    break;
                                case 10:
                                    tank_2->fire_key = '5';
                                    break;
                            }
                            break;
                        case SDLK_6:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = '6';
                                    break;
                                case 2:
                                    tank_1->down_key = '6';
                                    break;
                                case 3:
                                    tank_1->right_key = '6';
                                    break;
                                case 4:
                                    tank_1->left_key = '6';
                                    break;
                                case 5:
                                    tank_1->fire_key = '6';
                                    break;
                                case 6:
                                    tank_2->up_key = '6';
                                    break;
                                case 7:
                                    tank_2->down_key = '6';
                                    break;
                                case 8:
                                    tank_2->right_key = '6';
                                    break;
                                case 9:
                                    tank_2->left_key = '6';
                                    break;
                                case 10:
                                    tank_2->fire_key = '6';
                                    break;
                            }
                            break;
                        case SDLK_7:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = '7';
                                    break;
                                case 2:
                                    tank_1->down_key = '7';
                                    break;
                                case 3:
                                    tank_1->right_key = '7';
                                    break;
                                case 4:
                                    tank_1->left_key = '7';
                                    break;
                                case 5:
                                    tank_1->fire_key = '7';
                                    break;
                                case 6:
                                    tank_2->up_key = '7';
                                    break;
                                case 7:
                                    tank_2->down_key = '7';
                                    break;
                                case 8:
                                    tank_2->right_key = '7';
                                    break;
                                case 9:
                                    tank_2->left_key = '7';
                                    break;
                                case 10:
                                    tank_2->fire_key = '7';
                                    break;
                            }
                            break;
                        case SDLK_8:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = '8';
                                    break;
                                case 2:
                                    tank_1->down_key = '8';
                                    break;
                                case 3:
                                    tank_1->right_key = '8';
                                    break;
                                case 4:
                                    tank_1->left_key = '8';
                                    break;
                                case 5:
                                    tank_1->fire_key = '8';
                                    break;
                                case 6:
                                    tank_2->up_key = '8';
                                    break;
                                case 7:
                                    tank_2->down_key = '8';
                                    break;
                                case 8:
                                    tank_2->right_key = '8';
                                    break;
                                case 9:
                                    tank_2->left_key = '8';
                                    break;
                                case 10:
                                    tank_2->fire_key = '8';
                                    break;
                            }
                            break;
                        case SDLK_9:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = '9';
                                    break;
                                case 2:
                                    tank_1->down_key = '9';
                                    break;
                                case 3:
                                    tank_1->right_key = '9';
                                    break;
                                case 4:
                                    tank_1->left_key = '9';
                                    break;
                                case 5:
                                    tank_1->fire_key = '9';
                                    break;
                                case 6:
                                    tank_2->up_key = '9';
                                    break;
                                case 7:
                                    tank_2->down_key = '9';
                                    break;
                                case 8:
                                    tank_2->right_key = '9';
                                    break;
                                case 9:
                                    tank_2->left_key = '9';
                                    break;
                                case 10:
                                    tank_2->fire_key = '9';
                                    break;
                            }
                            break;
                        case SDLK_0:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = '0';
                                    break;
                                case 2:
                                    tank_1->down_key = '0';
                                    break;
                                case 3:
                                    tank_1->right_key = '0';
                                    break;
                                case 4:
                                    tank_1->left_key = '0';
                                    break;
                                case 5:
                                    tank_1->fire_key = '0';
                                    break;
                                case 6:
                                    tank_2->up_key = '0';
                                    break;
                                case 7:
                                    tank_2->down_key = '0';
                                    break;
                                case 8:
                                    tank_2->right_key = '0';
                                    break;
                                case 9:
                                    tank_2->left_key = '0';
                                    break;
                                case 10:
                                    tank_2->fire_key = '0';
                                    break;
                            }
                            break;
                        case SDLK_SPACE:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = ' ';
                                    break;
                                case 2:
                                    tank_1->down_key = ' ';
                                    break;
                                case 3:
                                    tank_1->right_key = ' ';
                                    break;
                                case 4:
                                    tank_1->left_key = ' ';
                                    break;
                                case 5:
                                    tank_1->fire_key = ' ';
                                    break;
                                case 6:
                                    tank_2->up_key = ' ';
                                    break;
                                case 7:
                                    tank_2->down_key = ' ';
                                    break;
                                case 8:
                                    tank_2->right_key = ' ';
                                    break;
                                case 9:
                                    tank_2->left_key = ' ';
                                    break;
                                case 10:
                                    tank_2->fire_key = ' ';
                                    break;
                            }
                            break;
                        case SDLK_UP:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = '^';
                                    break;
                                case 2:
                                    tank_1->down_key = '^';
                                    break;
                                case 3:
                                    tank_1->right_key = '^';
                                    break;
                                case 4:
                                    tank_1->left_key = '^';
                                    break;
                                case 5:
                                    tank_1->fire_key = '^';
                                    break;
                                case 6:
                                    tank_2->up_key = '^';
                                    break;
                                case 7:
                                    tank_2->down_key = '^';
                                    break;
                                case 8:
                                    tank_2->right_key = '^';
                                    break;
                                case 9:
                                    tank_2->left_key = '^';
                                    break;
                                case 10:
                                    tank_2->fire_key = '^';
                                    break;
                            }
                            break;
                        case SDLK_DOWN:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = '|';
                                    break;
                                case 2:
                                    tank_1->down_key = '|';
                                    break;
                                case 3:
                                    tank_1->right_key = '|';
                                    break;
                                case 4:
                                    tank_1->left_key = '|';
                                    break;
                                case 5:
                                    tank_1->fire_key = '|';
                                    break;
                                case 6:
                                    tank_2->up_key = '|';
                                    break;
                                case 7:
                                    tank_2->down_key = '|';
                                    break;
                                case 8:
                                    tank_2->right_key = '|';
                                    break;
                                case 9:
                                    tank_2->left_key = '|';
                                    break;
                                case 10:
                                    tank_2->fire_key = '|';
                                    break;
                            }
                            break;
                        case SDLK_RIGHT:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = '>';
                                    break;
                                case 2:
                                    tank_1->down_key = '>';
                                    break;
                                case 3:
                                    tank_1->right_key = '>';
                                    break;
                                case 4:
                                    tank_1->left_key = '>';
                                    break;
                                case 5:
                                    tank_1->fire_key = '>';
                                    break;
                                case 6:
                                    tank_2->up_key = '>';
                                    break;
                                case 7:
                                    tank_2->down_key = '>';
                                    break;
                                case 8:
                                    tank_2->right_key = '>';
                                    break;
                                case 9:
                                    tank_2->left_key = '>';
                                    break;
                                case 10:
                                    tank_2->fire_key = '>';
                                    break;
                            }
                            break;
                        case SDLK_LEFT:
                            switch (keynum) {
                                case 1:
                                    tank_1->up_key = '<';
                                    break;
                                case 2:
                                    tank_1->down_key = '<';
                                    break;
                                case 3:
                                    tank_1->right_key = '<';
                                    break;
                                case 4:
                                    tank_1->left_key = '<';
                                    break;
                                case 5:
                                    tank_1->fire_key = '<';
                                    break;
                                case 6:
                                    tank_2->up_key = '<';
                                    break;
                                case 7:
                                    tank_2->down_key = '<';
                                    break;
                                case 8:
                                    tank_2->right_key = '<';
                                    break;
                                case 9:
                                    tank_2->left_key = '<';
                                    break;
                                case 10:
                                    tank_2->fire_key = '<';
                                    break;
                            }
                            break;
                        case SDLK_TAB:
                            keynum = keynum == 10 ? 1 : keynum + 1;
                            break;
                    }
                }
        }
    }
}

void set_keysets(Tank *tank_1, Tank *tank_2) {
    switch (p1_keyset) {
        case 1:
            tank_1->up_key = '^';
            tank_1->down_key = '|';
            tank_1->right_key = '>';
            tank_1->left_key = '<';
            tank_1->fire_key = 'p';
            break;
        case 2:
            tank_1->up_key = 'h';
            tank_1->down_key = 'n';
            tank_1->right_key = 'm';
            tank_1->left_key = 'b';
            tank_1->fire_key = 'g';
            break;
        case 3:
            tank_1->up_key = 'w';
            tank_1->down_key = 's';
            tank_1->right_key = 'd';
            tank_1->left_key = 'a';
            tank_1->fire_key = 'q';
            break;
    }
    switch (p2_keyset) {
        case 1:
            tank_2->up_key = '^';
            tank_2->down_key = '|';
            tank_2->right_key = '>';
            tank_2->left_key = '<';
            tank_2->fire_key = 'p';
            break;
        case 2:
            tank_2->up_key = 'h';
            tank_2->down_key = 'n';
            tank_2->right_key = 'm';
            tank_2->left_key = 'b';
            tank_2->fire_key = 'g';
            break;
        case 3:
            tank_2->up_key = 'w';
            tank_2->down_key = 's';
            tank_2->right_key = 'd';
            tank_2->left_key = 'a';
            tank_2->fire_key = 'q';
            break;
    }
}