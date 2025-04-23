// main.cpp
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <chrono>
#include <cmath>
#include <string>
#include "item.h"
#include "object.h"
#include "monster.h"
#include "player.h"
#include "functions.h"

using namespace sf;

int main(){
	bool inv_open = false;
	bool is_winner = false;
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

	//creating all shapes
	Font font;
	font.loadFromFile("textures/arial.ttf");
	Text text;
	text.setFont(font);
	text.setFillColor(Color(250, 250, 250));
	text.setCharacterSize(25);


	player* player_1 = new player(0,0,36,20,max_hp);

	bool dist_atacked = false;
	int n_iter_drawing = 1000, ind_of_aim = 0;
    int n = 20; //number of plates we see
    int monster_types = 4;
    RectangleShape** monsters_tex = new RectangleShape*[monster_types]; //number of monster types
	monsters_tex[0] = new RectangleShape(Vector2f(28, 46));
	monsters_tex[1] = new RectangleShape(Vector2f(40, 50));
	monsters_tex[2] = new RectangleShape(Vector2f(37, 26));
	monsters_tex[3] = new RectangleShape(Vector2f(25, 25));

	int object_types = 2;
    RectangleShape** objects_tex = new RectangleShape*[object_types];
	objects_tex[0] = new RectangleShape(Vector2f(25, 25));
	objects_tex[1] = new RectangleShape(Vector2f(40, 60));

	

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
	RectangleShape win_back(Vector2f(30*(n+2), 30*(n+2)));
	RectangleShape aim(Vector2f(40, 40));
	RectangleShape item_dr(Vector2f(60, 60));
	RectangleShape background(Vector2f(30*(n+2), 30*(n+2)));

	
	RectangleShape dead_back(Vector2f(30*(n+2), 30*(n+2)));
	RectangleShape button_back(Vector2f(250, 36));
	button_back.setFillColor(Color(140, 70, 20));
	RectangleShape button_ch(Vector2f(258, 44));
	button_ch.setFillColor(Color(250, 250, 250));
	RectangleShape hp_bar(Vector2f(280, 40));
	hp_bar.setFillColor(Color(200, 0,0));
	RectangleShape hp_bar_back(Vector2f(288, 23));
	hp_bar_back.setPosition(350-140-4, 633-2);
	hp_bar_back.setFillColor(Color(0, 0, 0, 175));

    


	
	//adding textures
	Texture back_tex;
	back_tex.loadFromFile("textures/back.png");
	background.setTexture(&back_tex);


	Texture dead_tex;
	dead_tex.loadFromFile("textures/wasted.png");
	dead_back.setTexture(&dead_tex);

	Texture win_tex;
	win_tex.loadFromFile("textures/victory.png");
	win_back.setTexture(&win_tex);

	Texture item_tex;
	item_tex.loadFromFile("textures/weapons.png");
	item_dr.setTexture(&item_tex);

	Texture aim_tex;
	aim_tex.loadFromFile("textures/aim.png");
	aim.setTexture(&aim_tex);


	Texture ma_menue;
	ma_menue.loadFromFile("textures/menue.png");
	main_back.setTexture(&ma_menue);


	Texture field_tex;
	field_tex.loadFromFile("textures/terrains.png");
	field.setTexture(&field_tex);

	Texture player_tex;
	player_tex.loadFromFile("textures/player.png");
	play.setTexture(&player_tex);

	Texture** obj_tex = new Texture*[object_types];
	obj_tex[1] = new Texture;
	obj_tex[1]->loadFromFile("textures/portal.png");
	obj_tex[0] = new Texture;
	obj_tex[0]->loadFromFile("textures/chest.png");
	for(int i = 0; i < object_types; i++) objects_tex[i]->setTexture(obj_tex[i]);
	

	Texture** mons_te = new Texture*[monster_types];
	mons_te[0] = new Texture;
	mons_te[0]->loadFromFile("textures/soyjak.png");
	mons_te[1] = new Texture;
	mons_te[1]->loadFromFile("textures/flight.png");
	mons_te[2] = new Texture;
	mons_te[2]->loadFromFile("textures/bomb.png");
	mons_te[3] = new Texture;
	mons_te[3]->loadFromFile("textures/kolobok.png");
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
		if(in_main_menue == false&&player_1->dead==false&&is_winner == false){
			if(Keyboard::isKeyPressed(Keyboard::F)){
				//  open/close inventar 
				if(inv_open) inv_open = false;
				else inv_open = true;
				while (Keyboard::isKeyPressed(Keyboard::F)) continue; 
				inv_x = 0;
				inv_y = 0;
			}
			for(int i = 0; i < num_of_monsters; i++){
					if(monsters_list[i])monsters_list[i]->behavior(player_1); // actions of monsters
				}
			if((!inv_open) && (player_1->dead == 0)){
				player_1->action(level, p, q, monsters_list, num_of_monsters, objects_list, num_of_objects); //actions of player
				if(Keyboard::isKeyPressed(Keyboard::T)){
					// teleportation if player is near portal
					int ind = -1;
					for(int i = 0; i < num_of_objects; i++){
						if(abs(player_1->get_centre_x()-objects_list[i]->get_centre_x())+abs(player_1->get_centre_y()-objects_list[i]->get_centre_y())<40&&objects_list[i]->get_type() == 2) ind = i;
					}
					if(ind>=0){
						num_of_level = objects_list[ind]->destination();
						if(num_of_level == -1) is_winner = true;
						else{
							load_level(num_of_level, level, monsters_list, objects_list, num_of_monsters, num_of_objects, p, q, player_1);
							creat_saving(num_of_level, player_1);
						}
					}
				}
			}
			else{
				//actions in inventar
				if(Keyboard::isKeyPressed(Keyboard::W)) inv_x -= 1;
				if(Keyboard::isKeyPressed(Keyboard::S)) inv_x += 1;
				if(Keyboard::isKeyPressed(Keyboard::A)) inv_y -= 1;
				if(Keyboard::isKeyPressed(Keyboard::D)) inv_y += 1;
				if(Keyboard::isKeyPressed(Keyboard::Q)){
					//switch item between inventar and treasure, if near
					int ind = -1;
					for(int i = 0; i < num_of_objects; i++){
						if(abs(player_1->get_centre_x()-objects_list[i]->get_centre_x())+abs(player_1->get_centre_y()-objects_list[i]->get_centre_y())<40&&objects_list[i]->get_type() == 1) ind = i;
					}
					if(ind>=0){
						object* tr = objects_list[ind];
						if(inv_y < 3)tr->give(player_1->take(inv_x*3+inv_y));
						else player_1->give(tr->take(inv_x*3+inv_y-3));
					}
				}
				if(Keyboard::isKeyPressed(Keyboard::E)){
					//equping item
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
			//navigation in main menue
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
					player_1->dead = false;
					player_1->set_hp(max_hp);
					creat_saving(1, player_1);
					num_of_level = 1;
					in_main_menue = false;
				}
				if(menue_x == 2) window.close();
				menue_x = 0;
			}
			while(Keyboard::isKeyPressed(Keyboard::W)||Keyboard::isKeyPressed(Keyboard::S)||Keyboard::isKeyPressed(Keyboard::Q)) continue;
		}else if(player_1->dead){
			//navigation in death menue
			if(Keyboard::isKeyPressed(Keyboard::W)) menue_x -= 1;
			if(Keyboard::isKeyPressed(Keyboard::S)) menue_x += 1;
			if(menue_x < 0) menue_x = 2;
			if(menue_x > 2) menue_x = 0;
			if(Keyboard::isKeyPressed(Keyboard::Q)){
				for(int j = 0; j < 28; j++){
					player_1->inner[j] = NULL;
				}
				if(menue_x == 0){
					load_saving(num_of_level, level, monsters_list, objects_list, num_of_monsters, num_of_objects, p, q, player_1);
					player_1->dead = false;
				}
				if(menue_x == 1){
					load_level(1, level, monsters_list, objects_list, num_of_monsters, num_of_objects, p, q, player_1);
					player_1->dead = false;
					player_1->set_hp(max_hp);
					creat_saving(1, player_1);
					num_of_level = 1;
					
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
		else if(is_winner){
			//navigation in win menue
			if(Keyboard::isKeyPressed(Keyboard::W)) menue_x -= 1;
			if(Keyboard::isKeyPressed(Keyboard::S)) menue_x += 1;
			if(menue_x < 0) menue_x = 1;
			if(menue_x > 1) menue_x = 0;
			if(Keyboard::isKeyPressed(Keyboard::Q)){
				for(int j = 0; j < 28; j++){
					player_1->inner[j] = NULL;
				}
				if(menue_x == 0){
					load_level(1, level, monsters_list, objects_list, num_of_monsters, num_of_objects, p, q, player_1);
					num_of_level = 1;
					player_1->set_hp(max_hp);
					is_winner = false;
				}
				if(menue_x == 1){
					player_1->set_hp(max_hp);
					player_1->dead = false;
					in_main_menue = true;
					is_winner = false;
				}
				menue_x = 0;
			}
			while(Keyboard::isKeyPressed(Keyboard::W)||Keyboard::isKeyPressed(Keyboard::S)||Keyboard::isKeyPressed(Keyboard::Q)) continue;
		}
        Event event;
        while (window.pollEvent(event))
        {
			//closing window if "ESC" is pressed
			if(Keyboard::isKeyPressed(Keyboard::Escape)) window.close();
			if (event.type == Event::Closed) window.close();
        }
    window.clear(Color(192, 192, 192));
	window.draw(background);
	if(in_main_menue==false&&is_winner==false){
		//drawing field
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
		//drawing objects

		for(int i = 0; i < num_of_objects; i++){
            if(objects_list[i]){
                if(abs(player_1->get_x()-objects_list[i]->get_x())+abs(player_1->get_y()-objects_list[i]->get_y()) < 40*30){
                    objects_tex[objects_list[i]->get_type()-1]->setPosition(30-player_1->get_y()+objects_list[i]->get_y()+(10)*30,30-player_1->get_x()+ objects_list[i]->get_x()+ (10)*30);
                    window.draw(*objects_tex[objects_list[i]->get_type()-1]);
                }
            }
		}
		//drawing monsters
		for(int i = 0; i < num_of_monsters; i++){
            if(monsters_list[i]){
                if(abs(player_1->get_x()-monsters_list[i]->get_x())+abs(player_1->get_y()-monsters_list[i]->get_y()) < 40*30){
                    monsters_tex[monsters_list[i]->get_type()-1]->setPosition(30-player_1->get_y()+monsters_list[i]->get_y()+(10)*30,30-player_1->get_x()+ monsters_list[i]->get_x()+ (10)*30);
                    monsters_tex[monsters_list[i]->get_type()-1]->setTextureRect(monsters_list[i]->tex_rect());
					window.draw(*monsters_tex[monsters_list[i]->get_type()-1]);
                }
            }
		}

		
		//drawing aim if player can atack dist
		if(player_1->can_atack_dist() || n_iter_drawing < 100||dist_atacked){
			if(dist_atacked == true && player_1->can_atack_dist() == false) n_iter_drawing = 80;
			dist_atacked = player_1->can_atack_dist();
			int ind = -1;
			float mi = 10000000;
			for(int i = 0; i < num_of_monsters; i++){
				if(monsters_list[i]&&monsters_list[i]->get_type()!=3){
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
					aim.setPosition(30-player_1->get_y()+monsters_list[ind_of_aim]->get_centre_y()+(10)*30-20,30-player_1->get_x()+ monsters_list[ind_of_aim]->get_centre_x()+ (10)*30-20);
					window.draw(aim);
				}

				aim.setFillColor(Color(0, 250, 0));
			}
		}
		if (player_1->dead == 0) {
			//drawing player and health bar
			play.setTextureRect(player_1->tex_rect());
            window.draw(play);
			float hp = player_1->get_hp();
			hp_bar.setPosition(350-140.f*hp/max_hp, 635);
			hp_bar.setSize(Vector2f(280*hp/max_hp, 15.f));
			window.draw(hp_bar_back);
			window.draw(hp_bar);
		}
		if(inv_open){
			//drawing inventar
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
				window.draw(inv_item);
				item* dr = NULL;
				if(i%6 < 3){
					dr = player_1->inner[(i/6)*3+i%6+4];
				}
				else if(ind >= 0 && i%6 >= 3){
					dr = objects_list[ind]->inner[(i/6)*3+i%6-3];
				}
				if(dr){
					item_dr.setPosition(55+130+(i%6)*70, 40+40+(i/6)*70);
					if(dr->type == 0){
						item_dr.setTextureRect(IntRect(0, 59*dr->boost_damage-59, 60, 59));
						window.draw(item_dr);
					}
					else if(dr->type == 1){
						item_dr.setTextureRect(IntRect(0, 59*4+59*dr->boost_armor-59, 60, 59));
						window.draw(item_dr);
					}
					else if(dr->type == 2){
						item_dr.setTextureRect(IntRect(0, 59*7+59*dr->boost_damage-59, 60, 59));
						window.draw(item_dr);
						text.setCharacterSize(15);
						text.setString(std::to_string(dr->number));
						text.setPosition(55+130+(i%6)*70+40, 40+40+(i/6)*70+45);
						window.draw(text);
						text.setCharacterSize(25);
					}
					else if(dr->type == 3){
						if(dr->boost_hp > 0)item_dr.setTextureRect(IntRect(0, 709, 60, 59));
						if(dr->boost_speed > 0)item_dr.setTextureRect(IntRect(0, 709+59, 60, 59));
						window.draw(item_dr);
						text.setCharacterSize(15);
						text.setString(std::to_string(dr->number));
						text.setPosition(55+130+(i%6)*70+40, 40+40+(i/6)*70+45);
						window.draw(text);
						text.setCharacterSize(25);
					}
					else if(dr->type == 4){
						item_dr.setTextureRect(IntRect(0, 59*11, 60, 59));
						window.draw(item_dr);
						text.setCharacterSize(15);
						text.setString(std::to_string(dr->cost));
						text.setPosition(55+130+(i%6)*70+40, 40+40+(i/6)*70+45);
						window.draw(text);
						text.setCharacterSize(25);
					}
				}
				
			}
			inv_item.setPosition(55+50, 40+40+40+170);
			window.draw(inv_item);
			if(player_1->inner[0]){
				item_dr.setPosition(55+50, 40+40+40+170);
				item_dr.setTextureRect(IntRect(0, 59*player_1->inner[0]->boost_damage-59, 60, 59));
				window.draw(item_dr);
			}
			inv_item.setPosition(55+50, 40+40+40+170+80);
			window.draw(inv_item);
			if(player_1->inner[1]) {
				item_dr.setPosition(55+50, 40+40+40+170+80);
				item_dr.setTextureRect(IntRect(0, 59*4+59*player_1->inner[1]->boost_armor-59, 60, 59));
				window.draw(item_dr);
			}
			inv_item.setPosition(55+50, 40+40+40+170+160);
			window.draw(inv_item);
			if(player_1->inner[2]){
				item_dr.setPosition(55+50, 40+40+40+170+160);
				item_dr.setTextureRect(IntRect(0, 59*7+59*player_1->inner[2]->boost_damage-59, 60, 59));
				window.draw(item_dr);
			}
			inv_item.setPosition(55+50, 40+40+40+240+170);
			window.draw(inv_item);
			if(player_1->inner[3]){
				item_dr.setPosition(55+50, 40+40+40+240+170);
				if(player_1->inner[3]->boost_hp > 0)item_dr.setTextureRect(IntRect(0, 709, 60, 59));
				if(player_1->inner[3]->boost_speed > 0)item_dr.setTextureRect(IntRect(0, 709+59, 60, 59));
				window.draw(item_dr);
			}
			
			text.setCharacterSize(15);
			text.setString("gold: "+std::to_string(player_1->get_gold()));
			text.setPosition(55+5, 45);
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
				text.setPosition(3, 40+40+40+240+160+30);
				window.draw(text);
				text.setString("ammunition +"+std::to_string(player_1->inner[3]->boost_speed));
				text.setPosition(3, 40+40+40+240+160+50);
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
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85+30);
					window.draw(text);
					text.setString("ammunition +"+std::to_string(player_1->inner[inv_x*3+inv_y+4]->boost_speed));
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85+50);
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
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85+30);
					window.draw(text);
					text.setString("ammunition +"+std::to_string(objects_list[ind]->inner[inv_x*3+inv_y-3]->boost_speed));
					text.setPosition(58+130+inv_y*70-20, 40+40+inv_x*70-85+50);
					window.draw(text);
					text.setCharacterSize(25);
				}

			}
		}
		if(player_1->dead){
			//drawing death menue
			window.draw(dead_back);
			button_ch.setPosition(201, 446+50*menue_x);
			button_ch.setFillColor(Color(200, 0, 0));
			window.draw(button_ch);
			button_ch.setFillColor(Color(250, 250, 250));
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
	else if(in_main_menue){
		//drawing main menue
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
	else if(is_winner){
		//drawing win menue
		window.draw(win_back);
		button_ch.setPosition(361, 50+446+50*menue_x);
		button_ch.setFillColor(Color(255, 220, 51));
		window.draw(button_ch);
		button_ch.setFillColor(Color(250, 250, 250));
		button_back.setPosition(365, 500);
		window.draw(button_back);
		button_back.setPosition(365, 550);
		window.draw(button_back);
		text.setString("New game");
		text.setPosition(375, 502);
		window.draw(text);
		text.setString("Main menue");
		text.setPosition(375, 552);
		window.draw(text);
	}
	window.display();
	while(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()-start).count() < 25) continue; //fixed fps
    }
	//deleting arrays after closing game
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
	for(int i = 0; i < monster_types; i++){
		delete monsters_tex[i];
		delete mons_te[i];
	}
	for(int i = 0; i < object_types; i++){
		delete objects_tex[i];
		delete obj_tex[i];
	}
    return 0;
}
