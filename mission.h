#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

bool showMission(RenderWindow & window, Text &text1, Text &text1_2, Text &text2, Text &Ttime, int win){
    window.setView(window.getDefaultView());
    Image missionImage;
    missionImage.loadFromFile("missionbg.jpg");

    Texture missionTexture;
    missionTexture.loadFromImage(missionImage);

    Sprite missionSprite;
    missionSprite.setTexture(missionTexture);
    missionSprite.setTextureRect(IntRect(0, 0, 340, 510));
    missionSprite.setPosition(500, 200);

    text1.setPosition(540, 200);
    text1_2.setPosition(540, 200);
    text2.setPosition(520, 650);
    Ttime.setPosition(520, 300);
    Ttime.setCharacterSize(25);
    Ttime.setColor(Color::Black);

    while(true){
        Event event;

        while (window.pollEvent(event)){
            if (event.type == Event::Closed)
                window.close();
        }

        window.draw(missionSprite);
        if(win == 1){
            window.draw(Ttime);
            window.draw(text1);
            window.draw(text2);
        }
        else{
            window.draw(Ttime);
            window.draw(text1_2);
            window.draw(text2);
        }
        window.display();
        if (Keyboard::isKeyPressed(Keyboard::Enter)) {break;}
    }
    return false;
}
