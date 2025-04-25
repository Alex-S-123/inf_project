// item.h
#ifndef ITEM_H
#define ITEM_H

class item {
protected:
    int x0, y0, x1, y1; //texture rect for menue
public:
    int boost_damage, boost_armor, boost_hp, boost_speed, boost_atack_speed, boost_dist, type, number, cost;

    item(int t, int damage, int armor, int hp, int speed, int atack_speed, int dist, int c, int nu);
    item();
};

#endif