// object.cpp
#include "object.h"

object::object(int sx, int sy, int tx0, int ty0) {
    this->x = 0;
    this->y = 0;
    this->x0 = sx;
    this->y0 = sy;
    this->x1 = tx0;
    this->y1 = ty0;
    type = 0;
    inner = new item*[24];
    collision = true;
}

sf::IntRect object::tex_rect() {
    return sf::IntRect(x0, y0, x1-x0, y1-y0);
}

void object::set_pos(float x0, float y0) {
    this->x = x0;
    this->y = y0;
}

int object::get_centre_x() {
    return x + abs(this->y1 - this->y0) / 2;
}

int object::get_centre_y() {
    return y + abs(this->x1 - this->x0) / 2;
}

int object::get_x() { return this->x; }
int object::get_y() { return this->y; }
int object::get_type() { return type; }
bool object::get_collision() { return collision; }

bool object::give(item* it) { return 0; }
item* object::take(int i) { return NULL; }
int object::destination() { return 0; }


teleport::teleport(int x0, int y0, int dest):object(0, 0, 40, 60){
    x = x0;
    y = y0;
    type = 2;
    go_to = dest;
    collision = false;
}
int teleport::destination(){
    return go_to;
}

treasure::treasure(int x, int y):object(0, 0, 25, 25){
    type = 1;
    n = 0;
    set_pos(x, y);
    for(int i = 0; i < 24; i++){
        inner[i] = NULL;
    }
}

bool treasure::give(item* it){
    if(n == 24||it==NULL) return 0;
    inner[n] = it;
    n++;
    return 1;
}
item* treasure::take(int i){
    if(i > n) return NULL;
    item* res = inner[i];
    for(int j = i; j < n-1; j++) inner[j] = inner[j+1];
    inner[n-1] = NULL;
    n--;
    return res;
}