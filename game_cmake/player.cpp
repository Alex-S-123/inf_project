//player.cpp

#include "player.h"

using namespace sf;

player::player(int sx, int sy, int tx0, int ty0, int health) : object(sx, sy, tx0, ty0){
    x = 0;
    y = 0;
    speed = 1;
    damage = 1;
    armor = 0;
    gold = 0;
    dist_atack = 60;
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
    max_health = hp;
    for(int i = 0; i < 28; i++) inner[i] = NULL;
}
int player::get_hp() {
    return this->hp;
}
bool player::get_attack() {
    return attack;
}
int player::get_damage() {
    return this->damage;
}

bool player::can_atack_dist(){
    if(inner[2]==NULL) return 0;
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - this->last_dist_hit).count() >= inner[2]->boost_atack_speed;
}

void player::set_items(int n){
    num_of_items = n;
}

bool player::give(item* it){
    if(num_of_items == 24||it==NULL) return 0;
    if(it->type == 4){
        gold += it->cost;
        return 1;
    }
    inner[num_of_items+4] = it;
    num_of_items++;
    return 1;
}
item* player::take(int i){
    if(i >= num_of_items) return NULL;
    item* res = inner[i+4];
    for(int j = i+4; j < num_of_items+3; j++) inner[j] = inner[j+1];
    inner[num_of_items+3] = NULL;
    num_of_items--;
    return res;
}

void player::damaged(int damag) {
    if (this->armor < damag) {
        this->hp -= (damag - this->armor);
        if (this->hp <= 0) {
            dead = 1;
        }
    }
}
void player::set_hp(int he){
    hp = he;
}
int player::get_gold(){
    return gold;
}

void player::action(int** level, int p, int q, monster** monsters_list, int n, object** objects_list, int no){
    speed = 2;
    damage = 1;
    armor = 0;
    time_atack = 3000;
    dist_atack = 60;
    int sind = level[this->get_centre_y()/30+1][(x+xsize)/30+1];
    if(sind == 3||sind == 6) speed++;
    if(sind == 4) speed--;
    for(int i = 0; i<2; i++){
        if(inner[i]){
            speed += inner[i]->boost_speed;
            damage += inner[i]->boost_damage;
            time_atack -= inner[i]->boost_atack_speed;
            armor += inner[i]->boost_armor;
            dist_atack += inner[i]->boost_dist;
        }
    }
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
                if(monsters_list[i]&&monsters_list[i]->get_type()!=3){
                    float ro = sqrt(pow(monsters_list[i]->get_centre_x() - this->get_centre_x(), 2)+ pow(monsters_list[i]->get_centre_y() - this->get_centre_y(), 2));
                    if(ro<mi){
                        mi = ro;
                        ind = i;
                    }

                }
            }
            auto time_dist_delta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - this->last_dist_hit).count();
            if(ind >= 0 && mi<= inner[2]->boost_dist && time_dist_delta >= inner[2]->boost_atack_speed){
                if (monsters_list[ind]->get_hp() - inner[2]->boost_damage > 0) {
                    monsters_list[ind]->set_hp(-1 * inner[2]->boost_damage);
                } else {
                    monsters_list[ind] = NULL;
                }
                last_dist_hit = std::chrono::system_clock::now();
                inner[2]->number -= 1;
                if(inner[2]->number == 0) inner[2] = NULL;
            }
        }
    }
    if(Keyboard::isKeyPressed(Keyboard::R)){
        if(inner[3]){
            hp = std::min(max_health, hp+inner[3]->boost_hp);
            if(inner[2]) inner[2]->number += inner[3]->boost_speed;
            inner[3]->number -= 1;
            if(inner[3]->number == 0) inner[3] = NULL;
            while(Keyboard::isKeyPressed(Keyboard::R)) continue;
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
                        this->last_hit = std::chrono::system_clock::now();
                    } else {
                        if(monsters_list[i] ->get_type() != 3)monsters_list[i] = NULL;
                    }
                }
            }
        }
    }


    //calculating of texture rect
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