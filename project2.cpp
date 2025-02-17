#include <SFML/Graphics.hpp>
#include <ctime>

using namespace sf;

int n = 16;
int m = 30;
int mines = 99;
int to_win = m*n-mines;
bool start = true;
int flag = 1;
int flags = mines;
int star = 0;
int tim = 0;

void init(int**field, int x, int y){
	int coun = 0;
	int seed = time(0);
    srand(seed);
    flags = mines;
    to_win = m*n-mines;
	while(coun < mines){
		int xm = rand()%n;
		int ym = rand()%m;
		if(abs(x-xm) + abs(y-ym) > 3 && field[xm][ym] != -1){
			field[xm][ym] = -1;

			if(xm != 0 && field[xm-1][ym] != -1) field[xm-1][ym]++;
			if(xm != n-1 && field[xm+1][ym] != -1) field[xm+1][ym]++;
			if(ym != 0 && field[xm][ym-1] != -1) field[xm][ym-1]++;
			if(ym != m-1 && field[xm][ym+1] != -1) field[xm][ym+1]++;

			if(xm != 0 && ym != 0 && field[xm-1][ym-1] != -1) field[xm-1][ym-1]++;
			if(xm != n-1 && ym != 0 && field[xm+1][ym-1] != -1) field[xm+1][ym-1]++;
			if(xm != n-1 && ym != m-1 && field[xm+1][ym+1] != -1) field[xm+1][ym+1]++;
			if(xm != 0 && ym != m-1 && field[xm-1][ym+1] != -1) field[xm-1][ym+1]++;

			coun++;
		}
	}
	star = time(0);
	tim = 0;


}
bool openf(int**field, int** opened, int xm, int ym){
	if(xm >= n || xm < 0 || ym >= m || ym < 0) return 0;
    if (opened[xm][ym] == 0){
        opened[xm][ym] = 1;
        to_win -= 1;
    }

	if(field[xm][ym] == 0){
		if(xm != 0 && opened[xm-1][ym] == 0) openf(field, opened, xm-1, ym);
		if(xm != n-1 && opened[xm+1][ym] == 0) openf(field, opened, xm+1, ym);
		if(ym != 0 && opened[xm][ym-1] == 0) openf(field, opened, xm, ym-1);
		if(ym != m-1 && opened[xm][ym+1] == 0) openf(field, opened, xm, ym+1);

		if(xm != 0 && ym != 0 && opened[xm-1][ym-1] == 0) openf(field, opened, xm-1, ym-1);
		if(xm != n-1 && ym != 0 && opened[xm+1][ym-1] == 0) openf(field, opened, xm+1, ym-1);
		if(xm != n-1 && ym != m-1 && opened[xm+1][ym+1] == 0) openf(field, opened, xm+1, ym+1);
		if(xm != 0 && ym != m-1 && opened[xm-1][ym+1] == 0) openf(field, opened, xm-1, ym+1);

	}
	return 1;


}


