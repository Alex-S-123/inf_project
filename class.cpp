#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <chrono>
#include <cmath>
#include <vector>
                               
using namespace sf;


class item{
protected:
    int boost_damage, boost_armor, boost_hp, use_before_break, time_of_active;
    int x0, y0, x1, y1; //texture rect for menue
    char* name;
public:
    //virtual void use_item() = 0;
};


class object{
protected:
    int x, y; //coords
    int x0, y0, x1, y1; //texture rect
	int type;
	bool collision;
	int n;
    
public:
	item** inner;
    object(int sx, int sy, int tx0, int ty0){
        this->x = 0;
        this->y = 0;
        this->x1 = sx;
        this->y1 = sy;
        this->x0 = tx0;
        this->y0 = ty0;
		type = 0;
		inner = new item*[24];
		for(int i = 0; i < 24; i++) inner[i] = NULL;
		n = 0;
		collision = false;
    }

    void set_pos(float x0, float y0){
        this->x = x0;
        this->y = y0;
    }
    int get_centre_x() {
        return x + (this->x1 - this->x0) / 2;
    }
    int get_centre_y() {
        return y + (this->y1 - this->y0) / 2;
    }


    int get_x() {return this->x;}

    int get_y() {return this->y;}
	int get_type() {return type; }
	bool get_collision() {return collision;}
	bool give(item* it){
		if(n == 24||it==NULL) return 0;
		inner[n] = it;
		n++;
		return 1;	
	}
    item* take(int i){
		if(i > n) return NULL;
		item* res = inner[i];
		for(int j = i; j < n-1; j++) inner[j] = inner[j+1];
		inner[n-1] = NULL;
		n--;
		return res;	
	}
	

};




class effect: public item{
    virtual void use(/*monster* m*/); //i dont know how to do it
};

class monster: public object{
protected:
    int hp, armor, damage, speed;
    effect* effects;    //because we have this
    item* inner;
public:
    monster():object(0,0,0,0){};
    void damaged(int damag);
    int atack();
    int get_type(){return this->type;}
    int get_hp() {
        return this->hp;
    }
    void set_hp(int delta_hp) {
        this->hp += delta_hp;
    }
    virtual void behavior(){};
};

class player: public object{
protected:
    int hp, armor, damage, speed;
    effect* effects;    //because we have this
    
	int num_of_items;
    bool attack;
public:
	item** inner;
    std::chrono::time_point<std::chrono::system_clock> last_hit;
    player(int sx, int sy, int tx0, int ty0, int x0, int y0) : object(sx, sy, tx0, ty0){
		x = x0;
		y = y0;
		speed = 1;
		damage = 1;
		inner = new item*[28];
		num_of_items = 0;
		for(int i = 0; i < 28; i++) inner[i] = NULL;
    }
    bool get_attack() {
        return attack;
    }
    int get_damage() {
        return this->damage;
    }

    bool give(item* it){
		if(num_of_items == 24||it==NULL) return 0;
		inner[num_of_items+4] = it;
		num_of_items++;
		return 1;	
	}
    item* take(int i){
		if(i >= num_of_items) return NULL;
		item* res = inner[i+4];
		for(int j = i+4; j < num_of_items+3; j++) inner[j] = inner[j+1];
		inner[num_of_items+3] = NULL;
		num_of_items--;
		return res;	
	}

