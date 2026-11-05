#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>

#include "Constants.h"
#include "Map.h"
#include "Player.h"
#include "Order.h"
#include "UIAndFile.h"

// =====================================================
// LAST DELIVERY - GROUP 11
// Main Game Loop and State Controller
// =====================================================

int main()
{
    // 1. Create game window
    sf::RenderWindow window(
        sf::VideoMode(WINDOW_W, WINDOW_H),
        "Last Delivery - Group 11"
    );

    window.setFramerateLimit(60);

    // 2. Load font
    sf::Font font;

    if (!font.loadFromFile("arial.ttf"))
    {
        std::cout << "Font could not be loaded!" << std::endl;
    }

    // 3. Initialize character roster
    CharacterProfile roster[3];

    initCharacterRoster(roster);

    int selectedCharIndex = 0;

    // 4. Initialize game objects
    CityMap cityMap;
    Player player;
    OrderManager orderManager;
    UIAndFileManager uiManager;

    // 5. Initial game state
    GameState currentState = STATE_MENU;

    sf::Clock frameClock;

    bool highScoreSaved = false;

    // -------------------------------------------------
    // Helper function to draw text
    // -------------------------------------------------

    auto drawText = [&](std::string message,
                        int size,
                        float x,
                        float y,
                        sf::Color color)
    {
        sf::Text text;

        text.setFont(font);
        text.setString(message);
        text.setCharacterSize(size);
        text.setPosition(x, y);
        text.setFillColor(color);

        window.draw(text);
    };

    // -------------------------------------------------
    // Helper function to restart the game
    // -------------------------------------------------

    auto restartGame = [&]()
    {
        orderManager.resetOrders();

        player.setupCharacter(
            roster[selectedCharIndex],
            sf::Vector2f(
                5.5f * TILE_SIZE,
                4.5f * TILE_SIZE
            )
        );

        currentState = STATE_PLAYING;

        highScoreSaved = false;
    };

    // =================================================
    // MAIN GAME LOOP
    // =================================================

    while (window.isOpen())
    {
        // Calculate time between frames
        float dt = frameClock.restart().asSeconds();

        if (dt > 0.1f)
        {
            dt = 0.1f;
        }

        // =============================================
        // 6. HANDLE EVENTS
        // =============================================

        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed)
            {
                // Escape key
                if (event.key.code == sf::Keyboard::Escape)
                {
                    if (currentState == STATE_INSTRUCTIONS)
                    {
                        currentState = STATE_MENU;
                    }
                    else if (currentState == STATE_PAUSED)
                    {
                        currentState = STATE_PLAYING;
                    }
                    else
                    {
                        window.close();
                    }
                }

                // =====================================
                // MAIN MENU
                // =====================================

                if (currentState == STATE_MENU)
                {
                    if (event.key.code == sf::Keyboard::Enter)
                    {
                        currentState = STATE_CHAR_SELECT;
                    }

                    if (event.key.code == sf::Keyboard::I)
                    {
                        currentState = STATE_INSTRUCTIONS;
                    }
                }

                // =====================================
                // CHARACTER SELECTION
                // =====================================

                else if (currentState == STATE_CHAR_SELECT)
                {
                    if (event.key.code == sf::Keyboard::Num1)
                    {
                        selectedCharIndex = 0;
                    }

                    else if (event.key.code == sf::Keyboard::Num2)
                    {
                        selectedCharIndex = 1;
                    }

                    else if (event.key.code == sf::Keyboard::Num3)
                    {
                        selectedCharIndex = 2;
                    }

                    if (event.key.code == sf::Keyboard::Enter)
                    {
                        restartGame();
                    }
                }

                // =====================================
                // PLAYING STATE
                // =====================================

                else if (currentState == STATE_PLAYING)
                {
                    if (event.key.code == sf::Keyboard::P)
                    {
                        currentState = STATE_PAUSED;
                    }

                    if (event.key.code == sf::Keyboard::R)
                    {
                        restartGame();
                    }
                }

                // =====================================
                // PAUSED STATE
                // =====================================

                else if (currentState == STATE_PAUSED)
                {
                    if (event.key.code == sf::Keyboard::P)
                    {
                        currentState = STATE_PLAYING;
                    }

                    if (event.key.code == sf::Keyboard::R)
                    {
                        restartGame();
                    }
                }

                // =====================================
                // GAME OVER OR VICTORY
                // =====================================

                else if (
                    currentState == STATE_GAME_OVER ||
                    currentState == STATE_VICTORY
                )
                {
                    if (event.key.code == sf::Keyboard::R)
                    {
                        restartGame();
                    }

                    if (event.key.code == sf::Keyboard::Enter)
                    {
                        currentState = STATE_MENU;
                    }
                }
            }
        }

        // =============================================
        // 7. UPDATE GAME
        // =============================================

        if (currentState == STATE_PLAYING)
        {
            // Move player
            player.updateMovement(dt, cityMap);

            // Check whether all orders are completed
            if (orderManager.currentOrderIndex >=
                static_cast<int>(orderManager.orders.size()))
            {
                currentState = STATE_VICTORY;
            }

            // Check active order
            else if (!orderManager.orders.empty())
            {
                Order& currentOrder =
                    orderManager.orders[
                        orderManager.currentOrderIndex
                    ];

                // Countdown timer
                currentOrder.timeRemaining -= dt;

                // Check timeout
                if (currentOrder.timeRemaining <= 0.0f)
                {
                    orderManager.failedCount++;

                    orderManager.streakCount = 0;

                    orderManager.currentOrderIndex++;

                    orderManager.statusBanner =
                        "Order Failed!";

                    if (orderManager.failedCount >= 3)
                    {
                        currentState = STATE_GAME_OVER;
                    }
                    else if (
                        orderManager.currentOrderIndex >=
                        static_cast<int>(orderManager.orders.size())
                    )
                    {
                        currentState = STATE_VICTORY;
                    }
                }

                // Check pickup and delivery
                else
                {
                    sf::Vector2f targetPosition;

                    if (!currentOrder.packagePickedUp)
                    {
                        targetPosition =
                            cityMap.tileToPixelCenter(
                                currentOrder.pickupRow,
                                currentOrder.pickupCol
                            );
                    }
                    else
                    {
                        targetPosition =
                            cityMap.tileToPixelCenter(
                                currentOrder.dropRow,
                                currentOrder.dropCol
                            );
                    }

                    // Create target zone
                    sf::FloatRect targetZone(
                        targetPosition.x - 28,
                        targetPosition.y - 28,
                        56,
                        56
                    );

                    // Check collision with player
                    if (player.getBounds().intersects(targetZone))
                    {
                        // Package pickup
                        if (!currentOrder.packagePickedUp)
                        {
                            currentOrder.packagePickedUp = true;

                            orderManager.statusBanner =
                                "Package Picked Up! Deliver it!";
                        }

                        // Package delivery
                        else
                        {
                            int reward = static_cast<int>(
                                currentOrder.baseReward *
                                player.profile.scoreMultiplier
                            );

                            orderManager.score += reward;

                            orderManager.completedCount++;

                            orderManager.streakCount++;

                            orderManager.currentOrderIndex++;

                            orderManager.statusBanner =
                                "Delivery Successful!";

                            if (
                                orderManager.currentOrderIndex >=
                                static_cast<int>(orderManager.orders.size())
                            )
                            {
                                currentState = STATE_VICTORY;
                            }
                        }
                    }
                }
            }
            else
            {
                currentState = STATE_VICTORY;
            }
        }

        // =============================================
        // 8. SAVE HIGH SCORE WHEN GAME ENDS
        // =============================================

        if (
            (currentState == STATE_GAME_OVER ||
             currentState == STATE_VICTORY) &&
            !highScoreSaved
        )
        {
            std::ofstream outFile("highscores.txt", std::ios::app);

            if (outFile.is_open())
            {
                outFile
                    << player.profile.riderName << "-"
                    << player.profile.vehicleName << " "
                    << orderManager.score << std::endl;

                outFile.close();
            }

            highScoreSaved = true;
        }

        // =============================================
        // 9. DRAW EVERYTHING
        // =============================================

        window.clear(sf::Color(35, 35, 35));

        // =============================================
        // MAIN MENU
        // =============================================

        if (currentState == STATE_MENU)
        {
            drawText(
                "LAST DELIVERY",
                55,
                280,
                180,
                sf::Color::Yellow
            );

            drawText(
                "Press ENTER to Start",
                25,
                330,
                330,
                sf::Color::White
            );

            drawText(
                "Press I for Instructions",
                22,
                330,
                390,
                sf::Color::White
            );

            drawText(
                "Press ESC to Exit",
                20,
                350,
                450,
                sf::Color::White
            );
        }

        // =============================================
        // INSTRUCTIONS
        // =============================================

        else if (currentState == STATE_INSTRUCTIONS)
        {
            drawText(
                "HOW TO PLAY",
                40,
                350,
                100,
                sf::Color::Yellow
            );

            drawText(
                "W A S D or Arrow Keys: Move",
                23,
                250,
                230,
                sf::Color::White
            );

            drawText(
                "Collect packages from GREEN markers",
                23,
                250,
                290,
                sf::Color::White
            );

            drawText(
                "Deliver packages to YELLOW markers",
                23,
                250,
                350,
                sf::Color::White
            );

            drawText(
                "P: Pause",
                23,
                250,
                410,
                sf::Color::White
            );

            drawText(
                "R: Restart",
                23,
                250,
                450,
                sf::Color::White
            );

            drawText(
                "ESC: Back to Menu",
                23,
                250,
                510,
                sf::Color::White
            );
        }

        // =============================================
        // CHARACTER SELECTION
        // =============================================

        else if (currentState == STATE_CHAR_SELECT)
        {
            drawText(
                "SELECT YOUR RIDER",
                40,
                300,
                100,
                sf::Color::Yellow
            );

            for (int i = 0; i < 3; i++)
            {
                std::string characterInfo =
                    std::to_string(i + 1) + ". " +
                    roster[i].riderName + " - " +
                    roster[i].vehicleName;

                sf::Color textColor = sf::White;

                if (selectedCharIndex == i)
                {
                    textColor = sf::Color::Green;
                }

                drawText(
                    characterInfo,
                    25,
                    250,
                    250 + i * 65,
                    textColor
                );
            }

            drawText(
                "Press 1, 2 or 3 to Select",
                22,
                300,
                490,
                sf::Color::White
            );

            drawText(
                "Press ENTER to Play",
                22,
                300,
                540,
                sf::Color::White
            );
        }

        // =============================================
        // PLAYING STATE
        // =============================================

        else if (
            currentState == STATE_PLAYING ||
            currentState == STATE_PAUSED
        )
        {
            // Draw city map
            cityMap.draw(window);

            // Draw active delivery marker
            if (
                orderManager.currentOrderIndex <
                static_cast<int>(orderManager.orders.size())
            )
            {
                Order& currentOrder =
                    orderManager.orders[
                        orderManager.currentOrderIndex
                    ];

                sf::Vector2f markerPosition;

                sf::CircleShape marker(18);

                marker.setOrigin(18, 18);

                if (!currentOrder.packagePickedUp)
                {
                    markerPosition =
                        cityMap.tileToPixelCenter(
                            currentOrder.pickupRow,
                            currentOrder.pickupCol
                        );

                    marker.setFillColor(sf::Color::Green);
                }
                else
                {
                    markerPosition =
                        cityMap.tileToPixelCenter(
                            currentOrder.dropRow,
                            currentOrder.dropCol
                        );

                    marker.setFillColor(sf::Color::Yellow);
                }

                marker.setPosition(markerPosition);

                window.draw(marker);
            }

            // Draw player
            player.draw(window);

            // Draw score
            drawText(
                "Cash: " + std::to_string(orderManager.score),
                20,
                10,
                10,
                sf::Color::White
            );

            // Draw completed orders
            drawText(
                "Delivered: " +
                std::to_string(orderManager.completedCount),
                18,
                10,
                40,
                sf::Color::White
            );

            // Draw failed orders
            drawText(
                "Failed: " +
                std::to_string(orderManager.failedCount) +
                "/3",
                18,
                10,
                65,
                sf::Color::White
            );

            // Draw character name
            drawText(
                player.profile.riderName + " - " +
                player.profile.vehicleName,
                18,
                650,
                10,
                sf::Color::White
            );

            // Draw countdown
            if (
                orderManager.currentOrderIndex <
                static_cast<int>(orderManager.orders.size())
            )
            {
                int remainingTime = static_cast<int>(
                    orderManager.orders[
                        orderManager.currentOrderIndex
                    ].timeRemaining
                );

                drawText(
                    "Time: " + std::to_string(remainingTime),
                    18,
                    650,
                    40,
                    sf::Color::White
                );
            }

            // Obstacle warning
            if (player.hittingObstacle)
            {
                drawText(
                    "WARNING: SLOW DOWN!",
                    20,
                    350,
                    10,
                    sf::Color::Red
                );
            }

            // Order status
            drawText(
                orderManager.statusBanner,
                18,
                250,
                680,
                sf::Color::White
            );

            // Pause overlay
            if (currentState == STATE_PAUSED)
            {
                sf::RectangleShape overlay(
                    sf::Vector2f(WINDOW_W, WINDOW_H)
                );

                overlay.setFillColor(
                    sf::Color(0, 0, 0, 180)
                );

                window.draw(overlay);

                drawText(
                    "GAME PAUSED",
                    45,
                    350,
                    300,
                    sf::Color::Yellow
                );

                drawText(
                    "Press P to Resume",
                    23,
                    365,
                    380,
                    sf::Color::White
                );
            }
        }

        // =============================================
        // GAME OVER
        // =============================================

        else if (currentState == STATE_GAME_OVER)
        {
            drawText(
                "GAME OVER!",
                55,
                350,
                200,
                sf::Color::Red
            );

            drawText(
                "Final Score: " +
                std::to_string(orderManager.score),
                28,
                350,
                310,
                sf::Color::White
            );

            drawText(
                "Press R to Restart",
                23,
                350,
                400,
                sf::Color::White
            );

            drawText(
                "Press ENTER for Menu",
                23,
                350,
                450,
                sf::Color::White
            );
        }

        // =============================================
        // VICTORY
        // =============================================

        else if (currentState == STATE_VICTORY)
        {
            drawText(
                "DELIVERY COMPLETED!",
                45,
                250,
                200,
                sf::Color::Green
            );

            drawText(
                "Congratulations!",
                28,
                350,
                290,
                sf::Color::White
            );

            drawText(
                "Final Score: " +
                std::to_string(orderManager.score),
                28,
                350,
                350,
                sf::Color::Yellow
            );

            drawText(
                "Press R to Play Again",
                23,
                350,
                430,
                sf::Color::White
            );

            drawText(
                "Press ENTER for Menu",
                23,
                350,
                480,
                sf::Color::White
            );
        }

        // Display the frame
        window.display();
    }

    return 0;
}
