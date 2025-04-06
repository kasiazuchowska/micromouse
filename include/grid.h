#ifndef GRID_H
#define GRID_H

#include <vector>
#include "cell.h"

class Grid {
public:
    /// @brief konstruktor, ustawia wielkosc oraz inicjalizuje wszytskie komorki jako sciezka
    /// @param width 
    /// @param height 
    Grid(int width, int height);

    /// @brief destruktor, usuwa wszytskie obiekty komorek
    ~Grid();
    
    /// @brief zwraca odpowiednia komorke jezeli taka istnieje
    /// @param x 
    /// @param y 
    /// @return 
    Cell* getCell(int x, int y);

    /// @brief ustawia dana komorke na odpowiedni typ (pochodna klasy cell)
    /// @param x 
    /// @param y 
    /// @param cell 
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
