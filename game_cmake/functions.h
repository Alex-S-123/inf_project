// functions.h
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "player.h"
#include "monster.h"
#include "object.h"
#include <string>
#include <fstream>

void load_level(int n_of_level, int**& level, monster**& monsters_list, object**& objects_list, 
               int& num_of_monsters, int& num_of_objects, int& p, int& q, player* pla);
void load_saving(int& nu_lev, int**& level, monster**& monsters_list, object**& objects_list, 
                int& num_of_monsters, int& num_of_objects, int& p, int& q, player* pla);
void creat_saving(int num_of_level, player* pla);

#endif