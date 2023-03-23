#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <list>
#include <iterator>
#include <algorithm>
#include <random>

using namespace sf;
using namespace std;

class Entity {
public:
	vector<Object> obj;//вектор объектов карты
	float dx, dy, x, y, speed,moveTimer;
	int w,h,health;
	bool life, isMove, onGround;
	Texture texture;
	Sprite sprite;
	String name;
	Entity(Image &image, String Name, float X, float Y, int W, int H){
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}

	FloatRect getRect(){
		return FloatRect(x, y, w, h);
	}
};

class Player :public Entity {
public:
	enum { left, right, up, down, jump, stay } state;
	int playerScore;

	Player(Image &image, String Name, Level &lev, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H ){
		   playerScore = 0; state = stay; obj = lev.GetAllObjects();
		   if (name == "Player1" or name == "Player2"){
			   sprite.setTextureRect(IntRect(0, 0, w, h));
		   }
	   }

	   void control(){
		   if (Keyboard::isKeyPressed){
			   if (Keyboard::isKeyPressed(Keyboard::A)) {
				   state = left; speed = 0.35;
			   }
			   if (Keyboard::isKeyPressed(Keyboard::D)) {
				   state = right; speed = 0.35;
			   }

			   if ((Keyboard::isKeyPressed(Keyboard::W)) && (onGround)) {
				   state = jump; dy = -0.6; onGround = false;
			   }

			   if (Keyboard::isKeyPressed(Keyboard::S)) {
				   state = down;
			   }
		   }
	   }

	   void control_2(){
		   if (Keyboard::isKeyPressed){
			   if (Keyboard::isKeyPressed(Keyboard::Left)) {
				   state = left; speed = 0.35;
			   }
			   if (Keyboard::isKeyPressed(Keyboard::Right)) {
				   state = right; speed = 0.35;
			   }

			   if ((Keyboard::isKeyPressed(Keyboard::Up)) && (onGround)) {
				   state = jump; dy = -0.6; onGround = false;
			   }

			   if (Keyboard::isKeyPressed(Keyboard::Down)) {
				   state = down;
			   }
		   }
	   }

