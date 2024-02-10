#ifndef INTERSECT_H
#define INTERSECT_H

#include <cmath>

namespace mathjs
{
    float distance(const std::vector<float> &a, const std::vector<float> &b);
    float dot(const std::vector<float> &a, const std::vector<float> &b);
    std::vector<float> intersect(const std::vector<float> &a, const std::vector<float> &b,
                                 const std::vector<float> &c, const std::vector<float> &d);
}

bool isPointOnLine(const std::vector<float> &lineVectorA, const std::vector<float> &lineVectorB,
                   const std::vector<float> &point);

std::vector<float> getLineSegmentIntersection(std::vector<float> lineAStart,
                                              const std::vector<float> &lineAEnd,
                                              const std::vector<float> &lineBStart,
                                              const std::vector<float> &lineBEnd);

bool isPointInFrustum(const std::vector<float> &leftFrustum, const std::vector<float> &rightFrustum,
                      const std::vector<float> &point);

bool isLineInFrustum(const std::vector<float> &leftFrustum, const std::vector<float> &rightFrustum,
                     const std::vector<float> &pointA, const std::vector<float> &pointB);

std::vector<float> rotatePoint(const std::vector<float> &point, float angle,
                               const std::vector<float> &center);

std::vector<std::vector<float>> calculatePlayerBoundingBox(const std::vector<float> &playerPosition,
                                                           float playerYaw,
                                                           float width,
                                                           float height);

bool isPointInPolygon(const std::vector<std::vector<float>> &polygon, const std::vector<float> &point);

std::vector<float> getWallIntersectionPoint(int sectorId,
                                            const std::vector<float> &playerPosition,
                                            const std::vector<float> &rotationVector);

#endif
