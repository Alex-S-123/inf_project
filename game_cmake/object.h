// object.h
#ifndef OBJECT_H
#define OBJECT_H

#include <SFML/Graphics.hpp>
#include "item.h"


class object {
protected:
    int x, y; //coords
    int x0, y0, x1, y1; //texture rect
    int type;
    bool collision;

public:
    item** inner;
    object(int sx, int sy, int tx0, int ty0);

    sf::IntRect tex_rect();
    void set_pos(float x0, float y0);
    int get_centre_x();
    int get_centre_y();
    int get_x();
    int get_y();
    int get_type();
    bool get_collision();

    virtual bool give(item* it);
    virtual item* take(int i);
    virtual int destination();
};

class teleport : public object {
    protected:
        int go_to;
    public:
        teleport(int x0, int y0, int dest);
        int destination();
};

class treasure : public object {
    protected:
        int n;
    public:
        treasure(int x, int y);
        bool give(item* it);
        item* take(int i);
};

#endif