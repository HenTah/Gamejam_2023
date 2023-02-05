#include "main.h"

int main()
{
    Game    game;

    while (game.window.isOpen())
    {
        sf::Event event;
        while (game.window.pollEvent(event))
            game.handle_events(event);

        game.update_values();
        game.render();
    }

    return 0;
}