#ifndef UI_AND_FILE_H
#define UI_AND_FILE_H

#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <string>
#include "Constants.h"
#include "Map.h"
#include "Player.h"
#include "Order.h"

// =====================================================
// FAIQ AZIZ - High Score Structure and File Handling
// =====================================================

struct HighScoreEntry
{
    std::string riderAndVehicle;
    int score;
};

class UIAndFileManager
{
public:

    HighScoreEntry topScores[10];
    int scoreCount;

    // Constructor
    UIAndFileManager()
    {
        scoreCount = 0;
        loadHighScores();
    }

    // =================================================
    // LOAD HIGH SCORES FROM FILE
    // =================================================

    void loadHighScores()
    {
        scoreCount = 0;

        std::ifstream inFile("highscores.txt");

        if (!inFile.is_open())
        {
            // Default scores if file does not exist

            topScores[0] = {"Jimmy-Beamer", 650};
            topScores[1] = {"Khan-Harley", 520};
            topScores[2] = {"Noor-Cycle", 410};

            scoreCount = 3;

            return;
        }

        while (scoreCount < 10 &&
               (inFile >> topScores[scoreCount].riderAndVehicle
                       >> topScores[scoreCount].score))
        {
            scoreCount++;
        }

        inFile.close();

        sortHighScoresBubble();
    }

    // =================================================
    // BUBBLE SORT - DESCENDING ORDER
    // Highest score comes first
    // =================================================

    void sortHighScoresBubble()
    {
        for (int i = 0; i < scoreCount - 1; i++)
        {
            for (int j = 0; j < scoreCount - i - 1; j++)
            {
                if (topScores[j].score < topScores[j + 1].score)
                {
                    HighScoreEntry temp = topScores[j];

                    topScores[j] = topScores[j + 1];

                    topScores[j + 1] = temp;
                }
            }
        }
    }

    // =================================================
    // SAVE HIGH SCORES TO FILE
    // =================================================

    void saveHighScores()
    {
        std::ofstream outFile("highscores.txt");

        if (!outFile.is_open())
        {
            return;
        }

        for (int i = 0; i < scoreCount; i++)
        {
            outFile << topScores[i].riderAndVehicle
                    << " "
                    << topScores[i].score
                    << "\n";
        }

        outFile.close();
    }

    // =================================================
    // ADD A NEW HIGH SCORE
    // =================================================

    void addHighScore(std::string riderName, int newScore)
    {
        // Add score if there is space

        if (scoreCount < 10)
        {
            topScores[scoreCount].riderAndVehicle = riderName;
            topScores[scoreCount].score = newScore;

            scoreCount++;
        }

        // If full, replace lowest score only
        else if (newScore > topScores[scoreCount - 1].score)
        {
            topScores[scoreCount - 1].riderAndVehicle = riderName;
            topScores[scoreCount - 1].score = newScore;
        }

        sortHighScoresBubble();

        saveHighScores();
    }

    // =================================================
    // DISPLAY HIGH SCORE LEADERBOARD
    // =================================================

    void drawHighScores(sf::RenderWindow& window,
                        sf::Font& font,
                        float x,
                        float y)
    {
        sf::Text heading;
        heading.setFont(font);
        heading.setString("HIGH SCORES");
        heading.setCharacterSize(28);
        heading.setFillColor(sf::Color::Yellow);
        heading.setPosition(x, y);

        window.draw(heading);

        for (int i = 0; i < scoreCount; i++)
        {
            sf::Text scoreText;

            scoreText.setFont(font);

            scoreText.setString(
                std::to_string(i + 1) + ". " +
                topScores[i].riderAndVehicle + " - " +
                std::to_string(topScores[i].score)
            );

            scoreText.setCharacterSize(20);
            scoreText.setFillColor(sf::Color::White);

            scoreText.setPosition(x, y + 45 + (i * 30));

            window.draw(scoreText);
        }
    }
};

#endif
