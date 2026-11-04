#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <cmath>

#include "Constants.h"
#include "Map.h"

class Player {

public:

    // Player information
    sf::Vector2f position;
    CharacterProfile profile;

    float currentAngle;
    bool hittingObstacle;


    // Constructor
    Player() {

        CharacterProfile roster[3];

        initCharacterRoster(roster);

        // Start with Noor
        setupCharacter(
            roster[0],
            sf::Vector2f(5.5f * TILE_SIZE, 4.5f * TILE_SIZE)
        );
    }


    // Select a character
    void selectCharacter(int characterID) {

        CharacterProfile roster[3];

        initCharacterRoster(roster);

        if (characterID >= 0 && characterID < 3) {

            setupCharacter(
                roster[characterID],
                sf::Vector2f(5.5f * TILE_SIZE, 4.5f * TILE_SIZE)
            );
        }
    }


    // Set up the selected character
    void setupCharacter(
        const CharacterProfile& chosen,
        sf::Vector2f startPos
    ) {

        profile = chosen;

        position = startPos;

        currentAngle = 0.0f;

        hittingObstacle = false;
    }


    // Get the hitbox at any position
    sf::FloatRect getBoundsAt(float centerX, float centerY) const {

        float half = profile.hitboxSize / 2.0f;

        return sf::FloatRect(
            centerX - half,
            centerY - half,
            profile.hitboxSize,
            profile.hitboxSize
        );
    }


    // Get the current hitbox
    sf::FloatRect getBounds() const {

        return getBoundsAt(position.x, position.y);
    }


    // Check all four corners before moving
    bool canMoveTo(
        float newX,
        float newY,
        const CityMap& cityMap
    ) const {

        sf::FloatRect box = getBoundsAt(newX, newY);

        // Four corners of the hitbox
        float left = box.left;
        float right = box.left + box.width - 1.0f;

        float top = box.top;
        float bottom = box.top + box.height - 1.0f;

        // Convert pixel positions into map rows and columns

        int row1 = static_cast<int>(top) / TILE_SIZE;
        int row2 = static_cast<int>(bottom) / TILE_SIZE;

        int col1 = static_cast<int>(left) / TILE_SIZE;
        int col2 = static_cast<int>(right) / TILE_SIZE;


        // Top-left corner
        if (cityMap.isSolidTile(row1, col1)) {
            return false;
        }

        // Top-right corner
        if (cityMap.isSolidTile(row1, col2)) {
            return false;
        }

        // Bottom-left corner
        if (cityMap.isSolidTile(row2, col1)) {
            return false;
        }

        // Bottom-right corner
        if (cityMap.isSolidTile(row2, col2)) {
            return false;
        }

        return true;
    }


    // Check whether the vehicle is touching a road obstacle
    bool isOnObstacle(const CityMap& cityMap) const {

        sf::FloatRect box = getBounds();

        int firstRow =
            static_cast<int>(box.top) / TILE_SIZE;

        int lastRow =
            static_cast<int>(box.top + box.height - 1) / TILE_SIZE;

        int firstCol =
            static_cast<int>(box.left) / TILE_SIZE;

        int lastCol =
            static_cast<int>(box.left + box.width - 1) / TILE_SIZE;


        // Check every map tile covered by the hitbox
        for (int r = firstRow; r <= lastRow; r++) {

            for (int c = firstCol; c <= lastCol; c++) {

                if (r >= 0 && r < MAP_ROWS &&
                    c >= 0 && c < MAP_COLS) {

                    if (cityMap.grid[r][c] == TILE_OBSTACLE) {
                        return true;
                    }
                }
            }
        }

        return false;
    }


    // Move the player using keyboard
    void updateMovement(float dt, const CityMap& cityMap) {

        float moveX = 0.0f;
        float moveY = 0.0f;


        // Keyboard input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {

            moveY = -1.0f;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {

            moveY = 1.0f;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {

            moveX = -1.0f;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {

            moveX = 1.0f;
        }


        // Calculate movement length
        float length = std::sqrt(
            moveX * moveX + moveY * moveY
        );


        // Prevent diagonal movement from being faster
        if (length > 0.0f) {

            moveX = moveX / length;
            moveY = moveY / length;


            // Update vehicle direction
            if (moveX > 0) {
                currentAngle = 90.0f;
            }
            else if (moveX < 0) {
                currentAngle = 270.0f;
            }
            else if (moveY > 0) {
                currentAngle = 180.0f;
            }
            else if (moveY < 0) {
                currentAngle = 0.0f;
            }
        }


        // Check if player is on an obstacle
        hittingObstacle = isOnObstacle(cityMap);


        // Normal speed
        float currentSpeed = profile.maxSpeed;


        // Apply obstacle speed penalty
        if (hittingObstacle) {

            currentSpeed =
                profile.maxSpeed * profile.obstacleSlowFactor;
        }


        // Calculate distance moved this frame
        float distance = currentSpeed * dt;


        // Calculate new X and Y positions
        float newX = position.x + moveX * distance;
        float newY = position.y + moveY * distance;


        // Check X movement separately
        if (canMoveTo(newX, position.y, cityMap)) {

            position.x = newX;
        }


        // Check Y movement separately
        if (canMoveTo(position.x, newY, cityMap)) {

            position.y = newY;
        }


        // Update obstacle status after movement
        hittingObstacle = isOnObstacle(cityMap);
    }


    // Draw the selected vehicle
    void draw(sf::RenderWindow& window) {

        sf::RectangleShape vehicle;


        // Vehicle size
        float vehicleWidth = profile.hitboxSize;

        float vehicleHeight = profile.hitboxSize * 1.5f;


        vehicle.setSize(
            sf::Vector2f(vehicleWidth, vehicleHeight)
        );

        vehicle.setOrigin(
            vehicleWidth / 2.0f,
            vehicleHeight / 2.0f
        );

        vehicle.setPosition(position);

        vehicle.setRotation(currentAngle);

        vehicle.setFillColor(profile.primaryColor);

        vehicle.setOutlineThickness(2.0f);

        vehicle.setOutlineColor(profile.accentColor);


        window.draw(vehicle);


        // Draw a small front marker
        sf::CircleShape marker(4.0f);

        marker.setOrigin(4.0f, 4.0f);

        marker.setPosition(
            position.x,
            position.y - vehicleHeight / 2.0f + 5.0f
        );

        marker.setFillColor(profile.accentColor);

        window.draw(marker);
    }

};

#endif
