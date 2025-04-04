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
    
private:
    int width;
    int height;
    std::vector<std::vector<Cell*>> cells;
};

#endif
