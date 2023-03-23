#include "mission.h"
#include "level.h"
#include "Game_1.h"
#include "Game_3.h"
#include "menu.h"

int main(){
    RenderWindow window(VideoMode(1300, 760), "Game", Style::Fullscreen);
    menu(window);
    return 0;
}
