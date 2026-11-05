#ifndef ORDER_H
#define ORDER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

#include "Constants.h"
#include "Map.h"
#include "Player.h"

// =====================================================
// ABDUL BASEER KHAN - ORDER STRUCT
// =====================================================

struct Order
{
    std::string shopName;
    std::string customerName;

    int pickupRow;
    int pickupCol;

    int dropRow;
    int dropCol;

    float timeLimit;
    float timeRemaining;

    int baseReward;

    bool packagePickedUp;
};

// =====================================================
// ORDER MANAGER
// =====================================================

class OrderManager
{
public:

    std::vector<Order> orders;

    int currentOrderIndex;

    int score;
    int completedCount;
    int failedCount;
    int streakCount;

    std::string statusBanner;

    // =================================================
    // CONSTRUCTOR
    // =================================================

    OrderManager()
    {
        resetOrders();
    }

    // =================================================
    // RESET AND INITIALIZE SIX ORDERS
    // =================================================

    void resetOrders()
    {
        orders.clear();

        currentOrderIndex = 0;

        score = 0;
        completedCount = 0;
        failedCount = 0;
        streakCount = 0;

        statusBanner =
            "Drive to the GREEN Pickup Marker (Pizza Shop)!";

        // ORDER 1
        orders.push_back({
            "Pizza Shop",
            "House 15",
            2, 4,
            6, 15,
            100.0f,
            100.0f,
            100,
            false
        });

        // ORDER 2
        orders.push_back({
            "Burger Hub",
            "House 10",
            7, 2,
            8, 10,
            110.0f,
            110.0f,
            150,
            false
        });

        // ORDER 3
        orders.push_back({
            "Coffee Corner",
            "House 18",
            12, 10,
            8, 18,
            100.0f,
            100.0f,
            200,
            false
        });

        // ORDER 4
        orders.push_back({
            "Pizza Shop",
            "House 2",
            2, 4,
            11, 2,
            95.0f,
            95.0f,
            250,
            false
        });

        // ORDER 5
        orders.push_back({
            "Burger Hub",
            "House 17",
            7, 2,
            13, 17,
            90.0f,
            90.0f,
            300,
            false
        });

        // ORDER 6
        orders.push_back({
            "Coffee Corner",
            "House 15",
            12, 16,
            6, 15,
            85.0f,
            85.0f,
            400,
            false
        });
    }

    // =================================================
    // CHECK WHETHER ALL ORDERS ARE FINISHED
    // =================================================

    bool allOrdersCompleted() const
    {
        return currentOrderIndex >= (int)orders.size();
    }

    // =================================================
    // GET CURRENT ACTIVE ORDER
    // =================================================

    Order* getCurrentOrder()
    {
        if (allOrdersCompleted())
        {
            return nullptr;
        }

        return &orders[currentOrderIndex];
    }

    // =================================================
    // UPDATE ORDER TIMER
    // =================================================

    void updateTimer(float deltaTime)
    {
        if (allOrdersCompleted())
        {
            return;
        }

        Order& currentOrder = orders[currentOrderIndex];

        currentOrder.timeRemaining -= deltaTime;

        if (currentOrder.timeRemaining < 0)
        {
            currentOrder.timeRemaining = 0;
        }
    }

    // =================================================
    // CHECK IF CURRENT ORDER HAS EXPIRED
    // =================================================

    bool isOrderExpired() const
    {
        if (allOrdersCompleted())
        {
            return false;
        }

        return orders[currentOrderIndex].timeRemaining <= 0;
    }

    // =================================================
    // FAIL CURRENT ORDER
    // =================================================

    void failCurrentOrder()
    {
        if (allOrdersCompleted())
        {
            return;
        }

        failedCount++;
        streakCount = 0;

        currentOrderIndex++;

        statusBanner = "Order Failed! Time is over.";

        if (failedCount >= 3)
        {
            statusBanner = "GAME OVER! Too many failed orders.";
        }
        else if (allOrdersCompleted())
        {
            statusBanner = "All Orders Finished!";
        }
        else
        {
            statusBanner = "Next order! Find the pickup shop.";
        }
    }

    // =================================================
    // COMPLETE CURRENT ORDER
    // =================================================

    void completeCurrentOrder(float multiplier)
    {
        if (allOrdersCompleted())
        {
            return;
        }

        Order& currentOrder = orders[currentOrderIndex];

        score += (int)(currentOrder.baseReward * multiplier);

        completedCount++;
        streakCount++;

        currentOrderIndex++;

        statusBanner = "Delivery Successful!";

        if (allOrdersCompleted())
        {
            statusBanner = "VICTORY! All Orders Completed.";
        }
        else
        {
            statusBanner = "Great Job! Find the next pickup shop.";
        }
    }

    // =================================================
    // GET NUMBER OF REMAINING ORDERS
    // =================================================

    int getRemainingOrders() const
    {
        return (int)orders.size() - currentOrderIndex;
    }

};

#endif
