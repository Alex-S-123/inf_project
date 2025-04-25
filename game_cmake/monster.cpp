//monster.cpp

#include "monster.h"

monster::monster():object(0,0,0,0){
    fly = false;
};
bool monster::is_flying(){
    return fly;
}
int monster::get_type(){return this->type;}
int monster::get_hp() {
    return this->hp;
}
void monster::set_hp(int delta_hp) {
    this->hp += delta_hp;
}
void monster::behavior(player *p){};

soyjak_typical::soyjak_typical(int xp, int yp, int rad, bool x_dir){
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


void soyjak_typical::behavior(player *p) {
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
            this->damaged_player = true;
            p->damaged(damage);
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


bomb::bomb() {
    this->type = 3;
    this->x = 0; 
    this->y = 0;
    this->speed = 3;
    this->distance = 0; // distance - between start point and current point
    this->limit = 50; // distance where bomb explodes
    this->hp = 1;
    this->attack_radius = 60;
    x0 = 0;
    x1 = 37;
    damage = 4;
    frames_after_atack = 100;
    collision = false;
}

void bomb::start_attack(int x0, int y0){
    this->set_pos(x0, y0);
    distance = 0;
    is_attacking = 1;
    hp = 1;
}

void bomb::behavior(player *p) {
    if(frames_after_atack < 20) frames_after_atack++;
    if(hp <= 0){
        is_attacking = false;
    }
    if(is_attacking){
        this->x += speed;
        this->distance += speed;
        if (this->distance >= this->limit) {
            if ((pow(abs(this->get_centre_x() - p->get_centre_x()), 2) + pow(abs(this->get_centre_y() - p->get_centre_y()), 2) <= pow(this->attack_radius, 2))
                ) {
                p->damaged(damage);
            }
            is_attacking = false;
            frames_after_atack = 0;
        }
    }
    if(is_attacking){
        y0 = 0;
        y1 = 26;
    }
    else if(frames_after_atack < 20){
        y0 = 26;
        y1 = 52;
    }
    else{
        y0 = 52;
        y1 = 78;
    }
    // if bomb contacts ground, it is destroyed! 
}


flight::flight(int xp, int yp, int rad, bomb* b) {
    this->type = 2;
    this->period_bomb = rand() % (7000 - 3000 + 1) + 3000;
    this->xp = xp;
    this->yp = yp;
    this->set_pos(xp, yp);
    this->fly_radius = rad;
    this->speed = 2;
    //this->type = 1;
    this->up_direction = true;
    this->hp = 3;
    last_hit = std::chrono::system_clock::now();
    this->b_attack = b;
    collision = false;
    frames_of_walk = 0;
}
void flight::behavior(player *p) {
    // flying (now exists only on x-axis)
    if (this->up_direction) {
        if (this->y <= this->yp + this->fly_radius) {
            this->y += this->speed;
        } else {
            this->up_direction = false;
        }
    } else {
        if (this->y >= this->yp - fly_radius) {
            this->y -= this->speed;
        } else {
            this->up_direction = true;
        }
    }
    frames_of_walk = (frames_of_walk+1)%30;
    y0 = (frames_of_walk/10)*50;
    y1 = y0 + 50;
    if(up_direction){
        x0 = 0;
        x1 = 40;
    }
    else{
        x1 = 0;
        x0 = 40;
    }
    // throwing bomb
    int counted = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - this->last_hit).count();
    if (counted >= this->period_bomb) {
        b_attack->start_attack(this->get_centre_x(), this->get_centre_y());
        this->last_hit = std::chrono::system_clock::now();
    }
}


ball::ball(int xl, int yl, int xr, int yr){
	this->hp = 10;
	this->armor = 1;
	this->damage = 1;
	this->x = xl+5;
	this->y = yl;
	this->type = 4;
	this->xl = xl;
	this->xr = xr;
	this->yl = yl;
	this->yr = yr;
	y1 = 25;
	x1 = 25;
	collision = true;
	this->damaged_player = false;
	frames_of_walk = 0;
}
void ball::behavior(player* pla){
	if(x <= xl || x>=xr || y<=yl || y>=yr){ //random speed after collision with walls
		xspeed = rand() % (3 - 0 + 1) + 0;
		yspeed = rand() % (3 - 0 + 1) + 0;
		if(x>=xr){
			xspeed *= -1;
		}
		if(y>=yr){
			yspeed *= -1;
		}
	}
	frames_of_walk = (frames_of_walk + abs(xspeed) + abs(yspeed))%60;
	x += xspeed;
	y += yspeed;
	if(pow(abs(this->get_centre_x() - pla->get_centre_x()), 2) + pow(abs(this->get_centre_y() - pla->get_centre_y()), 2) <= pow(45, 2)){
		if(!damaged_player){
			damaged_player = true;
			pla->damaged(damage);
		}
	}
	else damaged_player = false;

	y0 = frames_of_walk/15*25;
	y1 = y0 + 25;
	if(xspeed >= 0){
		x0 = 0;
		x1 = 25;
	}
	else{
		x1 = 0;
		x0 = 25;
	}
}