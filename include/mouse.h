#ifndef MOUSE_H
#define MOUSE_H

#include "grid.h"
#include "sense.h"
#include <memory>
#include <vector>
#include <utility>

class Mouse {
public:
    /// @brief konstruktor, inicjalizuje pozycje myszy, odwiedzone miejsca, zmysly
    /// @param grid siatka
    /// @param startX poczatkowa pozycja myszy
    /// @param startY poczatkowa pozycja myszy
    Mouse(Grid* grid, int startX, int startY);
    ~Mouse();
    
    /// @brief symulacja jednego ruchu myszy
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
    
    /// @brief aktualizuje wartosci zmyslow
    void updateSensors();
    /// @brief uzywa wzroku do sprawdzenia czy mozna poruszyc sie w przod
    /// @returnczy mozna poruszyc sie w przod
    bool canMoveForward();
    /// @brief skret w lewo (zmiana kierunku)
    void turnLeft();
    /// @brief skret w prawo (zmiana kierunku)
    void turnRight();
    /// @brief poruszenie w przod jezeli jest to moziwe
    void moveForward();
    /// @brief szuka kierunku ktory zostal odwiedzony najmniejsza ilosc razy
    /// @return numer kierunku (1-gora, 2-prawo, 3-dol, 4-lewo)
    int findLeastVisitedDirection();
};

#endif