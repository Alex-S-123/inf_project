#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <chrono>
#include <cmath>
#include <string>
using namespace sf;


class item{
protected:
    int x0, y0, x1, y1; //texture rect for menue
    //char* name;
public:
	int boost_damage, boost_armor, boost_hp, boost_speed, boost_atack_speed, boost_dist,  type, number, cost;
	item(int t, int damage, int armor, int hp, int speed, int atack_speed, int dist,  int c, int nu){
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
	item(){
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
    //virtual void use_item() = 0;
};


class object{
protected:
    int x, y; //coords
    int x0, y0, x1, y1; //texture rect
	int type;
	bool collision;
	//int n;

public:
	item** inner;
    object(int sx, int sy, int tx0, int ty0){
        this->x = 0;
        this->y = 0;
        this->x0 = sx;
        this->y0 = sy;
        this->x1 = tx0;
        this->y1 = ty0;
		type = 0;
		inner = new item*[24];
		for(int i = 0; i < 24; i++) inner[i] = NULL;
		collision = false;
    }

	IntRect tex_rect(){
		return IntRect(x0, y0, x1-x0, y1-y0);
	}

    void set_pos(float x0, float y0){
        this->x = x0;
        this->y = y0;
    }
    int get_centre_x() {
        return x + abs(this->y1 - this->y0) / 2;
    }
    int get_centre_y() {
        return y + abs(this->x1 - this->x0) / 2;
    }


    int get_x() {return this->x;}

    int get_y() {return this->y;}
	int get_type() {return type; }
	bool get_collision() {return collision;}


	virtual bool give(item* it){return 0;}
	virtual item* take(int i){return NULL;}
	virtual int destination(){
		return 0;
	}
};

class player;

class monster: public object{
protected:
    int hp, armor, damage, speed;
	int frames_of_walk;//, frames_after_atack;
	bool fly;

    //item* inner;
public:
    monster():object(0,0,0,0){
		fly = false;
	};
    void damaged(int damag);
    int atack();
	bool is_flying(){
		return fly;
	}
    int get_type(){return this->type;}
    int get_hp() {
        return this->hp;
    }
    void set_hp(int delta_hp) {
        this->hp += delta_hp;
    }
    virtual void behavior(player *p){};
};

class player: public object{
protected:
    int hp, armor, damage, speed, time_atack, dist_atack;
	int num_of_items, gold;
    bool attack, r_dir, is_walking;
	int frames_after_atack, frames_of_walk;
	int xsize, ysize;
public:
		item** inner;
		std::chrono::time_point<std::chrono::system_clock> last_hit;
		std::chrono::time_point<std::chrono::system_clock> last_dist_hit;
		bool dead;
		player(int sx, int sy, int tx0, int ty0, int health) : object(sx, sy, tx0, ty0){
			x = 0;
			y = 0;
			speed = 1;
			damage = 1;
			armor = 0;
			gold = 0;
			dist_atack = 25;
			inner = new item*[28];
			num_of_items = 0;
			time_atack = 3000;
			hp = health;
			frames_after_atack = 20;
			frames_of_walk = 0;
			r_dir = false;
			is_walking = false;
			xsize = 36;
			ysize = 20;
			for(int i = 0; i < 28; i++) inner[i] = NULL;
		}
		int get_hp() {
            return this->hp;
        }
		bool get_attack() {
			return attack;
		}
		int get_damage() {
			return this->damage;
		}

		bool can_atack_dist(){
			if(inner[2]==NULL) return 0;
			return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - this->last_dist_hit).count() >= inner[2]->boost_atack_speed;
		}

		bool give(item* it){
			if(num_of_items == 24||it==NULL) return 0;
			if(it->type == 5){
				gold += it->cost;
				return 1;
			}
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

		void damaged(int damag) {
        //this->hp -= std::max(0, abs(this->armor - hp));
            if (this->armor < damag) {
                this->hp -= (damag - this->armor);
                if (this->hp <= 0) {
                    std::cout << "death" << std::endl;
                    //this->~player();
                }
            }
        }
		void set_hp(int he){
			hp = he;
		}
		int get_gold(){
			return gold;
		}

		void action(int** level, int p, int q, monster** monsters_list, int n, object** objects_list, int no){
		if(Keyboard::isKeyPressed(Keyboard::W) && (x-speed>0) && (level[y/30+1][(x-speed)/30+1] > 0) && (level[(y+ysize)/30+1][(x-speed)/30+1] > 0)) {x -= speed; r_dir = true; is_walking = true; frames_of_walk++;}
		if(Keyboard::isKeyPressed(Keyboard::S) && (x+xsize + speed < q*30-30) && (level[y/30+1][(x+xsize+speed)/30+1] > 0) && (level[(y+ysize)/30+1][(x+xsize + speed)/30+1] > 0)) {x += speed; r_dir = false; is_walking = true; frames_of_walk++;}
		if(Keyboard::isKeyPressed(Keyboard::A) && (y-speed > 0) && (level[(y-speed)/30+1][x/30+1] > 0) && (level[(y-speed)/30+1][(x+xsize)/30+1] > 0) )  {y -= speed; r_dir = false; is_walking = true;frames_of_walk++;}
		if(Keyboard::isKeyPressed(Keyboard::D) && (y+ysize+speed < p*30-30) && (level[(y+ysize+speed)/30+1][x/30+1] > 0) && (level[(y+ysize+speed)/30+1][(x+xsize)/30+1] > 0))  {y += speed; r_dir = true; is_walking = true;frames_of_walk++;}
		if(Keyboard::isKeyPressed(Keyboard::Q)) attack = true;
		if(Keyboard::isKeyPressed(Keyboard::Q) == false) attack = false;
		if(Keyboard::isKeyPressed(Keyboard::E)){
			if(inner[2]){
				int ind = -1;
				float mi = 10000000;
				for(int i = 0; i < n; i++){
					if(monsters_list[i]){
						float ro = sqrt(pow(monsters_list[i]->get_centre_x() - this->get_centre_x(), 2)+ pow(monsters_list[i]->get_centre_y() - this->get_centre_y(), 2));
						if(ro<mi){
							mi = ro;
							ind = i;
						}

					}
				}
				auto time_dist_delta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - this->last_dist_hit).count();
				if(ind >= 0 && mi<= inner[2]->boost_dist && time_dist_delta >= inner[2]->boost_atack_speed){
					//monsters_list[ind]->damaged(inner[2]->boost_damage);
					if (monsters_list[ind]->get_hp() - inner[2]->boost_damage > 0) {
						monsters_list[ind]->set_hp(-1 * inner[2]->boost_damage);
						std::cout << "time delta: " << time_dist_delta << std::endl;
						std::cout << "monster damaged, monster's hp: " << monsters_list[ind]->get_hp() << std::endl;
					} else {
						monsters_list[ind] = NULL;
						std::cout << "monster killed!" << std::endl;
					}
					last_dist_hit = std::chrono::system_clock::now();
				}
			}
		}
		for(int i = 0; i<2; i++){
			if(inner[i]){
				speed += inner[i]->boost_speed;
				damage += inner[i]->boost_damage;
				time_atack -= inner[i]->boost_atack_speed;
				armor += inner[i]->boost_armor;
				dist_atack += inner[i]->boost_dist;
			}
		}
		for(int i = 0; i < n; i++){
			if(monsters_list[i] != NULL && monsters_list[i]->get_collision()){
				while((x+xsize >= monsters_list[i]->get_x())&&(x+xsize<=monsters_list[i]->get_centre_x())&&(y<=monsters_list[i]->get_centre_y()&&y+ysize>=monsters_list[i]->get_centre_y())
						&& (x-1>0) && (level[y/30+1][(x-1)/30+1] > 0) && (level[(y+ysize)/30+1][(x-1)/30+1] > 0)) x-= 1;
				while((x >= monsters_list[i]->get_centre_x())&&(x<=2*monsters_list[i]->get_centre_x()-monsters_list[i]->get_x())&&(y<=monsters_list[i]->get_centre_y()&&y+ysize>=monsters_list[i]->get_centre_y())
						&& (x+xsize+1 < q*30-30) && (level[y/30+1][(x+xsize+1)/30+1] > 0) && (level[(y+ysize)/30+1][(x+xsize+1)/30+1] > 0)) x+= 1;
				while((y+ysize >= monsters_list[i]->get_y())&&(y+ysize<=monsters_list[i]->get_centre_y())&&(x<=monsters_list[i]->get_centre_x()&&x+xsize>=monsters_list[i]->get_centre_x())
						&& (y-1 > 0) && (level[(y-1)/30+1][x/30+1] > 0) && (level[(y-1)/30+1][(x+xsize)/30+1] > 0)) y-= 1;
				while((y >= monsters_list[i]->get_centre_y())&&(y<=2*monsters_list[i]->get_centre_y()-monsters_list[i]->get_y())&&(x<=monsters_list[i]->get_centre_x()&&x+xsize>=monsters_list[i]->get_centre_x())
						&& (y+ysize+1 < p*30-30) && (level[(y+ysize+1)/30+1][x/30+1] > 0) && (level[(y+ysize)/30+1][(x+xsize)/30+1] > 0)) y += 1;
			}
		}

		for(int i = 0; i < no; i++){
			if(objects_list[i] != NULL && objects_list[i]->get_collision()){
				while((x+xsize >= objects_list[i]->get_x())&&(x+xsize<=objects_list[i]->get_centre_x())&&(y<=objects_list[i]->get_centre_y()&&y+ysize>=objects_list[i]->get_centre_y())
						&& (x-1>0) && (level[y/30+1][(x-1)/30+1] > 0) && (level[(y+ysize)/30+1][(x-1)/30+1] > 0)) x-= 1;
				while((x >= objects_list[i]->get_centre_x())&&(x<=2*objects_list[i]->get_centre_x()-objects_list[i]->get_x())&&(y<=objects_list[i]->get_centre_y()&&y+ysize>=objects_list[i]->get_centre_y())
						&& (x+xsize+1 < q*30-30) && (level[y/30+1][(x+xsize+1)/30+1] > 0) && (level[(y+ysize)/30+1][(x+xsize+1)/30+1] > 0)) x+= 1;
				while((y+ysize >= objects_list[i]->get_y())&&(y+ysize<=objects_list[i]->get_centre_y())&&(x<=objects_list[i]->get_centre_x()&&x+xsize>=objects_list[i]->get_centre_x())
						&& (y-1 > 0) && (level[(y-1)/30+1][x/30+1] > 0) && (level[(y-1)/30+1][(x+xsize)/30+1] > 0)) y-= 1;
				while((y >= objects_list[i]->get_centre_y())&&(y<=2*objects_list[i]->get_centre_y()-objects_list[i]->get_y())&&(x<=objects_list[i]->get_centre_x()&&x+xsize>=objects_list[i]->get_centre_x())
						&& (y+ysize+1 < p*30-30) && (level[(y+ysize+1)/30+1][x/30+1] > 0) && (level[(y+ysize)/30+1][(x+xsize)/30+1] > 0)) y += 1;
			}
		}
		if(attack&&std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - this->last_hit).count()>time_atack) frames_after_atack = 0;

    	for(int i = 0; i < n; i++) {
            if (monsters_list[i] != NULL) {
                // player damages kills monsters
                // attack works for center of monster and player
                auto time_delta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - this->last_hit).count();
                if ((this->get_attack() == true) && (sqrt(pow(monsters_list[i]->get_centre_x() - this->get_centre_x(), 2)+ pow(monsters_list[i]->get_centre_y() - this->get_centre_y(), 2)) < dist_atack)&&(monsters_list[i]->is_flying()==false)) {
                    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - this->last_hit).count() > time_atack) {
                        if (monsters_list[i]->get_hp() - this->get_damage() > 0) {
                            monsters_list[i]->set_hp(-1 * this->get_damage());
                            std::cout << "time delta: " << time_delta << std::endl;
                            std::cout << "monster damaged, monster's hp: " << monsters_list[i]->get_hp() << std::endl;
                            this->last_hit = std::chrono::system_clock::now();
                        } else {
                            monsters_list[i] = NULL;
                            std::cout << "monster killed!" << std::endl;
                        }
                    }
                }
            }
        }
		speed = 1;
		damage = 1;
		armor = 0;
		time_atack = 3000;
		dist_atack = 25;


		if(frames_after_atack < 20){
			frames_after_atack ++;
			if(r_dir){
				x0 = 0;
				y0 = 108;
				x1 = 20;
				y1 = 144;
			}
			else{
				x0 = 20;
				y0 = 108;
				x1 = 0;
				y1 = 144;
			}
		}
		else{
			if(is_walking == false){
				x0 = 0;
				y0 = 0;
				x1 = 20;
				y1 = 36;
			}
			else if(r_dir){
				x0 = 0;
				y0 = 36+(frames_of_walk%20/10)*36;
				x1 = 20;
				y1 = 72+(frames_of_walk%20/10)*36;
			}
			else{
				x0 = 20;
				y0 = 36+(frames_of_walk%20/10)*36;
				x1 = 0;
				y1 = 72+(frames_of_walk%20/10)*36;
			}
		}


		frames_of_walk = frames_of_walk%20;
		is_walking = false;
    }
};