    void action(int** level, int p, int q, monster** monsters_list, int n, object** objects_list, int no){
	if(Keyboard::isKeyPressed(Keyboard::W) && (x-speed>0) && (level[y/30+1][(x-speed)/30+1] > 0) && (level[(y+10)/30+1][(x-speed)/30+1] > 0)) x -= speed;
	if(Keyboard::isKeyPressed(Keyboard::S) && (x+25 + speed < q*30-30) && (level[y/30+1][(x+25+speed)/30+1] > 0) && (level[(y+10)/30+1][(x+25 + speed)/30+1] > 0)) x += speed;
	if(Keyboard::isKeyPressed(Keyboard::A) && (y-speed > 0) && (level[(y-speed)/30+1][x/30+1] > 0) && (level[(y-speed)/30+1][(x+25)/30+1] > 0) ) y -= speed;
	if(Keyboard::isKeyPressed(Keyboard::D) && (y+10+speed < p*30-30) && (level[(y+10+speed)/30+1][x/30+1] > 0) && (level[(y+10+speed)/30+1][(x+25)/30+1] > 0)) y += speed;
    if(Keyboard::isKeyPressed(Keyboard::Q)) attack = true;
	if(Keyboard::isKeyPressed(Keyboard::Q) == false) attack = false;

	for(int i = 0; i < n; i++){
		if(monsters_list[i] != NULL && monsters_list[i]->get_collision()){
			while((x+25 >= monsters_list[i]->get_x())&&(x+25<=monsters_list[i]->get_centre_x())&&(y<=monsters_list[i]->get_centre_y()&&y+10>=monsters_list[i]->get_centre_y())
					&& (x-1>0) && (level[y/30+1][(x-1)/30+1] > 0) && (level[(y+10)/30+1][(x-1)/30+1] > 0)) x-= 1;
			while((x >= monsters_list[i]->get_centre_x())&&(x<=2*monsters_list[i]->get_centre_x()-monsters_list[i]->get_x())&&(y<=monsters_list[i]->get_centre_y()&&y+10>=monsters_list[i]->get_centre_y())
					&& (x+26 < q*30-30) && (level[y/30+1][(x+26)/30+1] > 0) && (level[(y+10)/30+1][(x+26)/30+1] > 0)) x+= 1;
			while((y+10 >= monsters_list[i]->get_y())&&(y+10<=monsters_list[i]->get_centre_y())&&(x<=monsters_list[i]->get_centre_x()&&x+25>=monsters_list[i]->get_centre_x())
					&& (y-1 > 0) && (level[(y-1)/30+1][x/30+1] > 0) && (level[(y-1)/30+1][(x+25)/30+1] > 0)) y-= 1;
			while((y >= monsters_list[i]->get_centre_y())&&(y<=2*monsters_list[i]->get_centre_y()-monsters_list[i]->get_y())&&(x<=monsters_list[i]->get_centre_x()&&x+25>=monsters_list[i]->get_centre_x())
					&& (y+11 < p*30-30) && (level[(y+11)/30+1][x/30+1] > 0) && (level[(y+11)/30+1][(x+25)/30+1] > 0)) y += 1;
		}
	}
	for(int i = 0; i < no; i++){
		if(objects_list[i] != NULL && objects_list[i]->get_collision()){
			while((x+25 >= objects_list[i]->get_x())&&(x+25<=objects_list[i]->get_centre_x())&&(y<=objects_list[i]->get_centre_y()&&y+10>=objects_list[i]->get_centre_y())
					&& (x-1>0) && (level[y/30+1][(x-1)/30+1] > 0) && (level[(y+10)/30+1][(x-1)/30+1] > 0)) x-= 1;
			while((x >= objects_list[i]->get_centre_x())&&(x<=2*objects_list[i]->get_centre_x()-objects_list[i]->get_x())&&(y<=objects_list[i]->get_centre_y()&&y+10>=objects_list[i]->get_centre_y())
					&& (x+26 < q*30-30) && (level[y/30+1][(x+26)/30+1] > 0) && (level[(y+10)/30+1][(x+26)/30+1] > 0)) x+= 1;
			while((y+10 >= objects_list[i]->get_y())&&(y+10<=objects_list[i]->get_centre_y())&&(x<=objects_list[i]->get_centre_x()&&x+25>=objects_list[i]->get_centre_x())
					&& (y-1 > 0) && (level[(y-1)/30+1][x/30+1] > 0) && (level[(y-1)/30+1][(x+25)/30+1] > 0)) y-= 1;
			while((y >= objects_list[i]->get_centre_y())&&(y<=2*objects_list[i]->get_centre_y()-objects_list[i]->get_y())&&(x<=objects_list[i]->get_centre_x()&&x+25>=objects_list[i]->get_centre_x())
					&& (y+11 < p*30-30) && (level[(y+11)/30+1][x/30+1] > 0) && (level[(y+11)/30+1][(x+25)/30+1] > 0)) y += 1;
		}
	}			//collision does not work  


    for(int i = 0; i < n; i++) {
            if (monsters_list[i] != NULL) {
                // player damages kills monsters
                // attack works for center of monster and player
                auto time_delta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - this->last_hit).count();
                if ((this->get_attack() == true) && (sqrt(pow(monsters_list[i]->get_centre_x() - this->get_centre_x(), 2)+ pow(monsters_list[i]->get_centre_y() - this->get_centre_y(), 2)) < 25)) {
                    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - this->last_hit).count() > 3000) {
                        if (monsters_list[i]->get_hp() - this->get_damage() > 0) {
                            monsters_list[i]->set_hp(-1 * this->get_damage());
                            std::cout << "time delta: " << time_delta << std::endl;
                            std::cout << "monster damaged, monster's hp: " << monsters_list[i]->get_hp() << std::endl;
                            this->last_hit = std::chrono::system_clock::now();
                        } else {
                            monsters_list[i] = NULL;
                            std::cout << "monster killed!" << std::endl;
                        }
                        //play.setFillColor(Color(0, 0, 150));
                    }
                }
            }
        }
    } 
};


