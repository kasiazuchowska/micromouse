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
    int x, y, direction;
    int width, height;

    std::unique_ptr<VisionSense> vision;
    std::unique_ptr<SmellSense> smell;

    Grid* grid;
    
    std::vector<std::vector<int>> visited;
    
    void updateSensors();
    bool canMoveForward();
    void turnLeft();
    void turnRight();
    void moveForward();
    int findLeastVisitedDirection();
};

#endif