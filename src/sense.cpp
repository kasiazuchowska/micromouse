#include "sense.h"
#include <cmath>

bool VisionSense::detect() {
    int dx = 0, dy = 0;
    
    // Convert direction to delta
    switch (direction) {
        case 0: dy = -1; break; // up
        case 1: dx = 1; break;  // right
        case 2: dy = 1; break;  // down
        case 3: dx = -1; break; // left
    }
    
    // Check the cell ahead
    Cell* cell = grid->getCell(x + dx, y + dy);
    return cell && !cell->isObstacle;
}

double SmellSense::detect() {
    // Find the goal cell
    Cell* goal = grid->getGoal();
    
    // Calculate Euclidean distance to goal
    double distance = std::sqrt(std::pow(x - goal->x, 2) + std::pow(y - goal->y, 2));
    
    // Convert to smell intensity (closer = stronger)
    double maxDistance = std::sqrt(std::pow(grid->getWidth(), 2) + std::pow(grid->getHeight(), 2));
    return 1.0 - (distance / maxDistance);
}