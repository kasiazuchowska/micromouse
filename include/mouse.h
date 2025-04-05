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
    int getX()const{return y;}
    int getY()const{return x;}
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
    
    // Updated to track visit count instead of just visited/not visited
    std::vector<std::vector<int>> visited;
    
    // Track deadends to avoid revisiting them
    std::vector<std::vector<bool>> deadends;
    
    // Path history to help with backtracking
    std::vector<std::pair<int, int>> pathHistory;
    
    // Helper methods
    void updateSensors();
    bool canMoveForward();
    void turnLeft();
    void turnRight();
    void moveForward();
    
    // New methods for improved navigation
    std::vector<std::pair<int, int>> getValidNeighbors(int cellX, int cellY);
    std::pair<int, int> findLeastVisitedNeighbor();
    std::vector<std::pair<int, int>> findPathToLeastVisited();
};

#endif // MOUSE_H