class treasure: public object{
protected:
    int n;
    
public:
	item** inner;
    treasure(int x, int y):object(0, 0, 15, 15){
		type = 1;
		n = 0;
		inner = new item*[24];
		set_pos(x, y);
		collision = true;
	}
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
    item* take(int i);
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

class seller: public treasure{
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
		y1 = 15;
		x1 = 30;
		collision = true;
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
	bool inv_open = false;
	int inv_x = 0,  inv_y = 0;
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


	std::ifstream fo("objects.txt");
	int num_of_objects;
	fo >> num_of_objects;
	object** objects_list = new object*[2*(num_of_objects+num_of_monsters)];
	for(int i = 0; i < num_of_objects; i++){
		int type, x0, y0;
		fo >> type >> x0 >> y0;
		if(type == 1){
			objects_list[i] = new treasure(x0, y0);
		}
		// here is creating new objects from file
	}

    int n = 20; //number of plates we see
    int monster_types = 1;
    RectangleShape** monsters_tex = new RectangleShape*[monster_types]; //number of monster types
	monsters_tex[0] = new RectangleShape(Vector2f(15, 30));
	monsters_tex[0]->setFillColor(Color(128, 0, 0));

	int object_types = 1;
    RectangleShape** objects_tex = new RectangleShape*[object_types];
	objects_tex[0] = new RectangleShape(Vector2f(15, 15));
	objects_tex[0]->setFillColor(Color(0, 128, 0));

    RectangleShape field(Vector2f(30, 30)); //one element of terrain
    field.setFillColor(Color(90, 90, 90));
    RectangleShape play(Vector2f(10, 25));
    play.setFillColor(Color(0, 0, 0));
    play.setPosition(330, 330);

	RectangleShape inv_back(Vector2f(550, 600));
	inv_back.setFillColor(Color(139, 69, 19));
	inv_back.setPosition(55, 40);	
	RectangleShape inv_item(Vector2f(60, 60));
	inv_item.setFillColor(Color(0, 0, 0));
	RectangleShape inv_ch(Vector2f(70, 70));
	inv_ch.setFillColor(Color(250, 250, 250));
	RectangleShape inv_bord(Vector2f(5, 600));
	inv_bord.setFillColor(Color(70, 70, 70));


	objects_list[0]->give(new item);

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
		if(Keyboard::isKeyPressed(Keyboard::R)){
			if(inv_open) inv_open = false;
			else inv_open = true;
			while (Keyboard::isKeyPressed(Keyboard::R)) continue; //not good, but I do not see other ways
			inv_x = 0;
			inv_y = 0;
		}
		for(int i = 0; i < num_of_monsters; i++){
				if(monsters_list[i])monsters_list[i]->behavior();
			}
		if(!inv_open)player_1->action(level, p, q, monsters_list, num_of_monsters, objects_list, num_of_objects);
		else{
			if(Keyboard::isKeyPressed(Keyboard::W)) inv_x -= 1;
			if(Keyboard::isKeyPressed(Keyboard::S)) inv_x += 1;
			if(Keyboard::isKeyPressed(Keyboard::A)) inv_y -= 1;
			if(Keyboard::isKeyPressed(Keyboard::D)) inv_y += 1;
    		if(Keyboard::isKeyPressed(Keyboard::Q)){
				int ind = -1;
				//std::cout << abs(player_1->get_centre_x()-objects_list[0]->get_centre_x())+abs(player_1->get_centre_y()-objects_list[0]->get_centre_y()) << std::endl;
				for(int i = 0; i < num_of_objects; i++){
					if(abs(player_1->get_centre_x()-objects_list[i]->get_centre_x())+abs(player_1->get_centre_y()-objects_list[i]->get_centre_y())<40&&objects_list[i]->get_type() == 1) ind = i;				
				}
				if(ind>=0){
					//std::cout << 1 << std::endl;
					object* tr = objects_list[ind];
					if(inv_y < 3)tr->give(player_1->take(inv_x*3+inv_y));
					else player_1->give(tr->take(inv_x*3+inv_y-3));
				}
			}
			if(Keyboard::isKeyPressed(Keyboard::E)){
				if(inv_y < 3){
					item* tmp = player_1->take(inv_x*3+inv_y);
					if(player_1->inner[0]!=tmp) {
						player_1->give(player_1->inner[0]);
						player_1->inner[0] = tmp;
					}
					else if(player_1->inner[1]!=tmp) {
						player_1->give(player_1->inner[1]);
						player_1->inner[1] = tmp;
					}
					else if(player_1->inner[2]!=tmp) {
						player_1->give(player_1->inner[2]);
						player_1->inner[2] = tmp;
					}
					else if(player_1->inner[3]!=tmp) {
						player_1->give(player_1->inner[3]);
						player_1->inner[3] = tmp;
					}
				}
					

			}
			if(inv_x < 0) inv_x = 7;
			if(inv_x > 7) inv_x = 0;
			if(inv_y < 0) inv_y = 5;
			if(inv_y > 5) inv_y = 0;
			while(Keyboard::isKeyPressed(Keyboard::W)||Keyboard::isKeyPressed(Keyboard::S)||Keyboard::isKeyPressed(Keyboard::A)||Keyboard::isKeyPressed(Keyboard::D)||Keyboard::isKeyPressed(Keyboard::Q)||Keyboard::isKeyPressed(Keyboard::E)) continue;
		}

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