int main()
{

	Texture tex;
	tex.loadFromFile("ButtonTypes.bmp");
	Texture num;
	num.loadFromFile("Numbers_new.bmp");
	Texture text;
	text.loadFromFile("text.bmp");

    srand(time(NULL));
	VideoMode vid;
	vid.width = 25*(m+2);
	vid.height = 25*(n+4);
    RenderWindow window(vid, L"Minesweeper", Style::Default);
	RectangleShape field1(Vector2f(25, 25));
	field1.setTexture(&tex);
	field1.setTextureRect(IntRect(0, 0, 25, 25));

	RectangleShape number(Vector2f(20, 35.5));
	number.setTexture(&num);
	number.setTextureRect(IntRect(0, 0, 20, 35.4));

	RectangleShape textbox1(Vector2f(115, 28));
	textbox1.setTexture(&text);

	RectangleShape textbox2(Vector2f(235, 28));
	textbox2.setTexture(&text);



    int** field = new int*[n];
	for(int j = 0; j < n; j++){
		int* rad = new int[m];
		for(int i = 0; i < m; i++){
			rad[i] = 0;
		}
		field[j] = rad;
	}

	int** opened = new int*[n];
	for(int j = 0; j < n; j++){
		int* radb = new int[m];
		for(int i = 0; i < m; i++){
			radb[i] = 0;
		}
		opened[j] = radb;
	}

       while (window.isOpen())
    {

        Event event;
        while (window.pollEvent(event))
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			Vector2i pos = sf::Mouse::getPosition(window);
			int y = pos.x;
			int x = pos.y;
			if(flag != 1){
                if (x < 25*(n+3) && y < 25*(m+1) && x > 75 && y > 25){
                    if (opened[(x-75)/25][(y-25) / 25] == 0){
                        if(start){
                            init(field, (x-75)/25, (y-25) / 25);
                        }
                        openf(field, opened, (x-75)/25, (y-25) / 25);
                        start = false;
                    }
                    if (opened[(x-75)/25][(y-25) / 25] == 1 && field[(x-75)/25][(y-25) / 25] == -1){
                        field[(x-75)/25][(y-25) / 25] = -2;
                        flag = -1;
                        for(int j = 0; j < n; j++){
                            for(int i = 0; i < m; i++){
                                if(opened[j][i] == -1 && field[j][i] >= 0) field[j][i] = -3;
                                opened[j][i] = 1;
                            }
                        }

                    }
                }
                if(flag == -1) flag = 10;
		else if (flag >= 10 || flag == -2) flag = 1;
			}else{
			    if (x > (25*(n+4)-94)/2 && y > (25*(m+2)-235)/2 && x < (25*(n+4)-94)/2 + 28 && y < (25*(m+2)-235)/2 + 235){
                    n = 9;
                    m = 9;
                    mines = 10;

			    } else if (x > (25*(n+4)-94)/2 + 33 && y > (25*(m+2)-235)/2 && x < (25*(n+4)-94)/2 + 33 + 28 && y < (25*(m+2)-235)/2 + 235){
                    n = 16;
                    m = 16;
                    mines = 40;
			    } else if (x > (25*(n+4)-94)/2 + 66 && y > (25*(m+2)-235)/2 && x < (25*(n+4)-94)/2 + 66 + 28 && y < (25*(m+2)-235)/2 + 235){
                    n = 16;
                    m = 30;
                    mines = 99;
			    }
				for(int j = 0; j < n; j++){
					for(int i = 0; i < m; i++){
						field[j][i] = 0;
						opened[j][i] = 0;
					}
				}
				vid.width = 25*(m+2);
				vid.height = 25*(n+4);
				window.create(vid, L"Minesweeper", Style::Default);
				while(sf::Mouse::isButtonPressed(sf::Mouse::Left)) flag = 0;
				start = true;
                    		flag = 0;
				

			}
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			Vector2i pos = sf::Mouse::getPosition(window);
			int y = pos.x;
			int x = pos.y;
			if (x < 25*(n+3) && y < 25*(m+1) && x > 75 && y > 25){
				if (opened[(x-75)/25][(y-25) / 25] == 0){
					opened[(x-75)/25][(y-25) / 25] = -1;
					flags--;
				}
				else if (opened[(x-75)/25][(y-25) / 25] == -1){
					opened[(x-75)/25][(y-25) / 25] = 0;
					flags++;
				}
			}
		}
		if (event.type == Event::Closed) window.close();
        }

    window.clear(Color(192, 192, 192));
    if(to_win == 0 && flag == 0) flag = -2; //-2 win; -1 lose; 0 game;
    if (flag == 1){
        textbox2.setPosition((25*(m+2)-235)/2, (25*(n+4)-94)/2);
        textbox2.setTextureRect(IntRect(0, 56, 235, 28));
        window.draw(textbox2);
        textbox2.move(0, 33);
        textbox2.setTextureRect(IntRect(0, 84, 235, 28));
        window.draw(textbox2);
        textbox2.move(0, 33);
        textbox2.setTextureRect(IntRect(0, 112, 235, 28));
        window.draw(textbox2);
    }
    else{
        if(flag == 10){
            textbox1.setPosition(95, 25);
            textbox1.setTextureRect(IntRect(60, 28, 115, 28));
            window.draw(textbox1);
        }
        if(flag == -2){
            textbox1.setPosition(95, 25);
            textbox1.setTextureRect(IntRect(60, 0, 115, 28));
            window.draw(textbox1);
        }
        number.setPosition(30, 25);
        number.setTextureRect(IntRect(0, (11-flags/100)*35.5, 20, 35.5));
        window.draw(number);
        number.move(20, 0);
        number.setTextureRect(IntRect(0, (11-(flags%100)/10)*35.5, 20, 35.5));
        window.draw(number);
        number.move(20, 0);
        number.setTextureRect(IntRect(0, (11-(flags%10))*35.5, 20, 35.5));
        window.draw(number);
        if (star > 0 && flag == 0) tim = (time(0) - star);
        number.setPosition(25*(m+2) - 50, 25);
        number.setTextureRect(IntRect(0, (11-(tim%10))*35.5, 20, 35.5));
        window.draw(number);
        number.move(-20, 0);
        number.setTextureRect(IntRect(0, (11-(tim%60)/10)*35.5, 20, 35.5));
        window.draw(number);
        number.move(-20, 0);
        number.setTextureRect(IntRect(0, (11-(tim/60))*35.5, 20, 35.5));
        window.draw(number);
            for(int j = 0; j < n; j++){
            for(int i = 0; i < m; i++){
                if(opened[j][i] == 0){
                    field1.setPosition(25+25*i, 75 + 25*j);
                    field1.setTextureRect(IntRect(0, 0, 25, 25));
                    window.draw(field1);
                }
                if(opened[j][i] == -1){
                    field1.setPosition(25+25*i, 75 + 25*j);
                    field1.setTextureRect(IntRect(0, 25, 25, 25));
                    window.draw(field1);
                }
                if(opened[j][i] == 1){
                    field1.setPosition(25+25*i, 75 + 25*j);
                    if(field[j][i] == -1) field1.setTextureRect(IntRect(0, 125, 25, 25));
                    else if(field[j][i] == -2) field1.setTextureRect(IntRect(0, 75, 25, 25));
                    else if(field[j][i] == -3) field1.setTextureRect(IntRect(0, 100, 25, 25));
                    else field1.setTextureRect(IntRect(0, 25*(15-field[j][i]), 25, 25));
                    window.draw(field1);
                }
            }
        }
    }
        window.display();
    }
    return 0;
}