class treasure: public object{
protected:
    int n;

public:
    treasure(int x, int y):object(0, 0, 15, 15){
		type = 1;
		n = 0;
		//collision = true;
		set_pos(x, y);
	}

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

class teleport:public object{
protected:
    int go_to;
public:
	teleport(int x0, int y0, int dest):object(0, 0, 40, 60){
		x = x0;
		y = y0;
		type = 2;
		go_to = dest;
	}
	int destination(){
		return go_to;
	}
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
    bool damaged_player;
    int attack_period;
    int wait_time;

public:
    bool is_attacking;
    int random_start;
    std::chrono::time_point<std::chrono::system_clock> last_hit;
    soyjak_typical(int xp, int yp, int rad, bool x_dir){
		this-> x_direction = x_dir;
        this->hp = 6;
        this->armor = 0;
        this->damage = 2;
        this->speed = 1;
        this->xp = xp;
        this->yp = yp;
        this->walking_radius = rad;
        this->attack_radius = 50;
        this->right_direction = true;
        this->attack_period = 4000;
		this->type = 1;
		this->set_pos(xp, yp);
		y1 = 46;
		x1 = 28;
		collision = true;
		this->is_attacking = false;
		this->damaged_player = false;
		frames_of_walk = 0;
		this->random_start = rand() % (5000 - 2000 + 1) + 2000;
		last_hit = std::chrono::system_clock::now();
    }


