#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include "Constants.h"

class CityMap {
public:
    int grid[MAP_ROWS][MAP_COLS];

    CityMap() {
        loadDefaultCity();
    }

    void loadDefaultCity() {
        int city[MAP_ROWS][MAP_COLS] = {
            {1,1,1,2,2,0,0,2,1,1,1,2,0,0,2,1,1,1,1,2},
            {1,1,1,2,1,0,0,2,1,1,1,2,0,0,2,1,3,1,1,2},
            {1,1,1,2,3,0,0,2,1,1,1,2,0,0,2,2,0,2,2,2},
            {2,2,2,2,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,0,0},
            {0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {2,2,0,2,2,0,0,2,2,1,1,2,0,0,2,4,1,1,2,2},
            {1,1,3,1,2,0,0,2,1,1,1,2,0,0,2,1,1,1,1,2},
            {1,1,1,1,2,0,0,2,1,1,4,2,0,0,2,1,1,1,4,2},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,5,0,0,0,0,0,5,0,0,0,0,0,0,0},
            {2,2,4,2,2,0,0,2,2,2,0,2,0,0,2,2,3,2,2,2},
            {1,1,1,1,2,0,0,2,1,1,3,2,0,0,2,1,1,1,1,2},
            {1,1,1,1,2,0,0,2,1,1,1,2,0,0,2,1,1,4,1,2},
            {2,2,2,2,2,0,0,2,2,2,2,2,0,0,2,2,2,2,2,2}
        };

        for (int row = 0; row < MAP_ROWS; row++) {
            for (int col = 0; col < MAP_COLS; col++) {
                grid[row][col] = city[row][col];
            }
        }
    }

    bool isSolidTile(int row, int col) const {
        if (row < 0 || row >= MAP_ROWS ||
            col < 0 || col >= MAP_COLS) {
            return true;
        }

        return grid[row][col] == TILE_BUILDING ||
               grid[row][col] == TILE_PARK;
    }

    void draw(sf::RenderWindow& window) const {
        sf::RectangleShape tileShape;
        tileShape.setSize(
            sf::Vector2f(
                static_cast<float>(TILE_SIZE),
                static_cast<float>(TILE_SIZE)
            )
        );

        for (int row = 0; row < MAP_ROWS; row++) {
            for (int col = 0; col < MAP_COLS; col++) {
                tileShape.setPosition(
                    static_cast<float>(col * TILE_SIZE),
                    static_cast<float>(row * TILE_SIZE)
                );

                int tile = grid[row][col];

                if (tile == TILE_ROAD) {
                    tileShape.setFillColor(sf::Color(65, 68, 74));
                } else if (tile == TILE_BUILDING) {
                    tileShape.setFillColor(sf::Color(150, 95, 75));
                } else if (tile == TILE_PARK) {
                    tileShape.setFillColor(sf::Color(55, 135, 75));
                } else if (tile == TILE_SHOP) {
                    tileShape.setFillColor(sf::Color(40, 190, 90));
                } else if (tile == TILE_HOUSE) {
                    tileShape.setFillColor(sf::Color(240, 205, 65));
                } else if (tile == TILE_OBSTACLE) {
                    tileShape.setFillColor(sf::Color(230, 95, 45));
                }

                window.draw(tileShape);
            }
        }
    }
};

#endif
