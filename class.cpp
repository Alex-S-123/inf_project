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
    this->collasion = false;
}

void object::set_pos(float x0, float y0){
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