    void behavior(player *p) {
        //std::cout<<"behave"<<std::endl;
        // walking (now exists only on x-axis)
		frames_of_walk = (frames_of_walk+1)%20;
		if(this->x_direction){
		    if (this->right_direction) {
		        if (this->x <= this->xp + walking_radius) {
		            this->x += this->speed;
		        } else {
		            this->right_direction = false;
		        }
		    } else {
		        if (this->x >= this->xp - walking_radius) {
		            this->x -= this->speed;
		        } else {
		            this->right_direction = true;
		        }
		    }
		}
		else{
			if (this->right_direction) {
		        if (this->y <= this->yp + walking_radius) {
		            this->y += this->speed;
		        } else {
		            this->right_direction = false;
		        }
		    } else {
		        if (this->y >= this->yp - walking_radius) {
		            this->y -= this->speed;
		        } else {
		            this->right_direction = true;
		        }
		    }

		}



		if ((p->dead==0)&&(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - this->last_hit).count() < 500) && (this->is_attacking == 1)) {
            if (
                (pow(abs(this->get_centre_x() - p->get_centre_x()), 2) + pow(abs(this->get_centre_y() - p->get_centre_y()), 2) <= pow(this->attack_radius, 2))
                && (this->damaged_player == false)
                )
            {
                this->last_hit = std::chrono::system_clock::now();
                std::cout << "player damaged " << p->get_hp() << std::endl;
                if (p->get_hp() == false) {
                    p->dead = true;
                }
                this->damaged_player = true;
                p->damaged(1);
            }
        }
        if ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - this->last_hit).count() >= 500) && (this->is_attacking == true)) {
            this->is_attacking = false;
        }
        int counted = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - this->last_hit).count();
        // monsters don't attack at the same moment
        if ((counted - random_start >= this->attack_period) && (this->is_attacking == false)) {
            if (random_start) {
                random_start = 0;
            }
            if (this->is_attacking == false) {
                this->is_attacking = true;
                this->damaged_player = false;
                this->last_hit = std::chrono::system_clock::now();
            }
        }

		if(is_attacking){
			if(right_direction){
				x0 = 0;
				y0 = 92;
				x1 = 28;
				y1 = 138;
			}
			else{
				x0 = 28;
				y0 = 92;
				x1 = 0;
				y1 = 138;
			}
		}
		else{
			if(right_direction){
				x0 = 0;
				y0 = 0+(frames_of_walk/10)*46;
				x1 = 28;
				y1 = 46+(frames_of_walk/10)*46;
			}
			else{
				x0 = 28;
				y0 = 0+(frames_of_walk/10)*46;
				x1 = 0;
				y1 = 46+(frames_of_walk/10)*46;
			}
		}
    }
};

class bomb: public monster {
// bomb falls out of flight periodically
// if bomb falls on player, player is damaged
protected:
public:
    bomb(int sx, int sy, int tx0, int ty0, int speed) {
    }
    ~bomb() {}
    void behavior() {
        this->set_pos(x - speed, y);
        // if bomb contacts ground, it is destroyed! It hasn't been created yet.
    }
};

