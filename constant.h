#pragma once

#include <SFML/Graphics.hpp>
#include <string>

// Window and map dimensions
const int TILE_SIZE = 48;
const int MAP_COLS = 20;
const int MAP_ROWS = 15;

const int WINDOW_W = TILE_SIZE * MAP_COLS;
const int WINDOW_H = TILE_SIZE * MAP_ROWS;

// Tile codes
const int TILE_ROAD     = 0;
const int TILE_BUILDING = 1;
const int TILE_PARK     = 2;
const int TILE_SHOP     = 3;
const int TILE_HOUSE    = 4;
const int TILE_OBSTACLE = 5;

// Game states
enum GameState {
    STATE_MENU,
    STATE_CHAR_SELECT,
    STATE_INSTRUCTIONS,
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_GAME_OVER,
    STATE_VICTORY
};

// Character information
struct CharacterProfile {
    int id;
    std::string riderName;
    std::string vehicleName;

    float maxSpeed;
    float hitboxSize;
    float scoreMultiplier;
    float obstacleSlowFactor;

    sf::Color primaryColor;
    sf::Color accentColor;
};

// Fill the array with the three available characters
inline void initCharacterRoster(CharacterProfile roster[3]) {

    // Character 0: Noor
    roster[0].id = 0;
    roster[0].riderName = "Noor";
    roster[0].vehicleName = "Cycle";
    roster[0].maxSpeed = 170.0f;
    roster[0].hitboxSize = 22.0f;
    roster[0].scoreMultiplier = 1.0f;
    roster[0].obstacleSlowFactor = 0.80f;
    roster[0].primaryColor = sf::Color::Cyan;
    roster[0].accentColor = sf::Color::White;

    // Character 1: Khan
    roster[1].id = 1;
    roster[1].riderName = "Khan";
    roster[1].vehicleName = "Harley Davidson";
    roster[1].maxSpeed = 230.0f;
    roster[1].hitboxSize = 28.0f;
    roster[1].scoreMultiplier = 1.25f;
    roster[1].obstacleSlowFactor = 0.55f;
    roster[1].primaryColor = sf::Color::Red;
    roster[1].accentColor = sf::Color::Black;

    // Character 2: Jimmy
    roster[2].id = 2;
    roster[2].riderName = "Jimmy";
    roster[2].vehicleName = "Beamer (BMW)";
    roster[2].maxSpeed = 285.0f;
    roster[2].hitboxSize = 34.0f;
    roster[2].scoreMultiplier = 1.50f;
    roster[2].obstacleSlowFactor = 0.55f;
    roster[2].primaryColor = sf::Color::Blue;
    roster[2].accentColor = sf::Color::White;
}

#endif
