#include <SFML/Graphics.hpp>
#include <iostream>
#include "constant.h"
#include "Map.h"

// ABDUL BASEER KHAN L26-0753



int main() {
    // firstly ,Creating  the 960x720 SFML Window using dimensions from Noor's constant.h
    sf::RenderWindow window(
        sf::VideoMode(WINDOW_W, WINDOW_H),
        "Last Delivery - Group 11 (Session 1 Integrated)"
    );

    // Locking frame rate to 60 FPS only
    window.setFramerateLimit(60);

    //  Loading font for HUD text
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        // Fallback for macOS if arial.ttf is in system folders
        font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf");
    }

    //  Initializing Character Roster i.e verifying struct works
    CharacterProfile roster[3];
    initCharacterRoster(roster);

    //  Instantiate Faiq's CityMap class (from Map.h)
    CityMap cityMap;

    //  SFML Clock for delta time (dt)
    sf::Clock frameClock;

    // MAIN GAME LOOP first input then update and finally draw
    
    while (window.isOpen()) {
        float dt = frameClock.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f;

        // --- INPUT ---
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }

        //  DRAW 
        window.clear(sf::Color(20, 28, 38));

        // Drawing Faiq's 2D City Map using his built-in draw() function
        cityMap.draw(window);

        // Top-Left Status Banner verifying integration
        sf::RectangleShape banner(sf::Vector2f(380.0f, 48.0f));
        banner.setPosition(12.0f, 12.0f);
        banner.setFillColor(sf::Color(15, 23, 34, 235));
        banner.setOutlineThickness(2.0f);
        banner.setOutlineColor(sf::Color(90, 140, 190));
        window.draw(banner);

        sf::Text titleText("Last Delivery | Team Integrated (26L-0754)", font, 16);
        titleText.setPosition(24.0f, 24.0f);
        titleText.setFillColor(sf::Color::White);
        window.draw(titleText);

        window.display();
    }

    return 0;
}
