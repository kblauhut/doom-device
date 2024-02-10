#include "portal_renderer.h"

void renderPortal(Player& player, Portal& portal, std::vector<Portal>& portalQueue) {
    std::vector<float>& frustumLeft = portal.frustumLeft;
    std::vector<float>& frustumRight = portal.frustumRight;
    Sector& sector = *portal.sector;
    std::vector<std::vector<float>>& renderBoundBottom = portal.renderBoundBottom;
    std::vector<std::vector<float>>& renderBoundTop = portal.renderBoundTop;
    int previousSectorId = portal.previousSectorId;

    float width = (renderBoundTop[1][0] - renderBoundTop[0][0]);
    float aspectRatio = width / float(SCREEN_HEIGHT);
    float fov = vectorAngle(frustumLeft, frustumRight);
    float verticalFov = fov / aspectRatio;
    float screenXOffset = renderBoundTop[0][0];


    std::vector<std::vector<std::vector<float>>> walls;
    for (size_t index = 1; index < sector.vertices.size(); ++index) {
        std::vector<float>& vertex = sector.vertices[index];
        std::vector<float>& prevVertex = sector.vertices[index - 1];
        std::vector<std::vector<float>> newWall(2);
        newWall[0] = prevVertex;
        newWall[1] = vertex;
        walls.push_back(newWall);
    }

    // draw room
    for (size_t index = 0; index < walls.size(); index++) {
        std::vector<std::vector<float>>& wall = walls[index];
        std::vector<float>& vertexA = wall[0];
        std::vector<float>& vertexB = wall[1];

        std::pair<std::vector<float>, std::vector<float>> wallPoints = clipLineSegmentWithFrustum(player.position, vertexA, vertexB, frustumLeft, frustumRight, VIEW_DISTANCE);

        //should be the same as if (!wallPoints) continue;
        if (wallPoints.first[0] == 0.0 && wallPoints.first[1] == 0.0 && 
                wallPoints.second[0] == 0.0 && wallPoints.second[1] == 0.0
        ) {continue;}

        auto& [leftWallPoint, rightWallPoint] = wallPoints;

        float lX = screenXOffset + getScreenX(*player.position, leftWallPoint, frustumLeft, frustumRight, width);
        float rX = screenXOffset + getScreenX(*player.position, rightWallPoint, frustumLeft, frustumRight, width);

        float lTopY = getScreenY(*player.position, leftWallPoint, sector.height - (*player.position)[2], verticalFov);
        float lBottomY = getScreenY(*player.position, leftWallPoint, sector.bottomOffset - (*player.position)[2], verticalFov);
        float rTopY = getScreenY(*player.position, rightWallPoint, sector.height - (*player.position)[2], verticalFov);
        float rBottomY = getScreenY(*player.position, rightWallPoint, sector.bottomOffset - (*player.position)[2], verticalFov);


        size_t portalIndex = -1;
        for (size_t i = 0; i < sector.portalWallsIndices.size(); i++) {
            if (sector.portalWallsIndices[i] == index) {
                portalIndex = i;
                break;
            }
        }

        if (portalIndex != -1) {
            auto& nextSector = *std::find_if(MAP.begin(), MAP.end(), [&](const auto& sec) { return sec.id == sector.neighbourIds[portalIndex]; });

            if (nextSector.id == previousSectorId) continue;

            float lTopYPortal = getScreenY(*player.position, leftWallPoint, nextSector.height - (*player.position)[2], verticalFov);
            float lBottomYPortal = getScreenY(*player.position, leftWallPoint, nextSector.bottomOffset - (*player.position)[2], verticalFov);
            float rTopYPortal = getScreenY(*player.position, rightWallPoint, nextSector.height - (*player.position)[2], verticalFov);
            float rBottomYPortal = getScreenY(*player.position, rightWallPoint, nextSector.bottomOffset - (*player.position)[2], verticalFov);

            float currLeftFrustumAngle = std::atan2(frustumLeft[1], frustumLeft[0]);
            float frustumToLeft = vectorAngle(frustumLeft, subtract(leftWallPoint, *player.position));
            float frustumToRight = vectorAngle(frustumLeft, subtract(rightWallPoint, *player.position));

            float newFrustumL = frustumToLeft + currLeftFrustumAngle;
            float newFrustumR = frustumToRight + currLeftFrustumAngle;

            int prevSectorId = sector.id;
            std::vector<float> nextFrustumLeft = {std::cos(newFrustumL), std::sin(newFrustumL)};
            std::vector<float> nextFrustumRight = {std::cos(newFrustumR), std::sin(newFrustumR)};
            std::vector<std::vector<float>> nextRenderBoundTop = {
                    {lX, std::max(lTopYPortal, lTopY)},
                    {rX, std::max(rTopYPortal, rTopY)}
            };
            std::vector<std::vector<float>> nextRenderBoundBottom ={
                    {lX, std::min(lBottomYPortal, lBottomY)},
                    {rX, std::min(rBottomYPortal, rBottomY)}
            };


            Portal nextPortal = {
                    const_cast<Sector *>(&nextSector),
                    prevSectorId,
                    nextFrustumLeft,
                    nextFrustumRight,
                    nextRenderBoundTop,
                    nextRenderBoundBottom
            };

            portalQueue.push_back(nextPortal);
        }

        std::vector<std::vector<float>> ceilTop = {
                {lX, 0},
                {rX, 0}
        };
        std::vector<std::vector<float>> ceilBottom = {
                {lX, lTopY},
                {rX, rTopY}
        };

        std::vector<std::vector<float>> floorTop = {
                {lX, lBottomY},
                {rX, rBottomY}
        };
        std::vector<std::vector<float>> floorBottom = {
                {lX, float(SCREEN_HEIGHT)},
                {rX, float(SCREEN_HEIGHT)}
        };

        int ceilColorIntensity = 120 + sector.bottomOffset * 10;
        rasterizeParallelogramInBounds(ceilTop, ceilBottom, renderBoundTop, renderBoundBottom, rgbColor(ceilColorIntensity, ceilColorIntensity, ceilColorIntensity), false);

        float wallAngle = vectorAngle(vertexA, vertexB);
        int wallColor = rgbColor(255 * (0.5 - wallAngle / (M_PI / 2)), 255 * (0.5 - wallAngle / (M_PI / 2)), 255);
        rasterizeParallelogramInBounds(ceilBottom, floorTop, renderBoundTop, renderBoundBottom, wallColor, true);

        int floorColorIntensity = 200 + sector.bottomOffset * 10;
        rasterizeParallelogramInBounds(floorTop, floorBottom, renderBoundTop, renderBoundBottom, rgbColor(floorColorIntensity, floorColorIntensity, floorColorIntensity), false);
    }
}