	   void checkCollisionWithMap(float Dx, float Dy)
	   {

		   for (int i = 0; i<obj.size(); i++)//проходимся по объектам
		   if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
		   {
			   if (obj[i].name == "solid")//если встретили препятствие
			   {
				   if (Dy>0)	{ y = obj[i].rect.top - h;  dy = 0; onGround = true; }
				   if (Dy<0)	{ y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
				   if (Dx>0)	{ x = obj[i].rect.left - w; }
				   if (Dx<0)	{ x = obj[i].rect.left + obj[i].rect.width; }
			   }
		   }
	   }

	   void update(float time)
	   {
	       if(name == "Player1")
            control();
           else if(name == "Player2")
            control_2();
		   switch (state)
		   {
		   case right:dx = speed; break;
		   case left:dx = -speed; break;
		   case up: break;
		   case down: dx = 0; break;
		   case stay: break;
		   }
		   x += dx*time;
		   checkCollisionWithMap(dx, 0);
		   y += dy*time;
		   checkCollisionWithMap(0, dy);
		   sprite.setPosition(x + w / 2, y + h / 2);
		   if (health <= 0){ life = false; }
		   if (!isMove){ speed = 0; }
		   //setPlayerCoordinateForView(x, y);
		   //if (life) { setPlayerCoordinateForView(x, y); }
		   dy = dy + 0.0015*time;
	   }
};

class Cube :public Entity {
public:
    //int orY;
    Cube(Image &image, String Name, Level &lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H){
        sprite.setTextureRect(IntRect(0, 0, W, H));
        //orY=y;
    }
    /*void update(float time){
            if(onGround){
                dy=-0.6;
                onGround=false;
            }
            else if(y>=orY){
                onGround=true;
                dy=0;
            }
            y+=dy*time;
            dy+=0.0015*time;
            sprite.setPosition(x+w/2, y+h/2);
    }*/

};

int getRandom(){
    mt19937 rnd(time(0));
    return rnd() % 10000;
}

bool startGame_1(RenderWindow & window){

    int aa, bb, cc;

    window.setMouseCursorVisible(false);

    View view;
    view.reset(FloatRect(0, 150, 1570, 930));//размер камеры

    list <Cube*> cubes;
    list <Cube*> :: iterator it;

    list <Cube*> cubes_2;
    list <Cube*> :: iterator it_2;

    Image heroImage;
    heroImage.loadFromFile("quad.png");

    Clock clock;
    Clock gtimeClock;

    Font font;//текст
    font.loadFromFile("CyrilicOld.ttf");

    Text text("", font, 50);
    text.setColor(Color::Red);
    Text answer("", font, 50);
    answer.setColor(Color::Red);
    Text answer_2("", font, 50);
    answer_2.setColor(Color::Red);
    Text score("", font, 20);
    score.setColor(Color::Green);
    Text score_2("", font, 20);
    score_2.setColor(Color::Green);
    Text text1("Left player win!", font, 40);
    text1.setColor(Color::Black);
    Text text1_2("Right player win!", font, 40);
    text1_2.setColor(Color::Black);
    Text text2("Press Enter to continue", font, 30);
    text2.setColor(Color::Black);
    Text Ttime("", font, 20);
    Ttime.setColor(Color::Green);

    string answ;
    string answ_2;

    Music music;
    music.openFromFile("-asia.ogg");
    music.play();
    music.setLoop(true);

    SoundBuffer chelkBuffer;
    chelkBuffer.loadFromFile("chelk.ogg");
    Sound chelk(chelkBuffer);

    Level lvl;
    lvl.LoadFromFile("mapp.tmx");

    Object player=lvl.GetObject("player");

    Player p(heroImage, "Player1", lvl, player.rect.left, player.rect.top, 32, 32);
    int originalPY=p.y;

    Object player_2=lvl.GetObject("player_2");

    Player p2(heroImage, "Player2", lvl, player_2.rect.left, player_2.rect.top, 32, 32);
    int originalP2Y=p2.y;

    vector<Object> e=lvl.GetObjects("numb");
    for (int i = 0; i < e.size(); i++){
        if(i==0){
            Image cubeImage;
            cubeImage.loadFromFile("cubes/cube0.png");
            cubes.push_back(new Cube(cubeImage, "Cube0", lvl, e[i].rect.left, e[i].rect.top, 32, 32));
        }
        if(i==1){
            Image cubeImage;
            cubeImage.loadFromFile("cubes/cube1.png");
            cubes.push_back(new Cube(cubeImage, "Cube1", lvl, e[i].rect.left, e[i].rect.top, 32, 32));
        }
        if(i==2){
            Image cubeImage;
            cubeImage.loadFromFile("cubes/cube2.png");
            cubes.push_back(new Cube(cubeImage, "Cube2", lvl, e[i].rect.left, e[i].rect.top, 32, 32));
        }
        if(i==3){
            Image cubeImage;
            cubeImage.loadFromFile("cubes/cube3.png");
            cubes.push_back(new Cube(cubeImage, "Cube3", lvl, e[i].rect.left, e[i].rect.top, 32, 32));
        }
        if(i==4){
            Image cubeImage;
            cubeImage.loadFromFile("cubes/cube4.png");
            cubes.push_back(new Cube(cubeImage, "Cube4", lvl, e[i].rect.left, e[i].rect.top, 32, 32));
        }
        if(i==5){
            Image cubeImage;
            cubeImage.loadFromFile("cubes/cube5.png");
            cubes.push_back(new Cube(cubeImage, "Cube5", lvl, e[i].rect.left, e[i].rect.top, 32, 32));
        }
        if(i==6){
            Image cubeImage;
            cubeImage.loadFromFile("cubes/cube6.png");
            cubes.push_back(new Cube(cubeImage, "Cube6", lvl, e[i].rect.left, e[i].rect.top, 32, 32));
        }
        if(i==7){
            Image cubeImage;
            cubeImage.loadFromFile("cubes/cube7.png");
            cubes.push_back(new Cube(cubeImage, "Cube7", lvl, e[i].rect.left, e[i].rect.top, 32, 32));
        }
        if(i==8){
            Image cubeImage;
            cubeImage.loadFromFile("cubes/cube8.png");
            cubes.push_back(new Cube(cubeImage, "Cube8", lvl, e[i].rect.left, e[i].rect.top, 32, 32));
        }
        if(i==9){
            Image cubeImage;
            cubeImage.loadFromFile("cubes/cube9.png");
            cubes.push_back(new Cube(cubeImage, "Cube9", lvl, e[i].rect.left, e[i].rect.top, 32, 32));
        }
        if(i==10){
            Image cubeImage;
            cubeImage.loadFromFile("cubes/cube10.png");
            cubes.push_back(new Cube(cubeImage, "Cube10", lvl, e[i].rect.left, e[i].rect.top, 32, 32));
        }
        if(i==11){
            Image cubeImage;
            cubeImage.loadFromFile("cubes/cube11.png");
            cubes.push_back(new Cube(cubeImage, "Cube11", lvl, e[i].rect.left, e[i].rect.top, 32, 32));
        }
    }

    vector<Object> e_2=lvl.GetObjects("numb_2");
    for (int i = 0; i < e_2.size(); i++){
        if(i==0){
            Image cubeImage;
            cubeImage.loadFromFile("cubes/cube0.png");
            cubes_2.push_back(new Cube(cubeImage, "Cube0", lvl, e_2[i].rect.left, e_2[i].rect.top, 32, 32));
        }
        if(i==1){
            Image cubeImage;
            cubeImage.loadFromFile("cubes/cube1.png");
            cubes_2.push_back(new Cube(cubeImage, "Cube1", lvl, e_2[i].rect.left, e_2[i].rect.top, 32, 32));
        }
        if(i==2){
            Image cubeImage;
            cubeImage.loadFromFile("cubes/cube2.png");
            cubes_2.push_back(new Cube(cubeImage, "Cube2", lvl, e_2[i].rect.left, e_2[i].rect.top, 32, 32));
        }
        if(i==3){
            Image cubeImage;
            cubeImage.loadFromFile("cubes/cube3.png");
            cubes_2.push_back(new Cube(cubeImage, "Cube3", lvl, e_2[i].rect.left, e_2[i].rect.top, 32, 32));
        }
        if(i==4){
            Image cubeImage;
            cubeImage.loadFromFile("cubes/cube4.png");
            cubes_2.push_back(new Cube(cubeImage, "Cube4", lvl, e_2[i].rect.left, e_2[i].rect.top, 32, 32));
        }
        if(i==5){
            Image cubeImage;
            cubeImage.loadFromFile("cubes/cube5.png");
            cubes_2.push_back(new Cube(cubeImage, "Cube5", lvl, e_2[i].rect.left, e_2[i].rect.top, 32, 32));
        }
        if(i==6){
            Image cubeImage;
            cubeImage.loadFromFile("cubes/cube6.png");
            cubes_2.push_back(new Cube(cubeImage, "Cube6", lvl, e_2[i].rect.left, e_2[i].rect.top, 32, 32));
        }
        if(i==7){
            Image cubeImage;
            cubeImage.loadFromFile("cubes/cube7.png");
            cubes_2.push_back(new Cube(cubeImage, "Cube7", lvl, e_2[i].rect.left, e_2[i].rect.top, 32, 32));
        }
        if(i==8){
            Image cubeImage;
            cubeImage.loadFromFile("cubes/cube8.png");
            cubes_2.push_back(new Cube(cubeImage, "Cube8", lvl, e_2[i].rect.left, e_2[i].rect.top, 32, 32));
        }
        if(i==9){
            Image cubeImage;
            cubeImage.loadFromFile("cubes/cube9.png");
            cubes_2.push_back(new Cube(cubeImage, "Cube9", lvl, e_2[i].rect.left, e_2[i].rect.top, 32, 32));
        }
        if(i==10){
            Image cubeImage;
            cubeImage.loadFromFile("cubes/cube10.png");
            cubes_2.push_back(new Cube(cubeImage, "Cube10", lvl, e_2[i].rect.left, e_2[i].rect.top, 32, 32));
        }
        if(i==11){
            Image cubeImage;
            cubeImage.loadFromFile("cubes/cube11.png");
            cubes_2.push_back(new Cube(cubeImage, "Cube11", lvl, e_2[i].rect.left, e_2[i].rect.top, 32, 32));
        }
    }


    it = cubes.begin();
    Cube *b = *it;
    int originalY = (*it)->y;

    it_2 = cubes_2.begin();
    Cube *b_2 = *it_2;
    int original2Y = (*it_2)->y;

    ostringstream number1, number2;
    aa = getRandom(); bb = getRandom(); bb %= 1111;
    number1 << aa; number2 << bb;
    text.setString(number1.str() + '+' + number2.str());
    text.setPosition(680, 380);

    while (true){

        float time = clock.getElapsedTime().asMicroseconds();
        float gtime = gtimeClock.getElapsedTime().asSeconds();
        clock.restart();
        time/=800;

        Event event;

        while (window.pollEvent(event)){
            if (event.type == Event::Closed)
                window.close();
        }

		for (it = cubes.begin(); it != cubes.end(); it++)
		{
			if (((*it)->name == "Cube0") && ((*it)->getRect().intersects(p.getRect())))
			{
			    Cube *b = *it;
			    (*it)->y-=96;
			    answ+='0';
			    chelk.play();
			}
			if (((*it)->name == "Cube1") && ((*it)->getRect().intersects(p.getRect())))
			{
			    Cube *b = *it;
			    (*it)->y-=96;
			    answ+='1';
			    chelk.play();
			}
			if (((*it)->name == "Cube2") && ((*it)->getRect().intersects(p.getRect())))
			{
			    Cube *b = *it;
			    (*it)->y-=96;
			    answ+='2';
			    chelk.play();
			}
			if (((*it)->name == "Cube3") && ((*it)->getRect().intersects(p.getRect())))
			{
			    Cube *b = *it;
			    (*it)->y-=96;
			    answ+='3';
			    chelk.play();
			}
			if (((*it)->name == "Cube4") && ((*it)->getRect().intersects(p.getRect())))
			{
			    Cube *b = *it;
			    (*it)->y-=96;
			    answ+='4';
			    chelk.play();
			}
			if (((*it)->name == "Cube5") && ((*it)->getRect().intersects(p.getRect())))
			{
			    Cube *b = *it;
			    (*it)->y-=96;
			    answ+='5';
			    chelk.play();
			}
			if (((*it)->name == "Cube6") && ((*it)->getRect().intersects(p.getRect())))
			{
			    Cube *b = *it;
			    (*it)->y-=96;
			    answ+='6';
			    chelk.play();
			}
			if (((*it)->name == "Cube7") && ((*it)->getRect().intersects(p.getRect())))
			{
			    Cube *b = *it;
			    (*it)->y-=96;
			    answ+='7';
			    chelk.play();
			}
			if (((*it)->name == "Cube8") && ((*it)->getRect().intersects(p.getRect())))
			{
			    Cube *b = *it;
			    (*it)->y-=96;
			    answ+='8';
			    chelk.play();
			}
			if (((*it)->name == "Cube9") && ((*it)->getRect().intersects(p.getRect())))
			{
			    Cube *b = *it;
			    (*it)->y-=96;
			    answ+='9';
			    chelk.play();
			}
			if (((*it)->name == "Cube10") && ((*it)->getRect().intersects(p.getRect())))
			{
			    Cube *b = *it;
			    (*it)->y-=96;
			    stringstream sc;
			    sc<<answ;
			    sc>>cc;
			    if(aa+bb==cc){
                    chelk.play();
                    p.playerScore++;
                    ostringstream number3, number4;
                    aa = getRandom(); bb = getRandom();
                    number3 << aa; number4 << bb%1111; bb%=1111;
                    text.setString(number3.str() + '+' + number4.str());
                    text.setPosition(680, 380);
                    answ="";
                    answ_2="";
			    }
			    if(p.playerScore==10){
                    showMission(window, text1, text1_2, text2, Ttime, 1);
                    window.setView(window.getDefaultView());
                    return false;
			    }
			}
            if (((*it)->name == "Cube11") && ((*it)->getRect().intersects(p.getRect())))
            {
			    Cube *b = *it;
			    (*it)->y-=96;
			    if(answ.size()>0){
                    string answw;
                    for(int j =0; j< answ.size()-1; ++j){
                        answw+=answ[j];
                    }
                    answ=answw;
			    }
			    chelk.play();
            }
			(*it)->sprite.setPosition((*it)->x + (*it)->w / 2, (*it)->y + (*it)->h / 2);
		}

		for (it_2 = cubes_2.begin(); it_2 != cubes_2.end(); it_2++)
		{
			if (((*it_2)->name == "Cube0") && ((*it_2)->getRect().intersects(p2.getRect())))
			{
			    Cube *b = *it_2;
			    (*it_2)->y-=96;
			    answ_2+='0';
			    chelk.play();
			}
			if (((*it_2)->name == "Cube1") && ((*it_2)->getRect().intersects(p2.getRect())))
			{
			    Cube *b = *it_2;
			    (*it_2)->y-=96;
			    answ_2+='1';
			    chelk.play();
			}
			if (((*it_2)->name == "Cube2") && ((*it_2)->getRect().intersects(p2.getRect())))
			{
			    Cube *b = *it_2;
			    (*it_2)->y-=96;
			    answ_2+='2';
			    chelk.play();
			}
			if (((*it_2)->name == "Cube3") && ((*it_2)->getRect().intersects(p2.getRect())))
			{
			    Cube *b = *it_2;
			    (*it_2)->y-=96;
			    answ_2+='3';
			    chelk.play();
			}
			if (((*it_2)->name == "Cube4") && ((*it_2)->getRect().intersects(p2.getRect())))
			{
			    Cube *b = *it_2;
			    (*it_2)->y-=96;
			    answ_2+='4';
			    chelk.play();
			}
			if (((*it_2)->name == "Cube5") && ((*it_2)->getRect().intersects(p2.getRect())))
			{
			    Cube *b = *it_2;
			    (*it_2)->y-=96;
			    answ_2+='5';
			    chelk.play();
			}
			if (((*it_2)->name == "Cube6") && ((*it_2)->getRect().intersects(p2.getRect())))
			{
			    Cube *b = *it_2;
			    (*it_2)->y-=96;
			    answ_2+='6';
			    chelk.play();
			}
			if (((*it_2)->name == "Cube7") && ((*it_2)->getRect().intersects(p2.getRect())))
			{
			    Cube *b = *it_2;
			    (*it_2)->y-=96;
			    answ_2+='7';
			    chelk.play();
			}
			if (((*it_2)->name == "Cube8") && ((*it_2)->getRect().intersects(p2.getRect())))
			{
			    Cube *b = *it_2;
			    (*it_2)->y-=96;
			    answ_2+='8';
			    chelk.play();
			}
			if (((*it_2)->name == "Cube9") && ((*it_2)->getRect().intersects(p2.getRect())))
			{
			    Cube *b = *it_2;
			    (*it_2)->y-=96;
			    answ_2+='9';
			    chelk.play();
			}
			if (((*it_2)->name == "Cube10") && ((*it_2)->getRect().intersects(p2.getRect())))
			{
			    Cube *b = *it_2;
			    (*it_2)->y-=96;
			    stringstream sc;
			    sc<<answ_2;
			    sc>>cc;
			    if(aa+bb==cc){
                    chelk.play();
                    p2.playerScore++;
                    ostringstream number3, number4;
                    aa = getRandom(); bb = getRandom();
                    number3 << aa; number4 << bb%1111; bb%=1111;
                    text.setString(number3.str() + '+' + number4.str());
                    text.setPosition(680, 380);
                    answ="";
                    answ_2="";
			    }
			    if(p2.playerScore==10){
                    showMission(window, text1, text1_2, text2, Ttime, 2);
                    window.setView(window.getDefaultView());
                    return false;
			    }
			}
            if (((*it_2)->name == "Cube11") && ((*it_2)->getRect().intersects(p2.getRect())))
            {
			    Cube *b = *it_2;
			    (*it_2)->y-=96;
			    if(answ_2.size()>0){
                    string answw;
                    for(int j =0; j< answ_2.size()-1; ++j){
                        answw+=answ_2[j];
                    }
                    answ_2=answw;
			    }
			    chelk.play();
            }
			(*it_2)->sprite.setPosition((*it_2)->x + (*it_2)->w / 2, (*it_2)->y + (*it_2)->h / 2);
		}

		p.update(time);
		p2.update(time);

		window.setView(view);

		window.clear(Color(168,146,92));

		lvl.Draw(window);

		for (it = cubes.begin(); it != cubes.end(); it++){
			window.draw((*it)->sprite);
		}

		for (it_2 = cubes_2.begin(); it_2 != cubes_2.end(); it_2++){
			window.draw((*it_2)->sprite);
		}

		answer.setString(answ);
		answer.setPosition(300, 450);

		answer_2.setString(answ_2);
		answer_2.setPosition(1000, 450);

		window.draw(p.sprite);

		window.draw(p2.sprite);

		window.draw(text);

		window.draw(answer);
		window.draw(answer_2);

		ostringstream points;
		points << p.playerScore;
		score.setString("Left player score: " + points.str());
		score.setPosition(40, 380);
		window.draw(score);

		ostringstream points_2;
		points_2 << p2.playerScore;
		score_2.setString("Right player score: " + points_2.str());
		score_2.setPosition(1350, 380);
		window.draw(score_2);

		int ggtime = gtime;
        ostringstream times;
        times << ggtime;
        Ttime.setString("Game time: " + times.str());
        Ttime.setPosition(40, 400);
        window.draw(Ttime);

        if(p.y > originalPY){
            for (it = cubes.begin(); it != cubes.end(); it++)
            {
                Cube *b = *it;
                (*it)->y=originalY;
                (*it)->sprite.setPosition((*it)->x + (*it)->w / 2, (*it)->y + (*it)->h / 2);
            }

            for (it = cubes.begin(); it != cubes.end(); it++){
                window.draw((*it)->sprite);
            }
        }

        if(p2.y > originalP2Y){
            for (it_2 = cubes_2.begin(); it_2 != cubes_2.end(); it_2++)
            {
                Cube *b = *it_2;
                (*it_2)->y=original2Y;
                (*it_2)->sprite.setPosition((*it_2)->x + (*it_2)->w / 2, (*it_2)->y + (*it_2)->h / 2);
            }

            for (it_2 = cubes_2.begin(); it_2 != cubes_2.end(); it_2++){
                window.draw((*it_2)->sprite);
            }
        }

		window.display();

        if (Keyboard::isKeyPressed(Keyboard::Escape)) {break;}
    }
    window.setView(window.getDefaultView());
    return false;
}
