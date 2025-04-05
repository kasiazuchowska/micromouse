#ifndef GRID_H
#define GRID_H

#include <vector>
#include "cell.h"

class Grid {
public:
    Grid(int width, int height);
    ~Grid();
    
    Cell* getCell(int x, int y);
    void setCell(int x, int y, Cell* cell);
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    Cell* getGoal();
    void setGoal(int x, int y);
    
private:
    int width;
    int height;
    int goalX;
    int goalY;
    std::vector<std::vector<Cell*>> cells;
};

#endif
