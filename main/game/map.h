#ifndef MAP_H
#define MAP_H

#include <vector>

struct Sector
{
    int id;
    std::vector<std::vector<float>> vertices;
    float height;
    float bottomOffset;
    std::vector<int> neighbourIds;
    std::vector<int> portalWallsIndices;
};

extern const std::vector<Sector> MAP;

#endif // DOOMDEVICE_MAP_H