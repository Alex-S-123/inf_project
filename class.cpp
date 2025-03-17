#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <chrono>
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
    monster():object(0,0,0,0){};
    void damaged(int damag);
    int atack();
    int get_type(){return this->type;}
    virtual void behavior(){};
};

class player: public object{
protected:
    int hp, armor, damage, speed;
    int type;
    effect* effects;    //because we have this
    item* inner;
public:
    player(int sx, int sy, int tx0, int ty0, int x0, int y0) : object(sx, sy, tx0, ty0){
		x = x0;
		y = y0;
		speed = 1;
    }
    void action(int** level, int p, int q){
	if(Keyboard::isKeyPressed(Keyboard::W) && (x-speed>0) && (level[y/30+1][(x-speed)/30+1] > 0) && (level[(y+10)/30+1][(x-speed)/30+1] > 0)) x -= speed;
	if(Keyboard::isKeyPressed(Keyboard::S) && (x+25 + speed < q*30-30) && (level[y/30+1][(x+25+speed)/30+1] > 0) && (level[(y+10)/30+1][(x+25 + speed)/30+1] > 0)) x += speed;
	if(Keyboard::isKeyPressed(Keyboard::A) && (y-speed > 0) && (level[(y-speed)/30+1][x/30+1] > 0) && (level[(y-speed)/30+1][(x+25)/30+1] > 0) ) y -= speed;
	if(Keyboard::isKeyPressed(Keyboard::D) && (y+10+speed < p*30-30) && (level[(y+10+speed)/30+1][x/30+1] > 0) && (level[(y+10+speed)/30+1][(x+25)/30+1] > 0)) y += speed;
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
    bool right_direction, x_direction;
    //need texture size!
public:
    soyjak_typical(int xp, int yp, int rad, bool x_dir){
		this-> x_direction = x_dir;
        this->hp = 6;
        this->armor = 3;
        this->damage = 2;
        this->speed = 1;
        this->xp = xp;
        this->yp = yp;
        this->walking_radius = rad;
        this->attack_radius = 5;
        this->right_direction = true;
		this->type = 1;
		this->set_pos(xp, yp);
    }

    ~soyjak_typical() {
        delete[] inner;
		delete[] effects;
    }

    void behavior() {
        // walking (now exists only on x-axis)
		if(this->x_direction){
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
		}
		else{
			if (this->right_direction) {
		        if (this->y <= this->y0 + walking_radius) {
		            this->y += this->speed;
		        } else {
		            this->right_direction = false;
		        }
		    } else {
		        if (this->y >= this->y0 - walking_radius) {
		            this->y -= this->speed;
		        } else {
		            this->right_direction = true;
		        }
		    }

		}
        // attack (attacks only on x-axis)
        //if (pow(pow(player.get_x() - this->x, 2) + pow(player.get_y() - this->y, 2), 1/2)) {
            // change of image to attacking image
        //}
    }
};


int main(){
	std::ifstream f("field.txt");
	int p, q;
	f >> p >> q;
    int** level = new int* [p+2];
	level[0] = new int [q+2];
	level[p+1] = new int [q+2];
	for(int i = 1; i < q+2; i++){
		level[0][i] = 0;
		level[p+1][i] = 0;
	}
	for(int i = 1; i < p+1; i++){
		level[i] = new int[15];
		for(int j = 0; j < q+2; j++){
			if(j == 0 || j == q+1)level[i][j] = 0;
			else f >> level[i][j];
		}
	}
	f.close();



	std::ifstream fm("monsters.txt");
	int num_of_monsters;
	fm >> num_of_monsters;
	monster** monsters_list = new monster*[num_of_monsters];
	for(int i = 0; i < num_of_monsters; i++){
		int type, x0, y0;
		fm >> type;
		if(type == 1){
			int rad, dir;
			fm >> x0 >> y0 >> rad >> dir;
			monsters_list[i] = new soyjak_typical(x0, y0, rad, dir);
		}
	}
    int n = 20; //number of plates we see
    int monster_types = 1;
    RectangleShape** monsters_tex = new RectangleShape*[monster_types]; //number of monster types
	monsters_tex[0] = new RectangleShape(Vector2f(15, 30));
	monsters_tex[0]->setFillColor(Color(128, 0, 0));
    RectangleShape field(Vector2f(30, 30)); //one element of terrain
    field.setFillColor(Color(90, 90, 90));
    RectangleShape play(Vector2f(10, 25));
    play.setFillColor(Color(0, 0, 0));
    play.setPosition(330, 330);
	int cx = 0, cy= 0;
    player* player_1 = new player(0,0,10,10, 30*cx, 30*cy);

    VideoMode vid;
	vid.width = 30*(n+2);
	vid.height = 30*(n+2); //change to size of window
    RenderWindow window(vid, L"Game", Style::Default);
    while (window.isOpen())
    {
		auto start = std::chrono::system_clock::now();
		//all actions of player, monsters, and others
		for(int i = 0; i < num_of_monsters; i++){
				monsters_list[i]->behavior();
			}
			player_1->action(level, p, q);

        Event event;
        while (window.pollEvent(event))
        {
        	
			
			if (event.type == Event::Closed) window.close();
        }

    window.clear(Color(192, 192, 192));
        for(int j = 0; j < q; j++){
            for(int i = 0; i < p; i++){
                if(abs(player_1->get_x()-30*i)+abs(player_1->get_y() - 30*j) < 41*30 && level[i][j] != 0){
                    field.setPosition(30-player_1->get_y()+30*i + (-cy+9)*30, 30 - player_1->get_x() + 30*j + (-cx+9)*30);
                    window.draw(field);
                }
            }
        }
		for(int i = 0; i < num_of_monsters; i++){
			if(abs(player_1->get_x()-monsters_list[i]->get_x())+abs(player_1->get_y()-monsters_list[i]->get_y()) < 40*30){
				monsters_tex[monsters_list[i]->get_type()-1]->setPosition(30-player_1->get_y()+monsters_list[i]->get_y()+(-cy+10)*30,30-player_1->get_x()+ monsters_list[i]->get_x()+ (-cx+10)*30);
				window.draw(*monsters_tex[monsters_list[i]->get_type()-1]);
			}		
		}
	    window.draw(play);
        window.display();
		while(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()-start).count() < 25) continue;
    }
    return 0;
}
