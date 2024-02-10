#ifndef MATH_H
#define MATH_H

#include <cmath>
#include <vector>

float distance(const std::vector<float>& a, const std::vector<float>& b);
float dot(const std::vector<float> &a, const std::vector<float> &b);
float norm(const std::vector<float> &v);
float minMax(float value, float min, float max);
float radiansToDegrees(float radians);
std::vector<float> scaleVector(const std::vector<float> &vector, float scale);
std::vector<float> toUnit(const std::vector<float> &vector);
float vectorAngle(const std::vector<float> &vectorA, const std::vector<float> &vectorB);
std::vector<float> intersect(const std::vector<float> &p1a, const std::vector<float> &p1b, const std::vector<float> &p2a, const std::vector<float> &p2b);

#endif 