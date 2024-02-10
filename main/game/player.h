#ifndef PLAYER_H
#define PLAYER_H

#include <cmath>
#include <vector>
#include <iostream>
#include <ctime>
#include <memory>

#include "map.h"
#include "../math/intersect.h"

class Player
{
private:
    double bobbingAmplitude;
    double bobbingSpeed;

    double acceleration;
    double deceleration;
    double maxVelocity;
    double minVelocity;

    double friction;
    double airFriction;

    double currentSpeed;

    bool isMoving;

public:
    // Constructor
    Player();

    // Function to set rotation
    // void setRotation(float pitch, float yaw, float roll) const;

    // void yawStep(float yaw) const;

    // void setIsMoving(bool moving);

    // void slowDown(bool moving);

    // void rotatePosition(float angle) const;

    // void addPosition(std::vector<float> newPosition) const;

    // void surgeStep(float step);

    // void swayStep(float step);

    // void update(float step);

    // void heaveStep(float step);

    // static bool doLinesIntersect(const std::vector<float> &line1Start, const std::vector<float> &line1End, const std::vector<float> &line2Start, const std::vector<float> &line2End);

    // static bool isEdgeInsideBoundingBox(std::vector<float> &startVertex, std::vector<float> &endVertex, std::vector<std::vector<float>> &playerBox);

    // float getAngleToWall(std::vector<float> &startVertex, std::vector<float> &endVertex) const;

    // bool canMove(std::vector<float> movementVector) const;

    bool isJumping;
    std::unique_ptr<std::vector<float>> velocity;
    std::unique_ptr<std::vector<float>> rotation;
    std::unique_ptr<std::vector<float>> position;
};

#endif // PLAYER_H