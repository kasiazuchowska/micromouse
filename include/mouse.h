#ifndef MOUSE_H
#define MOUSE_H

#include "grid.h"
#include "sense.h"
#include <memory>
#include <vector>
#include <utility>

class Mouse {
public:
    Mouse(Grid* grid, int startX, int startY);
    ~Mouse();
    
    void move();
    int getX()const{return x;}
    int getY()const{return y;}
    int getDirection() const { return direction; }
    const std::vector<std::vector<int>>& getVisited() const { return visited; }
    
private:
    // Current position and direction
    int x, y, direction;
    int width, height;  // Maze dimensions
    
    // Sensors
    std::unique_ptr<VisionSense> vision;
    std::unique_ptr<SmellSense> smell;
    
    // Reference to the grid
    Grid* grid;
    
        // Track visit counts instead of just visited/not visited
    std::vector<std::vector<int>> visited;
    
    // Helper methods
    void updateSensors();
    bool canMoveForward();
    void turnLeft();
    void turnRight();
    void moveForward();
    int findLeastVisitedDirection();
};

#endif // MOUSE_H