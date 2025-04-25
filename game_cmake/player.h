// player.h
#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"
#include "monster.h"
#include <chrono>
#include <cmath> 

class monster;

class player : public object {
protected:
    int hp, armor, damage, speed, time_atack, dist_atack;
    int num_of_items, gold;
    bool attack, r_dir, is_walking;
    int frames_after_atack, frames_of_walk;
    int xsize, ysize;
    int max_health;
public:
    item** inner;
    std::chrono::time_point<std::chrono::system_clock> last_hit;
    std::chrono::time_point<std::chrono::system_clock> last_dist_hit;
    bool dead;

    player(int sx, int sy, int tx0, int ty0, int health);
    int get_hp();
    bool get_attack();
    int get_damage();
    bool can_atack_dist();
    void set_items(int n);
    bool give(item* it);
    item* take(int i);
    void damaged(int damag);
    void set_hp(int he);
    int get_gold();
    void action(int** level, int p, int q, monster** monsters_list, int n, object** objects_list, int no);
};

#endif