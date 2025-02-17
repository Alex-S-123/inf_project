#include <iostream>

using namespace std;

class object{
protected:
    float x, y; //coords
    int stx, sty; //size of texture
    //Texture tex;
public:
    object();

    void set_pos(float x0, float y0);

    void set_tex_size(int x0, int y0);

    float get_x();

    float get_y();

    float get_tx();

    float get_ty();

    void set_texture();


};
object::object(){
    this->x = 0;
    this->y = 0;
    this->stx = 0;
    this->sty = 0;
}

void object::set_pos(float x0, float y0) {
    this->x = x0;
    this->y = y0;
}


class treasure: public object{
protected:
    int* inner;
public:
    treasure();
    bool give(int type, int number);
    bool take(int type, int number);
};

class wall: public object{
protected:
    bool visible;
};

class gate:public wall{
protected:
    bool is_active;
    int type;
    bool destr_key;
public:
    bool try_to_open(Player p);
};

class chest:public treasure{
protected:
    bool is_open;
    int type;
    bool destr_key;
public:
    bool try_to_open(Player p);
};

class teleport: gate{
protected:
    int go_to;
};

class sign: public object{
private:
    char* text;
public:
    char* read();
};

class seller: public sign{
protected:
    int* inner;
    int* price;
public:
    bool sell();
};

class monster: public object{
protected:
    int hp, armor, damage, speed;
public:
    damaged(int damag);
    atack(monster* m);
    behavior();
};

class player: public monster{
protected:
    int* inner;
    int money;
    int type;
public:
    action();
};

class soyjak_typical: public monster {
protected:
    float x0, y0; // monster walks around this point
    int walking_radius; // max radius from the point (x0; y0)
    int attack_radius; // if player is closer than this radius, monster attacks
    bool right_direction;
public:
    soyjak_typical(float x0, float y0) {
        this->hp = 6;
        this->armor = 3;
        this->damage = 2;
        this->speed = 3;
        this->x0 = x0;
        this->y0 = y0;
        this->walking_radius = 10;
        this->attack_radius = 5;
        this->right_direction = true;
    };

    ~soyjak_typical() {
        delete this;
    };

    void behavior() {
        // walking (now exists only on x-axis)
        if (this->right_direction) {
            if (this->x <= this->x0 + walking_radius) {
                this->x += this->speed;
            } else {
                this->right_direction = false;
            }
        } else {
            if (this->x >= this->x0 - walking_radius) {
                this->x -= this->speed;
            } else {
                this->right_direction = true;
            }
        }
        // attack (attacks only on x-axis)
        if (pow(pow(player.get_x() - this->x, 2) + pow(player.get_y() - this->y, 2), 1/2) {
            // change of image to attacking image
        /
    };
};
