#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H

#include "grid.h"
#include <random>

class MazeGenerator {
public:
    /// @brief ustawia liczbe losowa
    MazeGenerator();
    /// @brief tworzy labirynt na siatce
    /// @param grid 
    void generate(Grid& grid);
    /// @brief generuje sciezke dlugosci 2 w losowym kierunku, i wywoluje sie rekurencyjnie korzystajac z dfs
    /// @param grid siatka
    /// @param x punkt poczatkowy
    /// @param y punkt poczatkowy
    void carvePath(Grid& grid, int x, int y);
    /// @brief ustawia cel w mozliwie najdalszym punkcie od myszy
    /// @param grid 
    void placeGoal(Grid& grid);
    
private:
    std::mt19937 rng;
};

#endif