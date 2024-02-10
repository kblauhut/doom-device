#include "player.h"

Player::Player() :
        acceleration(1),
        deceleration(1),
        maxVelocity(0.5),
        minVelocity(-0.05),
        friction(0.002),
        airFriction(0.00008),
        currentSpeed(0),
        isMoving(false),
        isJumping(false),
        velocity(std::make_unique<std::vector<float>>(std::vector<float>{0.0f, 0.0f, 0.0f})),
        rotation(std::make_unique<std::vector<float>>(std::vector<float>{0.0f, 0.0f, 0.0f})),
        position(std::make_unique<std::vector<float>>(std::vector<float>{0.0f, 0.0f, 1.0f}))
        {}

// void Player::setRotation(float pitch, float yaw, float roll) const {
//     (*rotation)[0] += pitch;
//     (*rotation)[1] += yaw;
//     (*rotation)[2] += roll;
// }

// void Player::yawStep(float yaw) const {
//     (*rotation)[1] += yaw;
// }

// void Player::setIsMoving(bool moving) {
//     isMoving = moving;
// }

// void Player::slowDown(bool moving) {
//     if (!moving) {
//         (*velocity)[0] = (*velocity)[0] > 0 ?
//                          std::max(0.0, (*velocity)[0] - (isJumping ? airFriction : friction)) :
//                          std::min(0.0, (*velocity)[0] + (isJumping ? airFriction : friction));

//         (*velocity)[1] = (*velocity)[1] > 0 ?
//                          std::max(0.0, (*velocity)[1] - (isJumping ? airFriction : friction)) :
//                          std::min(0.0, (*velocity)[1] + (isJumping ? airFriction : friction));

//         (*velocity)[2] = std::max(0.0, (*velocity)[2] - 0.25);
//     }
//     for (int i = 0; i < 3; ++i) {
//         (*position)[i] += (*velocity)[i];
//     }
// }

// void Player::rotatePosition(float angle) const {
//     float radians = float(angle * M_PI) / 180;
//     float cosVal = std::cos(radians);
//     float sinVal = std::sin(radians);

//     float newX = (*position)[0] * cosVal - (*position)[1] * sinVal;
//     float newY = (*position)[0] * sinVal + (*position)[1] * cosVal;

//     (*position)[0] = newX;
//     (*position)[1] = newY;
// }

// void Player::addPosition(std::vector<float> newPosition) const {

//     (*position)[0] = newPosition[0];
//     (*position)[1] = newPosition[1];
//     (*position)[2] = newPosition[2];
// }

// void Player::surgeStep(float step) {
//     if (step > 0) {
//         currentSpeed = std::min(maxVelocity, currentSpeed + acceleration) * 0.4;
//     } else if (step < 0) {
//         currentSpeed = std::max(minVelocity, currentSpeed - deceleration) * 0.4;
//     }

//     std::vector<float> movementVector = {0, 0, 0};
//     movementVector[0] = std::cos((*rotation)[1]) * currentSpeed;
//     movementVector[1] = std::sin((*rotation)[1]) * currentSpeed;
//     movementVector[2] = (*velocity)[2];

//     if (canMove(movementVector)) {
//         *velocity = movementVector;
//     }
// }

// void Player::swayStep(float step) {
//     if (step > 0) {
//         currentSpeed = std::min(maxVelocity, currentSpeed + acceleration) * 0.4;
//     } else if (step < 0) {
//         currentSpeed = std::max(minVelocity, currentSpeed - deceleration) * 0.4;
//     }

//     std::vector<float> movementVector = {0, 0, 0};
//     movementVector[0] = std::cos((*rotation)[1] + M_PI / 2) * currentSpeed;
//     movementVector[1] = std::sin((*rotation)[1] + M_PI / 2) * currentSpeed;
//     movementVector[2] = (*velocity)[2];

//     if (canMove(movementVector)) {
//         *velocity = movementVector;
//     }
// }

// void Player::update(float step) {


//     std::vector<float> movementVector = {0, 0, 0};
//     movementVector[0] = std::cos((*rotation)[1] + M_PI / 2) * currentSpeed;
//     movementVector[1] = std::sin((*rotation)[1] + M_PI / 2) * currentSpeed;
//     movementVector[2] = (*velocity)[2];


//     if (canMove(movementVector)) {
//         *velocity = movementVector;
//     }
// }

