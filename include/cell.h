#ifndef CELL_H
#define CELL_H

/// @brief Bazowa klasa reprezentująca komorke
class Cell {
public:
    /// @brief konstruktor
    /// @param x 
    /// @param y 
    /// @param isObstacle 
    /// @param isGoal 
    Cell(int x, int y, bool isObstacle = false, bool isGoal = false)
        : x(x), y(y), isObstacle(isObstacle), isGoal(isGoal) {}
    
    virtual ~Cell() = default;
    
    int x;
    int y;
    bool isObstacle;
    bool isGoal;
};

/// @brief Klasa reprezsentująca ściane
class Wall : public Cell {
public:
    Wall(int x, int y) : Cell(x, y, true, false) {}
};

/// @brief Klasa reprezsentująca ścieżkę
class Path : public Cell {
public:
    Path(int x, int y) : Cell(x, y, false, false) {}
};

/// @brief Klasa reprezsentująca cel
class Goal : public Cell {
public:
    Goal(int x, int y) : Cell(x, y, false, true) {}
};

#endif
