#ifndef MOUSE_H
#define MOUSE_H

#include "movable.h"
#include "grid.h"
#include "sense.h"
#include <vector>
#include <memory>

class Mouse : public Movable {
public:
    Mouse(Grid* grid, int startX, int startY);
    ~Mouse();
    
    void move() override;
    
    int getX() const { return x; }
    int getY() const { return y; }
    int getDirection() const { return direction; }
    const std::vector<std::vector<bool>>& getVisited() const { return visited; }
    
private:
    Grid* grid;
    int x, y;
    int direction; // 0=up, 1=right, 2=down, 3=left
    std::vector<std::vector<bool>> visited;
    
    std::unique_ptr<Sense<bool>> vision;
    std::unique_ptr<Sense<double>> smell;
    
    void updateSensors();
    bool canMoveForward();
    void turnLeft();
    void turnRight();
    void moveForward();
};

#endif // MOUSE_H