// void Player::heaveStep(float step) {
//     (*position)[2] += step;
// }

// bool Player::doLinesIntersect(const std::vector<float> &line1Start, const std::vector<float> &line1End,
//                               const std::vector<float> &line2Start, const std::vector<float> &line2End) {
//     float det = (line1End[0] - line1Start[0]) * (line2End[1] - line2Start[1]) -
//                 (line1End[1] - line1Start[1]) * (line2End[0] - line2Start[0]);

//     if (det == 0) {
//         return false;
//     }

//     float lambda = ((line2End[1] - line2Start[1]) * (line2End[0] - line1Start[0]) +
//                     (line2Start[0] - line2End[0]) * (line2End[1] - line1Start[1])) / det;
//     float gamma = ((line1Start[1] - line1End[1]) * (line2End[0] - line1Start[0]) +
//                    (line1End[0] - line1Start[0]) * (line2End[1] - line1Start[1])) / det;

//     return (0 < lambda && lambda < 1) && (0 < gamma && gamma < 1);
// }

// bool Player::isEdgeInsideBoundingBox(std::vector<float> &startVertex, std::vector<float> &endVertex,
//                                      std::vector<std::vector<float>> &playerBox) {
//     return false;
//     std::vector<std::vector<std::vector<float>>> boxEdges = {
//             {playerBox[2], playerBox[3], playerBox[0], playerBox[1]}, // left
//             {playerBox[0], playerBox[1], playerBox[6], playerBox[7]}, // front
//             {playerBox[7], playerBox[6], playerBox[4], playerBox[5]}, // right
//             {playerBox[5], playerBox[4], playerBox[3], playerBox[2]}  // back
//     };

//     // std::vector<float> edgeToCheck = {startVertex[0], startVertex[1], endVertex[0], endVertex[1]};

//     bool linesIntersect = false;
//     // for (const std::vector<std::vector<float>> &boxEdge: boxEdges) {
//     //     if (doLinesIntersect(startVertex, endVertex, boxEdge[0], boxEdge[1])) {
//     //         linesIntersect = true;
//     //     }
//     // }

//     return linesIntersect;
// }

// float Player::getAngleToWall(std::vector<float> &startVertex, std::vector<float> &endVertex) const {
//     float wallDirectionX = endVertex[0] - startVertex[0];
//     float wallDirectionY = endVertex[1] - startVertex[1];

//     float normalizedWallDirectionX =
//             wallDirectionX / std::sqrt(wallDirectionX * wallDirectionX + wallDirectionY * wallDirectionY);
//     float normalizedWallDirectionY =
//             wallDirectionY / std::sqrt(wallDirectionX * wallDirectionX + wallDirectionY * wallDirectionY);

//     float viewDirectionX = std::cos((*rotation)[1]);
//     float viewDirectionY = std::sin((*rotation)[1]);

//     float dotProduct = normalizedWallDirectionX * viewDirectionX + normalizedWallDirectionY * viewDirectionY;

//     float angle = std::acos(dotProduct);
//     float angleInDegrees = angle * float(180 / M_PI);

//     return angleInDegrees;
// }

// bool Player::canMove(std::vector<float> movementVector) const {
//     float newX = (*position)[0] + movementVector[0];
//     float newY = (*position)[1] + movementVector[1];

//     std::vector<std::vector<float>> playerBox = calculatePlayerBoundingBox({newX, newY}, (*rotation)[1], 3, 3);

//     for (size_t i = 0; i < MAP[0].vertices.size() - 1; i++) {
//         std::vector<float> startVertex = MAP[0].vertices[i];
//         std::vector<float> endVertex = MAP[0].vertices[i + 1];

//         bool isInsidePlayerBox = isEdgeInsideBoundingBox(startVertex, endVertex, playerBox);

//         if (isInsidePlayerBox) {
//             float angleToWall = getAngleToWall(startVertex, endVertex);
//             float angleOffset = 90;
//             float desiredAngle = std::fmod((angleToWall + angleOffset + 360), 360);

//             std::vector<float> newMovement = {0, 0, 0};
//             newMovement[0] = movementVector[0] * 2;
//             newMovement[1] = movementVector[1] * 2;
//             newMovement[2] = movementVector[2];
//             this->addPosition(newMovement);
//             this->rotatePosition(desiredAngle);
//         }
//     }

//     return true;
// }