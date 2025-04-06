#include "sense.h"
#include <cmath>

bool VisionSense::detect() {
    int dx = 0, dy = 0;
    
    switch (direction) {
        case 0: dy = -1; break;
        case 1: dx = 1; break;
        case 2: dy = 1; break;
        case 3: dx = -1; break;
    }
    
    Cell* cell = grid->getCell(x + dx, y + dy);
    return cell && !cell->isObstacle;
}

double SmellSense::detect() {
    Cell* goal = grid->getGoal();
    
    double distance = std::sqrt(std::pow(x - goal->x, 2) + std::pow(y - goal->y, 2));
    
    double maxDistance = std::sqrt(std::pow(grid->getWidth(), 2) + std::pow(grid->getHeight(), 2));
    return 1.0 - (distance / maxDistance);
}