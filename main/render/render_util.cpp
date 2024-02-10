#include <cmath>
#include <vector>
#include <tuple>
#include <memory>

#include "render_util.h"
#include "../math/math.h"
#include "../math/intersect.h"
#include "../io/display.h"

static int SCREEN_HEIGHT_HALF = SCREEN_HEIGHT / 2;

std::vector<float> add(const std::vector<float>& a, const std::vector<float>& b) {
    return std::vector<float>{a[0] + b[0], a[1] + b[1]};
}

std::vector<float> subtract(const std::vector<float>& a, const std::vector<float>& b) {
    return std::vector<float>{a[0] - b[0], a[1] - b[1]};
}

float getScreenX(const std::vector<float>& origin, const std::vector<float>& point, const std::vector<float>& frustumLeft, const std::vector<float>& frustumRight, float width) {
    float fov = vectorAngle(frustumLeft, frustumRight);
    float frustumAngle = vectorAngle(frustumLeft, subtract(point, origin));
    float screenX = (frustumAngle / fov) * width;
    return screenX;
}

float getScreenY(const std::vector<float>& origin, const std::vector<float>& point, float pointHeight, float verticalFov) {
    float originDistance = distance(origin, point);

    float angleSign = (pointHeight < 0) ? -1 : 1;

    std::vector<float> vectorA = {0, 1};
    std::vector<float> vectorB = {pointHeight, originDistance};

    float angle = vectorAngle(vectorA, vectorB) * angleSign;

    float screenY = float(SCREEN_HEIGHT_HALF) - float(SCREEN_HEIGHT_HALF) * (angle / (verticalFov / 2));

    return screenY;
}

std::pair<std::vector<float>, std::vector<float>> clipLineSegmentWithFrustum(std::unique_ptr<std::vector<float>>& origin, const std::vector<float>& pointA, const std::vector<float>& pointB, const std::vector<float>& frustumLeft, const std::vector<float>& frustumRight, float VIEW_DISTANCE) {    
    std::vector<float> intersectionLeftFrustum = getLineSegmentIntersection(*origin, add(*origin, scaleVector(frustumLeft, VIEW_DISTANCE)), pointA, pointB);
    std::vector<float> intersectionRightFrustum = getLineSegmentIntersection(*origin, add(*origin, scaleVector(frustumRight, VIEW_DISTANCE)), pointA, pointB);


    std::vector<float> leftClippedPoint = intersectionLeftFrustum.empty() ? pointA : intersectionLeftFrustum;
    std::vector<float> rightClippedPoint = intersectionRightFrustum.empty() ? pointB : intersectionRightFrustum;

    bool lineIsInFrustum = isLineInFrustum(frustumLeft, frustumRight, subtract(leftClippedPoint, *origin), subtract(rightClippedPoint, *origin));

    if (!lineIsInFrustum) {
        return std::make_pair(std::vector<float>{0.0, 0.0}, std::vector<float>{0.0, 0.0});
    }

    return std::make_pair(leftClippedPoint, rightClippedPoint);
}