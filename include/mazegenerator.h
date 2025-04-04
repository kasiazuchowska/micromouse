#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H

#include "grid.h"
#include <random>

class MazeGenerator {
public:
    MazeGenerator();
    void generate(Grid& grid);
    void carvePath(Grid& grid, int x, int y);
    void placeGoal(Grid& grid);
    
private:
    std::mt19937 rng;
};

#endif