		for(int i = 0; i < num_of_objects; i++){
            if(monsters_list[i]){
                if(abs(player_1->get_x()-objects_list[i]->get_x())+abs(player_1->get_y()-objects_list[i]->get_y()) < 40*30){
                    objects_tex[objects_list[i]->get_type()-1]->setPosition(30-player_1->get_y()+objects_list[i]->get_y()+(-cy+10)*30,30-player_1->get_x()+ objects_list[i]->get_x()+ (-cx+10)*30);
                    window.draw(*objects_tex[monsters_list[i]->get_type()-1]);
                }
            }
		}

		for(int i = 0; i < num_of_monsters; i++){
            if(monsters_list[i]){
                if(abs(player_1->get_x()-monsters_list[i]->get_x())+abs(player_1->get_y()-monsters_list[i]->get_y()) < 40*30){
                    monsters_tex[monsters_list[i]->get_type()-1]->setPosition(30-player_1->get_y()+monsters_list[i]->get_y()+(-cy+10)*30,30-player_1->get_x()+ monsters_list[i]->get_x()+ (-cx+10)*30);
                    window.draw(*monsters_tex[monsters_list[i]->get_type()-1]);
                }
            }
		}
	    window.draw(play);
		if(inv_open){
			window.draw(inv_back);
			inv_ch.setPosition(55+130+inv_y*70-5, 40+40+inv_x*70-5);
			window.draw(inv_ch);
			inv_bord.setPosition(55+125-2, 40);
			window.draw(inv_bord);
			inv_bord.setPosition(55+125+210-2, 40);
			window.draw(inv_bord);
			int ind = -1;
			for(int i = 0; i < num_of_objects; i++){
				if(abs(player_1->get_centre_x()-objects_list[i]->get_centre_x())+abs(player_1->get_centre_y()-objects_list[i]->get_centre_y())<40&&objects_list[i]->get_type() == 1) ind = i;				
			}
			for(int i = 0; i < 48; i++){
				inv_item.setPosition(55+130+(i%6)*70, 40+40+(i/6)*70);
				if(i%6 < 3 && player_1->inner[(i/6)*3+i%6+4]) inv_item.setFillColor(Color(90, 90, 90));
				if(ind >= 0 && i%6 >= 3 && objects_list[ind]->inner[(i/6)*3+i%6-3]) inv_item.setFillColor(Color(90, 90, 90));
				window.draw(inv_item);
				inv_item.setFillColor(Color(0, 0, 0));
			}
			if(player_1->inner[0]) inv_item.setFillColor(Color(90, 90, 90));
			inv_item.setPosition(55+15, 40+40+40);
			window.draw(inv_item);
			inv_item.setFillColor(Color(0, 0, 0));
			if(player_1->inner[1]) inv_item.setFillColor(Color(90, 90, 90));
			inv_item.setPosition(55+15, 40+40+40+140);
			window.draw(inv_item);
			inv_item.setFillColor(Color(0, 0, 0));
			if(player_1->inner[2]) inv_item.setFillColor(Color(90, 90, 90));
			inv_item.setPosition(55+15, 40+40+40+280);
			window.draw(inv_item);
			inv_item.setFillColor(Color(0, 0, 0));
			if(player_1->inner[3]) inv_item.setFillColor(Color(90, 90, 90));
			inv_item.setPosition(55+15, 40+40+40+280+140);
			window.draw(inv_item);
			inv_item.setFillColor(Color(0, 0, 0));
			

		}
        window.display();
		while(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()-start).count() < 25) continue;
    }
    return 0;
}
