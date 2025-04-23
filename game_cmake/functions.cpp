//functions.cpp

#include "functions.h"

void load_level(int n_of_level, int**& level, monster**& monsters_list, object**& objects_list, int& num_of_monsters, int& num_of_objects, int& p, int& q, player* pla){
	//loading level from file 
	if(level){
		for(int i = 0; i < p+2; i++){
				if(level[i])delete[] level[i];
		}
		delete[] level;
	}
	for(int i = 0; i < num_of_monsters; i++){
		if(monsters_list[i]) delete monsters_list[i];
	}
	if(monsters_list) delete[] monsters_list;
	for(int i = 0; i < num_of_objects; i++){
		if(objects_list[i]) delete objects_list[i];
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
	monsters_list = new monster*[2*num_of_monsters];
	for(int i = 0; i < num_of_monsters; i++){
		int type, x0, y0;
		fm >> type;
		if(type == 1){
			int rad, dir;
			fm >> x0 >> y0 >> rad >> dir;
			monsters_list[i] = new soyjak_typical(x0, y0, rad, dir);
		}
		else if(type == 2){
			bomb* b = new bomb();
			num_of_monsters++;
			int rad;
			fm >> x0 >> y0 >> rad;
			monsters_list[i] = new flight(x0, y0, rad, b);
			i++;
			monsters_list[i] = b;
		}
		else if(type == 4){
			int x1, y1;
			fm >>x0 >> y0 >> x1 >> y1;
			monsters_list[i] = new ball(x0, y0, x1, y1);
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
			int n_items;
			int a, b, c, d, e, f, g, h, k, l, o, t;
			objects_list[i] = new treasure(x0, y0);
			fo >> n_items;
			for(int j = 0; j < n_items; j++){
				fo >> t >> a >> b >> c >> d >> e >> f >> g >> h;
				objects_list[i]->give(new item(t, a, b, c, d, e, f, g, h));
			}
		}
		if(type == 2){
			int dest;
			fo>>dest;
			objects_list[i] = new teleport(x0, y0, dest);
		}
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
		int counter = 0;
		for(int i = 0; i < 28; i++){
			int type;
			fs >> type;
			if(type >= 0){
				if(i > 3) counter++;
				int a, b, c, d, e, f, g, h, k, l, o;
				fs >> a >> b >> c >> d >> e >> f >> g >> h;
				pla->inner[i] = new item(type, a, b, c, d, e, f, g, h);
			}
			else{
				pla->inner[i] = NULL;
			}
		}
		pla->set_items(counter);
	}
	else{
		// if save.txt is not correct or does not exist, start level 1
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
			fns << pla->inner[i]->type << " " << pla->inner[i]->boost_damage << " " << pla->inner[i]->boost_armor 
			<< " " << pla->inner[i]->boost_hp << " " << pla->inner[i]->boost_speed << " " 
			<< pla->inner[i]->boost_atack_speed << " " << pla->inner[i]->boost_dist 
			<< " " << pla->inner[i]->cost << " " << pla->inner[i]->number << '\n';
		}
		else{
			fns << -1 << '\n';
		}
	}
	fns.close();
}