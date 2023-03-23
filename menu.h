#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

void menu(RenderWindow & window) {
    window.setMouseCursorVisible(true);

	Texture menuTexture1, menuTexture1_3, menuTexture2, menuTexture3, aboutTexture, menuBackground;
	menuTexture1.loadFromFile("menu/play.png");
	menuTexture1_3.loadFromFile("menu/play_3.png");
	menuTexture2.loadFromFile("menu/how.png");
	menuTexture3.loadFromFile("menu/exit.png");
	aboutTexture.loadFromFile("menu/about.png");
	menuBackground.loadFromFile("menu/newfon.png");
	Sprite menu1(menuTexture1), menu1_3(menuTexture1_3), menu2(menuTexture2), menu3(menuTexture3), about(aboutTexture), menuBg(menuBackground);
	bool isMenu = 1;
	int menuNum = 0;
	menuBg.setTextureRect(IntRect(0, 0, 1440, 900));
    menu1.setTextureRect(IntRect(0, 0, 93, 25));
    menu1_3.setTextureRect(IntRect(0, 0, 93, 25));
    menu2.setTextureRect(IntRect(0, 0, 112, 28));
    menu3.setTextureRect(IntRect(0, 0, 93, 25));
    menuBg.setPosition(0, 0);
	menu1.setPosition(menuBg.getPosition().x+200, menuBg.getPosition().y+520);
	menu1_3.setPosition(menuBg.getPosition().x+200, menuBg.getPosition().y+580);
	menu2.setPosition(menuBg.getPosition().x+200, menuBg.getPosition().y+640);
	menu3.setPosition(menuBg.getPosition().x+200, menuBg.getPosition().y+700);
	about.setPosition(25, 0);

    SoundBuffer chelkBuffer;
    chelkBuffer.loadFromFile("choise.ogg");
    Sound chelk(chelkBuffer);

	while (isMenu)
	{
	    Event event;
        while (window.pollEvent(event)) {
           if (event.type == Event::Closed)
                window.close();
        }
		menu1.setColor(Color::White);
		menu1_3.setColor(Color::White);
		menu2.setColor(Color::White);
		menu3.setColor(Color::White);
		menuNum = 0;

		window.clear(Color::Black);

		if (IntRect(menu1.getPosition().x, menu1.getPosition().y, 93, 25).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Yellow); menuNum = 11; }
		if (IntRect(menu1_3.getPosition().x, menu1_3.getPosition().y, 93, 25).contains(Mouse::getPosition(window))) { menu1_3.setColor(Color::Yellow); menuNum = 13; }
		if (IntRect(menu2.getPosition().x, menu2.getPosition().y, 112, 28).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Yellow); menuNum = 20; }
		if (IntRect(menu3.getPosition().x, menu3.getPosition().y, 93, 25).contains(Mouse::getPosition(window))) { menu3.setColor(Color::Red); menuNum = 30; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 11) { isMenu=false; chelk.play(); if(!startGame_1(window)){ menu(window); } };
			if (menuNum == 13) { isMenu=false; chelk.play(); if(!startGame_3(window)){ menu(window); } };
			if (menuNum == 20) { chelk.play(); window.draw(about); window.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape)); }
			if (menuNum == 30)  { chelk.play(); window.close(); isMenu = false;}

		}

		window.draw(menuBg);
		window.draw(menu1);
		window.draw(menu1_3);
		window.draw(menu2);
		window.draw(menu3);

		window.display();
	}
}
