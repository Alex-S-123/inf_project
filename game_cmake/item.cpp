//item.cpp

#include "item.h"

item::item(int t, int damage, int armor, int hp, int speed, int atack_speed, int dist, int c, int nu) {
    boost_damage = damage;
    boost_armor = armor;
    boost_hp = hp;
    boost_speed = speed;
    boost_atack_speed = atack_speed;
    boost_dist = dist;
    type = t;
    cost = c;
    number = nu;
}

item::item() {
    boost_damage = 0;
    boost_armor = 0;
    boost_hp = 0;
    boost_speed = 0;
    boost_atack_speed = 0;
    boost_dist = 0;
    type = 0;
    cost = 0;
    this->x1 = 0;
    this->y1 = 0;
    this->x0 = 60;
    this->y0 = 60;
}