# SoyCentury game #

### Installing ###

#### Using cmake ####

1. Check that packages g++ and libsfml-dev are installed and cmake is working correctly.
2. Download and unpack **game_cmake.zip** or download **game_cmake** folder
3. Change directory in terminal to **/game/build** folder
4. Run the following commands: cmake .. ; make 
5. Next run file **Game** from folder **/game/build**


#### With no cmake ####

1. Check that packages g++ and libsfml-dev are installed.
2. Download and unpack **game_cmake.zip** or download **game_cmake** folder
3. Change directory in terminal to **game** folder
4. Run the following command: g++ -c main.cpp item.cpp object.cpp functions.cpp monster.cpp player.cpp && g++ functions.o item.o main.o monster.o object.o player.o -lsfml-graphics -lsfml-window -lsfml-system -o Game
5. Next run file **Game** from folder **game**



### Control ###

* To navigate in menues, use **W** and **S** buttons to move and **Q** button to select
* In game use **W** **A** **S** **D** buttons to move, **Q** button to fight, **F** button to open inventar
* In inventar use **W** **A** **S** **D** buttons to navigate, **E** button to equip selected item
* If you open inventar near chest, use **Q** button to swap selected item between chest and player
* If you equiped poisons, press **R** to use it
* If you have distant weapon and see aim on the monster, press **E** to attack this monster
* In any moment you can use **ESC** button to quit game
