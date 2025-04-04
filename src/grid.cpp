#include "grid.h"

Grid::Grid(int width, int height) : width(width), height(height) {
    cells.resize(height);
    for (int y = 0; y < height; ++y) {
        cells[y].resize(width);
        for (int x = 0; x < width; ++x) {
            cells[y][x] = new Path(x, y);
        }
    }
}

Grid::~Grid() {
    for (auto& row : cells) {
        for (auto& cell : row) {
            delete cell;
        }
    }
}

Cell* Grid::getCell(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return cells[y][x];
    }
    return nullptr;
}

void Grid::setCell(int x, int y, Cell* cell) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        delete cells[y][x];
        cells[y][x] = cell;
    }
}