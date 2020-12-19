//
// Created by sepehr on 12/19/18.
//

#ifndef ALTER_TANK_STRUCTS_H
#define ALTER_TANK_STRUCTS_H

typedef struct {
    double x;
    double y;
    int type;
    int is_full;
} Powerup;

typedef struct {
    int countdown;
    int p1pt;
    int p2pt;
    Powerup *powerup;
} Info;

typedef struct {
    double x;
    double y;
    double angle;
    int is_active;
    double life_time;
} Bullet;

typedef struct {
    double x;
    double y;
    double angle;
    int is_active;
    int life_time;
} Frag;

typedef struct {
    double x;
    double y;
    double angle;
    int is_active;
    int life_time;
    Frag *frags;
} Fragbomb;

typedef struct {
    double x;
    double y;
    int is_set;
    int trigger_time;
    Frag *frags;
} Mine;

typedef struct {
    double x;
    double y;
    double angle;
    int move_status;
    int turn_status;
    Bullet *bullet;
    int is_dead;
    int laser;
    int mine;
    int fbomb;
    char up_key;
    char down_key;
    char right_key;
    char left_key;
    char fire_key;
    Fragbomb *fragbomb;
} Tank;

typedef struct {
    double x1;
    double y1;
    double x2;
    double y2;
    int does_exist;
} Wall;

extern int bullet_limit;
extern int bullet_life_time;
extern int count_rows;
extern int count_columns;
extern int fps;
extern int maze_start_x;
extern int maze_start_y;
extern int cell_size;
extern int is_pause;
extern int wall_size;
extern int move_speed;
extern int tank_radius;
extern int passed_ticks;
extern int bullet_radius;
extern int window_width;
extern int window_height;
extern int end_program;
extern int which_page;
extern int win_point;
extern int game_start;
extern int powerup_limit;
extern int powerup_radius;
extern int laser_radius;
extern int error;
extern int key_conflict;
extern int p1_keyset;
extern int p2_keyset;
extern int mine_limit;
extern int frag_count;
extern int frag_speed;
extern int frag_lifetime;
extern int fragbomb_radius;
extern int frag_radius;
extern int mine_set_time;
extern int mine_explode_time;
extern int mine_radius;

#endif //ALTER_TANK_STRUCTS_H