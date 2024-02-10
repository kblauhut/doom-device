#include <cmath>
#include <vector>
#include <iostream>

#include "math.h"

float distance(const std::vector<float>& a, const std::vector<float>& b) {
    float dx = a[0] - b[0];
    float dy = a[1] - b[1];
    return std::sqrt(dx * dx + dy * dy);
}

std::vector<float> add1(const std::vector<float> &a, const std::vector<float> &b) {
    std::vector<float> result;
    result.reserve(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result.push_back(a[i] + b[i]);
    }
    return result;
}

std::vector<float> subtract1(const std::vector<float> &a, const std::vector<float> &b) {
    std::vector<float> result;
    result.reserve(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result.push_back(a[i] - b[i]);
    }
    return result;
}

std::vector<float> multiplyScalar(const std::vector<float> &vector, float scalar) {
    std::vector<float> result;
    result.reserve(vector.size());
    for (float v: vector) {
        result.push_back(v * scalar);
    }
    return result;
}

// Define your own dot and norm functions if not available
float dot(const std::vector<float> &a, const std::vector<float> &b) {
    float result = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        result += a[i] * b[i];
    }
    return result;
}

float norm(const std::vector<float> &v) {
    float result = 0.0;
    for (float val: v) {
        result += val * val;
    }
    return std::sqrt(result);
}

// Clamp a value between a minimum and maximum value
float minMax(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Convert radians to degrees
float radiansToDegrees(float radians) {
    return radians * float(180.0 / M_PI);
}

// Scale a vector by a scalar
std::vector<float> scaleVector(const std::vector<float> &vector, float scale) {
    std::vector<float> result;
    result.reserve(vector.size());
    for (float v: vector) {
        result.push_back(v * scale);
    }
    return result;
}

std::vector<float> intersect(const std::vector<float> &p1a, const std::vector<float> &p1b, const std::vector<float> &p2a, const std::vector<float> &p2b) {
    std::vector<float> o1 = p1a;
    std::vector<float> o2 = p2a;
    std::vector<float> d1 = subtract1(o1, p1b);
    std::vector<float> d2 = subtract1(o2, p2b);
    float det = d1[0] * d2[1] - d2[0] * d1[1];
    if (std::abs(det) < 0.005) return std::vector<float>{};
    float d20o11 = d2[0] * o1[1];
    float d21o10 = d2[1] * o1[0];
    float d20o21 = d2[0] * o2[1];
    float d21o20 = d2[1] * o2[0];
    float t = (((d20o11 - d21o10) - d20o21) + d21o20) / det;
    return add1(multiplyScalar(d1, t), o1);
}

// Convert a vector to its unit vector
std::vector<float> toUnit(const std::vector<float> &vector) {
    float normValue = norm(vector);
    std::vector<float> result;
    result.reserve(vector.size());
    for (float v: vector) {
        result.push_back(v / normValue);
    }
    return result;
}

// Calculate the angle between two vectors in radians
float vectorAngle(const std::vector<float> &vectorA, const std::vector<float> &vectorB) {
    float dotProduct = dot(vectorA, vectorB);
    float normA = norm(vectorA);
    float normB = norm(vectorB);

    return std::acos(minMax(dotProduct / (normA * normB), -1.0, 1.0));
}