class flight: public monster {
// monster flies up and down
// monster throws bombs
// monster falls if it dies
protected:
    int xp, yp;
    int fly_radius;
    bool up_direction;
    int period_bomb;
    int time_last;
public:
    flight(int xp, int yp, int tx0, int ty0) {
        this->period_bomb = rand() % (7000 - 3000 + 1) / 3000;
        this->xp = xp;
        this->yp = yp;
        this->time_last = 0;
        this->fly_radius = 10;
        this->speed = 8;
		this->fly = true;
        this->up_direction = true;
    }
    void behavior() {
        // flying (now exists only on x-axis)
        if (this->up_direction) {
            if (this->x <= this->xp + this->fly_radius) {
                this->x += this->speed;
            } else {
                this->up_direction = false;
            }
        } else {
            if (this->x >= this->x0 - up_direction) {
                this->x -= this->speed;
            } else {
                this->up_direction = true;
            }
        }
        // attack (attacks only on x-axis)
        if (clock() - this->time_last > this->period_bomb) {
            this->time_last = 0;
            // bomb is created
        }
    }
};

void load_level(int n_of_level, int**& level, monster**& monsters_list, object**& objects_list, int& num_of_monsters, int& num_of_objects, int& p, int& q, player* pla){

	if(level){
		for(int i = 0; i < p+2; i++){
				delete[] level[i];
		}
		delete[] level;
	}
	for(int i = 0; i < num_of_monsters; i++){
		delete monsters_list[i];
	}
	if(monsters_list) delete[] monsters_list;
	for(int i = 0; i < num_of_objects; i++){
		delete objects_list[i];
	}
	if(objects_list)delete[] objects_list;

	std::ifstream f(std::to_string(n_of_level)+"/field.txt");
	int xplay, yplay;
	f>>xplay >> yplay;
	pla->set_pos(xplay, yplay);
	f>>p>>q;
	level = new int* [p+2];
	level[0] = new int [q+2];
	level[p+1] = new int [q+2];
	for(int i = 0; i < q+2; i++){
		level[0][i] = 0;
		level[p+1][i] = 0;
	}
	for(int i = 1; i < p+1; i++){
		level[i] = new int[q+2];
		for(int j = 0; j < q+2; j++){
			if(j == 0 || j == q+1)level[i][j] = 0;
			else f >> level[i][j];
		}
	}
	f.close();



	std::ifstream fm(std::to_string(n_of_level)+"/monsters.txt");
	fm >> num_of_monsters;
	monsters_list = new monster*[num_of_monsters];
	for(int i = 0; i < num_of_monsters; i++){
		int type, x0, y0;
		fm >> type;
		if(type == 1){
			int rad, dir;
			fm >> x0 >> y0 >> rad >> dir;
			monsters_list[i] = new soyjak_typical(x0, y0, rad, dir);
		}
	}
	fm.close();


	std::ifstream fo(std::to_string(n_of_level)+"/objects.txt");
	fo >> num_of_objects;
	objects_list = new object*[2*(num_of_objects+num_of_monsters)];
	for(int i = 0; i < num_of_objects; i++){
		int type, x0, y0;
		fo >> type >> x0 >> y0;
		if(type == 1){
			objects_list[i] = new treasure(x0, y0);
		}
		if(type == 2){
			int dest;
			fo>>dest;
			objects_list[i] = new teleport(x0, y0, dest);
		}
		// here is creating new objects from file
	}

}

void load_saving(int& nu_lev, int**& level, monster**& monsters_list, object**& objects_list, int& num_of_monsters, int& num_of_objects, int& p, int& q, player* pla){
	std::ifstream fs("save.txt");
	int n_of_level;
	fs >> n_of_level;
	if(n_of_level > 0){
		nu_lev = n_of_level;
		load_level(n_of_level, level, monsters_list, objects_list, num_of_monsters, num_of_objects, p, q, pla);
		int he;
		fs >> he;
		pla->set_hp(he);
		for(int i = 0; i < 28; i++){
			int type;
			fs >> type;
			if(type > 0){
				int a, b, c, d, e, f, g, h, k, l, o;
				fs >> a >> b >> c >> d >> e >> f >> g >> h;
				pla->inner[i] = new item(type, a, b, c, d, e, f, g, h);
			}
			else{
				pla->inner[i] = NULL;
			}
		}
	}
	else{
		nu_lev = 1;
		load_level(1, level, monsters_list, objects_list, num_of_monsters, num_of_objects, p, q, pla);
	}
	
}


void creat_saving(int num_of_level, player* pla){
	std::ofstream fns("save.txt");
	fns << num_of_level << '\n';
	fns << pla->get_hp() << '\n';
	for(int i = 0; i < 28; i++){
		if(pla->inner[i]){
			//type, boost_damage, boost_armor, boost_hp, boost_speed, boost_atack_speed, boost_dist, number, cost;
			fns << pla->inner[i]->type << " " << pla->inner[i]->boost_damage << " " << pla->inner[i]->boost_armor 
			<< " " << pla->inner[i]->boost_hp << " " << pla->inner[i]->boost_speed << " " 
			<< pla->inner[i]->boost_atack_speed << " " << pla->inner[i]->boost_dist 
			<< " " << pla->inner[i]->cost << " " << pla->inner[i]->number << '\n';
		}
		else{
			fns << 0 << '\n';
		}
	}
	fns.close();
}


