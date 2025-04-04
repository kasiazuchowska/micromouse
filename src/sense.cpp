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
    int goalX = -1, goalY = -1;
    
    for (int cy = 0; cy < grid->getHeight(); ++cy) {
        for (int cx = 0; cx < grid->getWidth(); ++cx) {
            Cell* cell = grid->getCell(cx, cy);
            if (cell && cell->isGoal) {
                goalX = cx;
                goalY = cy;
                break;
            }
        }
        if (goalX != -1) break;
    }
    
    if (goalX == -1) return 0.0; // No goal found
    
    // Calculate Euclidean distance to goal
    double distance = std::sqrt(std::pow(x - goalX, 2) + std::pow(y - goalY, 2));
    
    // Convert to smell intensity (closer = stronger)
    double maxDistance = std::sqrt(std::pow(grid->getWidth(), 2) + std::pow(grid->getHeight(), 2));
    return 1.0 - (distance / maxDistance);
}