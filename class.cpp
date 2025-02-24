#include <iostream>
#include <SFML/Graphics.hpp>
                                //uncomment to test
using namespace sf;

class object{
protected:
    int x, y; //coords
    int x0, y0, x1, y1; //texture rect 
public:
    object(int sx, int sy, int tx0, int ty0){
        this->x = 0;
        this->y = 0;
        this->x1 = sx;
        this->y1 = sy;
        this->x0 = tx0;
        this->y0 = ty0;
    }

    void set_pos(float x0, float y0){
        this->x = x0;
        this->y = y0;
    }


    int get_x() {return this->x;}

    int get_y() {return this->y;}


};

class item{
protected:
    int boost_damage, boost_armor, boost_hp, use_before_break, time_of_active;
    int x0, y0, x1, y1; //texture rect for menue
    char* name;
public:
    virtual void use_item() = 0;
};


class effect: public item{
    virtual void use(/*monster* m*/); //i dont know how to do it
};

class monster: public object{
protected:
    int hp, armor, damage, speed;
    int type;
    effect* effects;    //because we have this
    item* inner;
public:
    monster();
    void damaged(int damag);
    int atack();
    int get_type(){return this->type;}
    virtual void behavior() = 0;
};

class player: public object{
protected:
    int hp, armor, damage, speed;
    int type;
    effect* effects;    //because we have this
    item* inner;
public:
    player(int sx, int sy, int tx0, int ty0) : object(sx, sy, tx0, ty0){
    }
    void action(){
	if(Keyboard::isKeyPressed(Keyboard::W)) x -= 1;
	if(Keyboard::isKeyPressed(Keyboard::S)) x += 1;
	if(Keyboard::isKeyPressed(Keyboard::A)) y -= 1;
	if(Keyboard::isKeyPressed(Keyboard::D)) y += 1;
    }
};


class treasure: public object{
protected:
    int n;
    item* inner;
public:
    treasure();
    bool give(item it);
    bool take(int i);
};

class wall: public object{
protected:
    bool visible;
public:
    bool is_visible(){return this->visible;}
};

class gate:public wall{
protected:
    bool is_active;
    int type;
    bool destr_key;
public:
    bool try_to_open();
};

class chest:public treasure, public gate{
    bool take(int i);
};

class teleport:public gate{
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
    int xp, yp; // monster walks around this point
    int walking_radius; // max radius from the point (x0; y0)
    int attack_radius; // if player is closer than this radius, monster attacks
    bool right_direction;
    //need texture size!
public:
    soyjak_typical(float x0, float y0) {
        this->hp = 6;
        this->armor = 3;
        this->damage = 2;
        this->speed = 3;
        this->xp = xp;
        this->yp = yp;
        this->walking_radius = 10;
        this->attack_radius = 5;
        this->right_direction = true;
    }

    ~soyjak_typical() {
        delete[] inner;
	delete[] effects;
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


int main(){
    int n = 20; //number of plates we see
    int monster_types = 1;
    RectangleShape* monsters = new RectangleShape[monster_types]; //number of monster types
    RectangleShape field(Vector2f(30, 30)); //one element of terrain
    field.setFillColor(Color(90, 90, 90));
    RectangleShape play(Vector2f(10, 25));
    play.setFillColor(Color(0, 0, 0));
    play.setPosition(330, 330);
    for(int i = 0; i < monster_types; i++)
        monsters[i] = RectangleShape(Vector2f(25, 25));
    player* player_1 = new player(0,0,10,10);

    VideoMode vid;
	vid.width = 30*(n+2);
	vid.height = 30*(n+2); //change to size of window
    RenderWindow window(vid, L"Game", Style::Default);
    while (window.isOpen())
    {

        Event event;
        while (window.pollEvent(event))
        {
        //all actions of player, monsters, and others
		player_1->action();
		if (event.type == Event::Closed) window.close();
        }

    window.clear(Color(192, 192, 192));
            for(int j = 0; j < n; j++){
            for(int i = 0; i < n; i++){
                if(player_1->get_x()-(n/2-i)*30 >= 0 && player_1->get_y()-(n/2-j)*30 >= 0 && player_1->get_x()+(i-n/2)*30 <= 390 && player_1->get_y()+(j-n/2)*30 <= 300){
                    field.setPosition(30-player_1->get_y()%30+30*j, 30 - player_1->get_x()%30 + 30*i);
                    window.draw(field);
                }
            }
            }
	    window.draw(play);
        window.display();
    }
    return 0;
}










