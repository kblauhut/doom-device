#ifndef RENDER_UTIL_H
#define RENDER_UTIL_H

std::vector<float> add(const std::vector<float>& a, const std::vector<float>& b);
std::vector<float> subtract(const std::vector<float>& a, const std::vector<float>& b);
float distance(const std::vector<float>& a, const std::vector<float>& b);

float getScreenX(const std::vector<float>& origin, const std::vector<float>& point, const std::vector<float>& frustumLeft, const std::vector<float>& frustumRight, float width);
float getScreenY(const std::vector<float>& origin, const std::vector<float>& point, float pointHeight, float verticalFov);

std::pair<std::vector<float>, std::vector<float>> clipLineSegmentWithFrustum(std::unique_ptr<std::vector<float>>& origin, const std::vector<float>& pointA, const std::vector<float>& pointB, const std::vector<float>& frustumLeft, const std::vector<float>& frustumRight, float VIEW_DISTANCE);

#endif  