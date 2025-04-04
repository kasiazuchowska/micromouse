#ifndef SENSE_H
#define SENSE_H

#include "grid.h"

template <typename T>
class Sense {
public:
    virtual T detect() = 0;
    virtual ~Sense() = default;
};

class VisionSense : public Sense<bool> {
public:
    VisionSense(Grid* grid, int x, int y, int direction) 
        : grid(grid), x(x), y(y), direction(direction) {}
        
    bool detect() override;
    
private:
    Grid* grid;
    int x, y;
    int direction; // 0=up, 1=right, 2=down, 3=left
};

class SmellSense : public Sense<double> {
public:
    SmellSense(Grid* grid, int x, int y) 
        : grid(grid), x(x), y(y) {}
    
    double detect() override;
    
private:
    Grid* grid;
    int x, y;
};

#endif // SENSE_H