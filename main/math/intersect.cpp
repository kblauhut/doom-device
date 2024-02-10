
#include <cmath>
#include <vector>
#include "intersect.h"
#include "math.h"
#include "../game/map.h"

namespace {
    const float epsilon = 0.005;
}

bool isPointOnLine(const std::vector<float>& lineVectorA, const std::vector<float>& lineVectorB,
                   const std::vector<float>& point) {
    float aToPoint = distance(lineVectorA, point);
    float bToPoint = distance(lineVectorB, point);
    float aToB = distance(lineVectorA, lineVectorB);

    return (aToPoint + bToPoint - aToB) < epsilon;
}

std::vector<float> getLineSegmentIntersection(std::vector<float> lineAStart,
                                              const std::vector<float>& lineAEnd,
                                              const std::vector<float>& lineBStart,
                                              const std::vector<float>& lineBEnd) {
    std::vector<float> intersectionPoint = intersect(lineAStart, lineAEnd, lineBStart, lineBEnd);

    if (intersectionPoint.empty()) return {};

    // Check if the intersection point is on the line segments
    bool isOnLineA = isPointOnLine(lineAStart, lineAEnd, intersectionPoint);
    bool isOnLineB = isPointOnLine(lineBStart, lineBEnd, intersectionPoint);

    if (!isOnLineA || !isOnLineB) return {};

    return intersectionPoint;
}

bool isPointInFrustum(const std::vector<float>& leftFrustum, const std::vector<float>& rightFrustum,
                      const std::vector<float>& point) {
    std::vector<float> leftFrustumPerpendicular = {-leftFrustum[1], leftFrustum[0]};
    std::vector<float> rightFrustumPerpendicular = {rightFrustum[1], -rightFrustum[0]};

    float dotLeft = dot(leftFrustumPerpendicular, point);
    float dotRight = dot(rightFrustumPerpendicular, point);

    return (dotLeft > epsilon) && (dotRight > epsilon);
}

bool isLineInFrustum(const std::vector<float>& leftFrustum, const std::vector<float>& rightFrustum,
                     const std::vector<float>& pointA, const std::vector<float>& pointB) {
    std::vector<float> leftFrustumPerpendicular = {-leftFrustum[1], leftFrustum[0]};
    std::vector<float> rightFrustumPerpendicular = {rightFrustum[1], -rightFrustum[0]};

    float dotARight = dot(rightFrustumPerpendicular, pointA);
    float dotBLeft = dot(leftFrustumPerpendicular, pointB);

    return (dotARight > epsilon) && (dotBLeft > epsilon);
}

std::vector<float> rotatePoint(const std::vector<float>& point, float angle,
                                const std::vector<float>& center) {
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);

    std::vector<float> translatedPoint = {point[0] - center[0], point[1] - center[1]};
    std::vector<float> rotatedPoint = {
            translatedPoint[0] * cosA - translatedPoint[1] * sinA,
            translatedPoint[0] * sinA + translatedPoint[1] * cosA
    };

    return {rotatedPoint[0] + center[0], rotatedPoint[1] + center[1]};
}

std::vector<std::vector<float>> calculatePlayerBoundingBox(const std::vector<float>& playerPosition,
                                                            float playerYaw,
                                                            float width = 3,
                                                            float height = 3) {
    std::vector<float> center = playerPosition;
    std::vector<std::vector<float>> corners = {
            {center[0] - width / 2, center[1] - height / 2},
            {center[0] + width / 2, center[1] - height / 2},
            {center[0] + width / 2, center[1] + height / 2},
            {center[0] - width / 2, center[1] + height / 2}
    };

    std::vector<std::vector<float>> rotatedCorners;
    rotatedCorners.reserve(corners.size());
    for (const auto& corner : corners) {
        rotatedCorners.push_back(rotatePoint(corner, playerYaw, center));
    }

    return rotatedCorners;
}

bool isPointInPolygon(const std::vector<std::vector<float>>& polygon, const std::vector<float>& point) {
    bool inside = false;
    for (size_t i = 0, j = polygon.size() - 1; i < polygon.size(); j = i++) {
        float xi = polygon[i][0];
        float yi = polygon[i][1];
        float xj = polygon[j][0];
        float yj = polygon[j][1];

        bool intersect = (yi > point[1]) != (yj > point[1]) &&
                         point[0] < ((xj - xi) * (point[1] - yi)) / (yj - yi) + xi;
        if (intersect) inside = !inside;
    }

    return inside;
}

std::vector<float> getWallIntersectionPoint(int sectorId,
                                             const std::vector<float>& playerPosition,
                                             const std::vector<float>& rotationVector) {
    for (const auto& sector : MAP) {
        if (sector.id == sectorId) {
            for (size_t i = 1; i < sector.vertices.size(); ++i) {
                const std::vector<float>& vertexA = sector.vertices[i - 1];
                const std::vector<float>& vertexB = sector.vertices[i];

                std::vector<float> vertexAPlayer = {vertexA[0] - playerPosition[0],
                                                     vertexA[1] - playerPosition[1]};
                std::vector<float> vertexBPlayer = {vertexB[0] - playerPosition[0],
                                                     vertexB[1] - playerPosition[1]};

                float rotationVectorX = rotationVector[0];
                float rotationVectorY = rotationVector[1];
                std::vector<float> rotationVectorNormal = {rotationVectorY, -rotationVectorX};

                // Half space tests wall points between the rotation vector
                bool aInNormalDir = dot(rotationVectorNormal, vertexAPlayer) > 0;
                bool bInNormalDir = dot(rotationVectorNormal, vertexBPlayer) > 0;

                // Half space test, wall is in behind the rotation vector
                bool wallBehindRotationVec = dot(vertexAPlayer, rotationVector) < 0;

                if ((aInNormalDir && bInNormalDir) ||
                    (!aInNormalDir && !bInNormalDir) ||
                    wallBehindRotationVec) {
                    continue;
                }

                int portalWallIndex = -1;
                for (int j = 0; j < sector.portalWallsIndices.size(); ++j) {
                    if (sector.portalWallsIndices[j] == i - 1) {
                        portalWallIndex = j;
                        break;
                    }
                }

                if (portalWallIndex != -1) {
                    return getWallIntersectionPoint(sector.neighbourIds[portalWallIndex],
                                                    playerPosition, rotationVector);
                }

                return intersect({0, 0}, rotationVector, vertexAPlayer, vertexBPlayer);
            }
        }
    }

    return {};
}