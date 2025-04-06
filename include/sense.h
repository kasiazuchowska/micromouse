#ifndef SENSE_H
#define SENSE_H

#include "grid.h"

/// @brief szablon klasy zmyslu
/// @tparam T typ zwracany przez metode detect
template <typename T>
class Sense {
public:
    /// @brief uzycie zmyslu
    /// @return wynik dzialania zmyslu
    virtual T detect() = 0;
    virtual ~Sense() = default;
};

class VisionSense : public Sense<bool> {
public:
    VisionSense(Grid* grid, int x, int y, int direction) 
        : grid(grid), x(x), y(y), direction(direction) {}
        
    /// @brief sprawdza czy w kierunku w ktorym jest skierowana mysz znajduje sie przeszkoda
    /// @return czy przed mysza jest przeszkoda
    bool detect() override;
    
private:
    Grid* grid;
    int x, y;
    int direction;
};

class SmellSense : public Sense<double> {
public:
    SmellSense(Grid* grid, int x, int y) 
        : grid(grid), x(x), y(y) {}
    
    /// @brief sprawdza w jakiej odleglosci od myszy jest przeszkoda
    /// @return "intensywnosc zapachu" stosunek maksymalnej odleglosci od celu do rzeczywistej
    double detect() override;
    
private:
    Grid* grid;
    int x, y;
};

#endif