int main(){
	bool inv_open = false;
	int num_of_monsters = 0;
	int inv_x = 0,  inv_y = 0, menue_x = 0;
	int p = 0, q = 0;
	int num_of_objects = 0;
	monster** monsters_list = NULL;
	object** objects_list = NULL;
	int** level = NULL;
	int num_of_level = 1;
	bool in_main_menue = true;
	int max_hp = 5;
	Font font;
	font.loadFromFile("textures/arial.ttf");
	Text text;
	text.setFont(font);
	text.setFillColor(Color(250, 250, 250));
	text.setString("HELLOW");
	text.setCharacterSize(25);


	

	player* player_1 = new player(0,0,36,20,max_hp);

	load_level(num_of_level, level, monsters_list, objects_list, num_of_monsters, num_of_objects, p, q, player_1);
	bool dist_atacked = false;
	int n_iter_drawing = 1000, ind_of_aim = 0;
    int n = 20; //number of plates we see
    int monster_types = 1;
    RectangleShape** monsters_tex = new RectangleShape*[monster_types]; //number of monster types
	monsters_tex[0] = new RectangleShape(Vector2f(28, 46));
	//monsters_tex[0]->setFillColor(Color(128, 0, 0));

	int object_types = 2;
    RectangleShape** objects_tex = new RectangleShape*[object_types];
	objects_tex[0] = new RectangleShape(Vector2f(15, 15));
	objects_tex[0]->setFillColor(Color(0, 128, 0)); //comment after adding textures
	objects_tex[1] = new RectangleShape(Vector2f(40, 60));
	objects_tex[1]->setFillColor(Color(0, 0, 128));

    RectangleShape field(Vector2f(30, 30)); 
    RectangleShape play(Vector2f(20, 36));
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
	RectangleShape inv_text_back(Vector2f(100, 80));
	inv_text_back.setFillColor(Color(0, 0, 0)); 

	RectangleShape main_back(Vector2f(30*(n+2), 30*(n+2)));
	main_back.setFillColor(Color(200, 200, 200));
	RectangleShape dead_back(Vector2f(30*(n+2), 30*(n+2)));
	dead_back.setFillColor(Color(0,0,0,150));
	RectangleShape button_back(Vector2f(250, 36));
	button_back.setFillColor(Color(140, 70, 20));
	RectangleShape button_ch(Vector2f(258, 44));
	button_ch.setFillColor(Color(250, 250, 250));

	//comment line after
	player_1->give(new item(2, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000));
    

	CircleShape aim(5.f);
	//add texture


	Texture field_tex;
	field_tex.loadFromFile("textures/terrains.png");
	field.setTexture(&field_tex);

	Texture player_tex;
	player_tex.loadFromFile("textures/player.png");
	play.setTexture(&player_tex);
	Texture** mons_te = new Texture*[monster_types];
	mons_te[0] = new Texture;
	mons_te[0]->loadFromFile("textures/soyjak.png");
	for(int i = 0; i < monster_types; i++){
		monsters_tex[i]->setTexture(mons_te[i]);
	}


    VideoMode vid;
	vid.width = 30*(n+2);
	vid.height = 30*(n+2); //change to size of window
    RenderWindow window(vid, L"Game", Style::Default);
    while (window.isOpen())
    {
		auto start = std::chrono::system_clock::now();
		
		//all actions of player, monsters, and others
		if(in_main_menue == false&&player_1->dead==false){
			if(Keyboard::isKeyPressed(Keyboard::F)){
				if(inv_open) inv_open = false;
				else inv_open = true;
				while (Keyboard::isKeyPressed(Keyboard::F)) continue; //not good, but I do not see other ways
				inv_x = 0;
				inv_y = 0;
			}
			for(int i = 0; i < num_of_monsters; i++){
					if(monsters_list[i])monsters_list[i]->behavior(player_1);
				}
			if((!inv_open) && (player_1->dead == 0)){
				player_1->action(level, p, q, monsters_list, num_of_monsters, objects_list, num_of_objects);
				if(Keyboard::isKeyPressed(Keyboard::T)){
					int ind = -1;
					//std::cout << abs(player_1->get_centre_x()-objects_list[0]->get_centre_x())+abs(player_1->get_centre_y()-objects_list[0]->get_centre_y()) << std::endl;
					for(int i = 0; i < num_of_objects; i++){
						if(abs(player_1->get_centre_x()-objects_list[i]->get_centre_x())+abs(player_1->get_centre_y()-objects_list[i]->get_centre_y())<40&&objects_list[i]->get_type() == 2) ind = i;
					}
					if(ind>=0){
						num_of_level = objects_list[ind]->destination();
						load_level(num_of_level, level, monsters_list, objects_list, num_of_monsters, num_of_objects, p, q, player_1);
						creat_saving(num_of_level, player_1);
					}
				}
			}
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
						if(tmp == NULL){
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
						else{
							player_1->give(player_1->inner[tmp->type]);
							player_1->inner[tmp->type] = tmp;
						}
					}


				}
				if(inv_x < 0) inv_x = 7;
				if(inv_x > 7) inv_x = 0;
				if(inv_y < 0) inv_y = 5;
				if(inv_y > 5) inv_y = 0;
				while(Keyboard::isKeyPressed(Keyboard::W)||Keyboard::isKeyPressed(Keyboard::S)||Keyboard::isKeyPressed(Keyboard::A)||Keyboard::isKeyPressed(Keyboard::D)||Keyboard::isKeyPressed(Keyboard::Q)||Keyboard::isKeyPressed(Keyboard::E)) continue;
			}
		}else if(in_main_menue){
			if(Keyboard::isKeyPressed(Keyboard::W)) menue_x -= 1;
			if(Keyboard::isKeyPressed(Keyboard::S)) menue_x += 1;
			if(menue_x < 0) menue_x = 2;
			if(menue_x > 2) menue_x = 0;
			if(Keyboard::isKeyPressed(Keyboard::Q)){
				if(menue_x == 0){
					load_saving(num_of_level, level, monsters_list, objects_list, num_of_monsters, num_of_objects, p, q, player_1);
					in_main_menue = false;
				}
				if(menue_x == 1){
					load_level(1, level, monsters_list, objects_list, num_of_monsters, num_of_objects, p, q, player_1);
					num_of_level = 1;
					in_main_menue = false;
				}
				if(menue_x == 2) window.close();
				menue_x = 0;
			}
			while(Keyboard::isKeyPressed(Keyboard::W)||Keyboard::isKeyPressed(Keyboard::S)||Keyboard::isKeyPressed(Keyboard::Q)) continue;
		}else if(player_1->dead){
			if(Keyboard::isKeyPressed(Keyboard::W)) menue_x -= 1;
			if(Keyboard::isKeyPressed(Keyboard::S)) menue_x += 1;
			if(menue_x < 0) menue_x = 2;
			if(menue_x > 2) menue_x = 0;
			if(Keyboard::isKeyPressed(Keyboard::Q)){
				if(menue_x == 0){
					load_saving(num_of_level, level, monsters_list, objects_list, num_of_monsters, num_of_objects, p, q, player_1);
					player_1->dead = false;
				}
				if(menue_x == 1){
					load_level(1, level, monsters_list, objects_list, num_of_monsters, num_of_objects, p, q, player_1);
					num_of_level = 1;
					player_1->dead = false;
				}
				if(menue_x == 2){
					player_1->set_hp(max_hp);
					player_1->dead = false;
					in_main_menue = true;
				}
				menue_x = 0;

			}
			while(Keyboard::isKeyPressed(Keyboard::W)||Keyboard::isKeyPressed(Keyboard::S)||Keyboard::isKeyPressed(Keyboard::Q)) continue;
		}
        Event event;
        while (window.pollEvent(event))
        {

			if(Keyboard::isKeyPressed(Keyboard::Escape)) window.close();
			if (event.type == Event::Closed) window.close();
        }
    window.clear(Color(192, 192, 192));
	if(in_main_menue==false){
		int xmaxf = std::min(q, player_1->get_centre_x()/30+n/2+3), ymaxf = std::min(p, player_1->get_centre_y()/30+n/2+3);
        for(int j = std::max(0, player_1->get_centre_x()/30-n/2-1); j < xmaxf; j++){
            for(int i = std::max(0, player_1->get_centre_y()/30-n/2-1); i < ymaxf; i++){
                if(level[i][j] != 0){
                    field.setPosition(30-player_1->get_y()+30*i + (9)*30, 30 - player_1->get_x() + 30*j + (9)*30);
                    field.setTextureRect(IntRect(0, 30*level[i][j]-30, 30, 30));
					window.draw(field);
                }
            }
        }

		for(int i = 0; i < num_of_objects; i++){
            if(objects_list[i]){
                if(abs(player_1->get_x()-objects_list[i]->get_x())+abs(player_1->get_y()-objects_list[i]->get_y()) < 40*30){
                    objects_tex[objects_list[i]->get_type()-1]->setPosition(30-player_1->get_y()+objects_list[i]->get_y()+(10)*30,30-player_1->get_x()+ objects_list[i]->get_x()+ (10)*30);
                    window.draw(*objects_tex[objects_list[i]->get_type()-1]);
                }
            }
		}

		for(int i = 0; i < num_of_monsters; i++){
            if(monsters_list[i]){
                if(abs(player_1->get_x()-monsters_list[i]->get_x())+abs(player_1->get_y()-monsters_list[i]->get_y()) < 40*30){
                    monsters_tex[monsters_list[i]->get_type()-1]->setPosition(30-player_1->get_y()+monsters_list[i]->get_y()+(10)*30,30-player_1->get_x()+ monsters_list[i]->get_x()+ (10)*30);
                    monsters_tex[monsters_list[i]->get_type()-1]->setTextureRect(monsters_list[i]->tex_rect());
					window.draw(*monsters_tex[monsters_list[i]->get_type()-1]);
                }
            }
		}

		
		//std::cout << (player_1->can_atack_dist()) << std::endl;
		if(player_1->can_atack_dist() || n_iter_drawing < 100||dist_atacked){
			if(dist_atacked == true && player_1->can_atack_dist() == false) n_iter_drawing = 80;
			dist_atacked = player_1->can_atack_dist();
			int ind = -1;
			float mi = 10000000;
			for(int i = 0; i < num_of_monsters; i++){
				if(monsters_list[i]){
					float ro = sqrt(pow(monsters_list[i]->get_centre_x() - player_1->get_centre_x(), 2)+ pow(monsters_list[i]->get_centre_y() - player_1->get_centre_y(), 2));
					if(ro<mi){
						mi = ro;
						ind = i;
					}

				}
			}
			if(ind >= 0){

				if(n_iter_drawing < 100){
					n_iter_drawing+=1;
					aim.setFillColor(Color(250, 0, 0));
				}else ind_of_aim = ind;
				if(monsters_list[ind_of_aim]!=NULL){
					aim.setPosition(30-player_1->get_y()+monsters_list[ind_of_aim]->get_centre_y()+(10)*30-5,30-player_1->get_x()+ monsters_list[ind_of_aim]->get_centre_x()+ (10)*30-5);
					window.draw(aim);
				}

				aim.setFillColor(Color(0, 128, 0));
			}
		}
		if (player_1->dead == 0) {
			play.setTextureRect(player_1->tex_rect());
            window.draw(play);
		}
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
			inv_item.setPosition(55+50, 40+40+40+170);
			window.draw(inv_item);
			inv_item.setFillColor(Color(0, 0, 0));
			if(player_1->inner[1]) inv_item.setFillColor(Color(90, 90, 90));
			inv_item.setPosition(55+50, 40+40+40+170+80);
			window.draw(inv_item);
			inv_item.setFillColor(Color(0, 0, 0));
			if(player_1->inner[2]) inv_item.setFillColor(Color(90, 90, 90));
			inv_item.setPosition(55+50, 40+40+40+170+160);
			window.draw(inv_item);
			inv_item.setFillColor(Color(0, 0, 0));
			if(player_1->inner[3]) inv_item.setFillColor(Color(90, 90, 90));
			inv_item.setPosition(55+50, 40+40+40+240+170);
			window.draw(inv_item);
			inv_item.setFillColor(Color(0, 0, 0));
			text.setCharacterSize(15);
			text.setString(std::to_string(player_1->get_gold()));
			text.setPosition(55+40, 45);
			window.draw(text);
			text.setCharacterSize(25);
			if(player_1->inner[0]){
				inv_text_back.setPosition(0, 40+40+40+160);
				window.draw(inv_text_back);
				text.setCharacterSize(15);
				text.setString("close");
				text.setPosition(3, 40+40+40+160);
				window.draw(text);
				text.setString("damage +"+std::to_string(player_1->inner[0]->boost_damage));
				text.setPosition(3, 40+40+40+160+20);
				window.draw(text);
				text.setString("distanse +"+std::to_string(player_1->inner[0]->boost_dist));
				text.setPosition(3, 40+40+40+160+40);
				window.draw(text);
				text.setString("cool down -"+std::to_string(player_1->inner[0]->boost_atack_speed/1000));
				text.setPosition(3, 40+40+40+160+60);
				window.draw(text);
				text.setCharacterSize(25);
			}
			if(player_1->inner[1]){
				inv_text_back.setPosition(0, 40+40+40+170+70);
				window.draw(inv_text_back);
				text.setCharacterSize(15);
				text.setString("armor");
				text.setPosition(3, 40+40+40+170+70);
				window.draw(text);
				text.setString("damage +"+std::to_string(player_1->inner[1]->boost_damage));
				text.setPosition(3, 40+40+40+170+70+20);
				window.draw(text);
				text.setString("armor +"+std::to_string(player_1->inner[1]->boost_armor));
				text.setPosition(3, 40+40+40+170+70+40);
				window.draw(text);
				text.setString("speed "+std::to_string(player_1->inner[1]->boost_speed));
				text.setPosition(3, 40+40+40+170+70+60);
				window.draw(text);
				text.setCharacterSize(25);
			}
			if(player_1->inner[2]){
				inv_text_back.setPosition(0, 40+40+40+170+150);
				window.draw(inv_text_back);
				text.setCharacterSize(15);
				text.setString("distant");
				text.setPosition(3, 40+40+40+170+150);
				window.draw(text);
				text.setString("damage "+std::to_string(player_1->inner[2]->boost_damage));
				text.setPosition(3, 40+40+40+170+150+20);
				window.draw(text);
				text.setString("distanse "+std::to_string(player_1->inner[2]->boost_dist));
				text.setPosition(3, 40+40+40+170+150+40);
				window.draw(text);
				text.setString("cool down "+std::to_string(player_1->inner[2]->boost_atack_speed/1000));
				text.setPosition(3, 40+40+40+170+150+60);
				window.draw(text);
				text.setString(std::to_string(player_1->inner[2]->number));
				text.setPosition(55+90, 40+40+40+170+205);
				window.draw(text);
				text.setCharacterSize(25);
			}
			if(player_1->inner[3]){
				inv_text_back.setPosition(0, 40+40+40+240+160);
				window.draw(inv_text_back);
				text.setCharacterSize(15);
				text.setString("poison");
				text.setPosition(3, 40+40+40+240+160);
				window.draw(text);
				text.setString("health +"+std::to_string(player_1->inner[3]->boost_hp));
				text.setPosition(3, 40+40+40+240+160+40);
				window.draw(text);
				text.setString(std::to_string(player_1->inner[3]->number));
				text.setPosition(55+90, 40+40+40+240+215);
				window.draw(text);
				text.setCharacterSize(25);
			}
			if((inv_y < 3)&&(player_1->inner[inv_x*3+inv_y+4])){
				if(player_1->inner[inv_x*3+inv_y+4]->type == 0){
					inv_text_back.setPosition(55+130+inv_y*70-20, 40+40+inv_x*70-85);
					window.draw(inv_text_back);
					text.setCharacterSize(15);
					text.setString("close");
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85);
					window.draw(text);
					text.setString("damage +"+std::to_string(player_1->inner[inv_x*3+inv_y+4]->boost_damage));
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85+20);
					window.draw(text);
					text.setString("distanse +"+std::to_string(player_1->inner[inv_x*3+inv_y+4]->boost_dist));
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85+40);
					window.draw(text);
					text.setString("cool down -"+std::to_string(player_1->inner[inv_x*3+inv_y+4]->boost_atack_speed/1000));
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85+60);
					window.draw(text);
					text.setCharacterSize(25);
				}
				if(player_1->inner[inv_x*3+inv_y+4]->type == 1){
					inv_text_back.setPosition(55+130+inv_y*70-20, 40+40+inv_x*70-85);
					window.draw(inv_text_back);
					text.setCharacterSize(15);
					text.setString("armor");
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85);
					window.draw(text);
					text.setString("damage +"+std::to_string(player_1->inner[inv_x*3+inv_y+4]->boost_damage));
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85+20);
					window.draw(text);
					text.setString("armor +"+std::to_string(player_1->inner[inv_x*3+inv_y+4]->boost_armor));
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85+40);
					window.draw(text);
					text.setString("speed "+std::to_string(player_1->inner[inv_x*3+inv_y+4]->boost_speed));
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85+60);
					window.draw(text);
					text.setCharacterSize(25);
				}
				if(player_1->inner[inv_x*3+inv_y+4]->type == 2){
					inv_text_back.setPosition(55+130+inv_y*70-20, 40+40+inv_x*70-85);
					window.draw(inv_text_back);
					text.setCharacterSize(15);
					text.setString("distant");
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85);
					window.draw(text);
					text.setString("damage "+std::to_string(player_1->inner[inv_x*3+inv_y+4]->boost_damage));
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85+20);
					window.draw(text);
					text.setString("distanse "+std::to_string(player_1->inner[inv_x*3+inv_y+4]->boost_dist));
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85+40);
					window.draw(text);
					text.setString("cool down "+std::to_string(player_1->inner[inv_x*3+inv_y+4]->boost_atack_speed/1000));
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85+60);
					window.draw(text);
					text.setString(std::to_string(player_1->inner[inv_x*3+inv_y+4]->number));
					text.setPosition(55+130+inv_y*70+40, 40+40+inv_x*70+45);
					window.draw(text);
					text.setCharacterSize(25);
				}
				if(player_1->inner[inv_x*3+inv_y+4]->type == 3){
					inv_text_back.setPosition(55+130+inv_y*70-20, 40+40+inv_x*70-85);
					window.draw(inv_text_back);
					text.setCharacterSize(15);
					text.setString("poison");
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85);
					window.draw(text);
					text.setString("health +"+std::to_string(player_1->inner[inv_x*3+inv_y+4]->boost_hp));
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85+40);
					window.draw(text);
					text.setString(std::to_string(player_1->inner[inv_x*3+inv_y+4]->number));
					text.setPosition(55+130+inv_y*70+40, 40+40+inv_x*70+45);
					window.draw(text);
					text.setCharacterSize(25);
				}

			}
			else if(inv_y>2 && ind >= 0 && objects_list[ind]->inner[inv_x*3+inv_y-3]){
				if(objects_list[ind]->inner[inv_x*3+inv_y-3]->type == 0){
					inv_text_back.setPosition(55+130+inv_y*70-20, 40+40+inv_x*70-85);
					window.draw(inv_text_back);
					text.setCharacterSize(15);
					text.setString("close");
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85);
					window.draw(text);
					text.setString("damage +"+std::to_string(objects_list[ind]->inner[inv_x*3+inv_y-3]->boost_damage));
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85+20);
					window.draw(text);
					text.setString("distanse +"+std::to_string(objects_list[ind]->inner[inv_x*3+inv_y-3]->boost_dist));
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85+40);
					window.draw(text);
					text.setString("cool down -"+std::to_string(objects_list[ind]->inner[inv_x*3+inv_y-3]->boost_atack_speed/1000));
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85+60);
					window.draw(text);
					text.setCharacterSize(25);
				}
				if(objects_list[ind]->inner[inv_x*3+inv_y-3]->type == 1){
					inv_text_back.setPosition(55+130+inv_y*70-20, 40+40+inv_x*70-85);
					window.draw(inv_text_back);
					text.setCharacterSize(15);
					text.setString("armor");
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85);
					window.draw(text);
					text.setString("damage +"+std::to_string(objects_list[ind]->inner[inv_x*3+inv_y-3]->boost_damage));
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85+20);
					window.draw(text);
					text.setString("armor +"+std::to_string(objects_list[ind]->inner[inv_x*3+inv_y-3]->boost_armor));
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85+40);
					window.draw(text);
					text.setString("speed "+std::to_string(objects_list[ind]->inner[inv_x*3+inv_y-3]->boost_speed));
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85+60);
					window.draw(text);
					text.setCharacterSize(25);
				}
				if(objects_list[ind]->inner[inv_x*3+inv_y-3]->type == 2){
					inv_text_back.setPosition(55+130+inv_y*70-20, 40+40+inv_x*70-85);
					window.draw(inv_text_back);
					text.setCharacterSize(15);
					text.setString("distant");
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85);
					window.draw(text);
					text.setString("damage "+std::to_string(objects_list[ind]->inner[inv_x*3+inv_y-3]->boost_damage));
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85+20);
					window.draw(text);
					text.setString("distanse "+std::to_string(objects_list[ind]->inner[inv_x*3+inv_y-3]->boost_dist));
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85+40);
					window.draw(text);
					text.setString("cool down "+std::to_string(objects_list[ind]->inner[inv_x*3+inv_y-3]->boost_atack_speed/1000));
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85+60);
					window.draw(text);
					text.setString(std::to_string(objects_list[ind]->inner[inv_x*3+inv_y-3]->number));
					text.setPosition(55+130+inv_y*70+40, 40+40+inv_x*70+45);
					window.draw(text);
					text.setCharacterSize(25);
				}
				if(objects_list[ind]->inner[inv_x*3+inv_y-3]->type == 3){
					inv_text_back.setPosition(55+130+inv_y*70-20, 40+40+inv_x*70-85);
					window.draw(inv_text_back);
					text.setCharacterSize(15);
					text.setString("poison");
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85);
					window.draw(text);
					text.setString("health +"+std::to_string(objects_list[ind]->inner[inv_x*3+inv_y-3]->boost_hp));
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85+40);
					window.draw(text);
					text.setString(std::to_string(objects_list[ind]->inner[inv_x*3+inv_y-3]->number));
					text.setPosition(55+130+inv_y*70+40, 40+40+inv_x*70+45);
					window.draw(text);
					text.setCharacterSize(25);
				}

			}
		}
		if(player_1->dead){
			window.draw(dead_back);
			button_ch.setPosition(201, 446+50*menue_x);
			window.draw(button_ch);
			button_back.setPosition(205, 450);
			window.draw(button_back);
			button_back.setPosition(205, 500);
			window.draw(button_back);
			button_back.setPosition(205, 550);
			window.draw(button_back);
			text.setString("Load saving");
			text.setPosition(215, 452);
			window.draw(text);
			text.setString("New game");
			text.setPosition(215, 502);
			window.draw(text);
			text.setString("Main menue");
			text.setPosition(215, 552);
			window.draw(text);
		}
	}
	else{
		window.draw(main_back);
		button_ch.setPosition(46, 446+50*menue_x);
		window.draw(button_ch);
		button_back.setPosition(50, 450);
		window.draw(button_back);
		button_back.setPosition(50, 500);
		window.draw(button_back);
		button_back.setPosition(50, 550);
		window.draw(button_back);
		text.setString("Continue");
		text.setPosition(60, 452);
		window.draw(text);
		text.setString("New game");
		text.setPosition(60, 502);
		window.draw(text);
		text.setString("Exit game");
		text.setPosition(60, 552);
		window.draw(text);
	}
	window.display();
	while(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()-start).count() < 25) continue;
    }
	if(level){
		for(int i = 0; i < p+2; i++){
				delete[] level[i];
		}
		delete[] level;
	}
	for(int i = 0; i < num_of_monsters; i++){
		delete monsters_list[i];
	}
	if(monsters_list) delete[] monsters_list;
	for(int i = 0; i < num_of_objects; i++){
		delete objects_list[i];
	}
	if(objects_list)delete[] objects_list;
	delete player_1;
    return 0;
}
