#include <iostream>

using namespace std;

class object{
protected:
    float x, y; //coords
    int stx, sty; //size of texture
    //Sprite s; ?
public:
    object(/*Texture te*/){
        this->x = 0;
        this->y = 0;
        this->stx = 0;
        this->sty = 0;
        //this->st.setTexture(te);?
    }

    void set_pos(float x0, float y0){
        this->x = x0;
        this->y = y0;
    }

    void set_tex_size(int x0, int y0){
        this->stx = x0;
        this->sty = y0;
    }

    float get_x() {return this->x;}

    float get_y() {return this->y;}

    int get_tx() {return this->stx;}

    int get_ty() {return this->sty;}

    void set_texture(int x0, int y0){
        //this->st.setTextureRect();?
    }

    void draw(){
    //this->st.draw();?
    }


};

class item{
protected:
    int boost_damage, boost_armor, boost_hp, use_before_break, time_of_destruction, weight;
    bool is_active;
    int x0, y0, x1, y1; //texture rect for menue
    char* name;
public:

};

class monster: public object{
protected:
    int hp, armor, damage, speed;
public:
    void damaged(int damag);
    int atack();
    virtual void behavior();
};

class player: public monster{
protected:
    item* inner;
    int type;
    int weight;
public:
    action();
};


class treasure: public object{
protected:
    int n;
    item* inner;
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
    bool try_to_open(player p);
};

class chest:public treasure, public gate{
    bool give(int type, int number);
    bool take(int type, int number);
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
    }

    ~soyjak_typical() {
        delete this;
    }

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
        //if (pow(pow(player.get_x() - this->x, 2) + pow(player.get_y() - this->y, 2), 1/2)) {
            // change of image to attacking image
        //}
    }
};








