// monster.h
#ifndef MONSTER_H
#define MONSTER_H

#include "object.h"
#include "player.h"
#include <chrono>
#include <cmath> 

class player;

class monster : public object {
protected:
    int hp, armor, damage, speed;
    int frames_of_walk;
    bool fly;
public:
    monster();
    bool is_flying();
    int get_type();
    int get_hp();
    void set_hp(int delta_hp);
    virtual void behavior(player *p);
};

class soyjak_typical : public monster {
protected:
    int xp, yp;
    int walking_radius;
    int attack_radius;
    bool right_direction, x_direction;
    bool damaged_player;
    int attack_period;
    int wait_time;

public:
    bool is_attacking;
    int random_start;
    std::chrono::time_point<std::chrono::system_clock> last_hit;
    soyjak_typical(int xp, int yp, int rad, bool x_dir);
    void behavior(player *p);
};

class bomb : public monster {
protected:
    int distance;
    int limit;
    int attack_radius;
    int frames_after_atack;
    bool is_attacking;
public:
    bomb();
    void start_attack(int x0, int y0);
    void behavior(player *p);
};

class flight : public monster {
protected:
    int xp, yp;
    int fly_radius;
    bool up_direction;
    int period_bomb;
    int speed;
    bomb *b_attack;
    int frames_of_walk;
public:
    std::chrono::time_point<std::chrono::system_clock> last_hit;
    flight(int xp, int yp, int
rad, bomb* b);
    void behavior(player *p);
};

class ball : public monster {
protected:
    int xl, xr, yl, yr;
    int xspeed, yspeed;
    bool damaged_player;
    int frames_of_walk;
public:
    ball(int xl, int yl, int xr, int yr);
    void behavior(player* pla);
};

#endif