#ifndef CELL_H
#define CELL_H

class Cell {
public:
    Cell(int x, int y, bool isObstacle = false, bool isGoal = false)
        : x(x), y(y), isObstacle(isObstacle), isGoal(isGoal) {}
    
    virtual ~Cell() = default;
    
    int x;
    int y;
    bool isObstacle;
    bool isGoal;
};

class Wall : public Cell {
public:
    Wall(int x, int y) : Cell(x, y, true, false) {}
};

class Path : public Cell {
public:
    Path(int x, int y) : Cell(x, y, false, false) {}
};

class Goal : public Cell {
public:
    Goal(int x, int y) : Cell(x, y, false, true) {}
};

#endif
