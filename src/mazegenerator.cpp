#include "mazegenerator.h"
#include <stack>
#include <utility>
#include <algorithm>
#include <ctime>

MazeGenerator::MazeGenerator() {
    rng.seed(std::time(0));
}

void MazeGenerator::generate(Grid& grid) {
    int width = grid.getWidth();
    int height = grid.getHeight();
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grid.setCell(x, y, new Wall(x, y));
        }
    }
    
    carvePath(grid, 1, 1);
    
    placeGoal(grid);
}

void MazeGenerator::carvePath(Grid& grid, int x, int y) {
    grid.setCell(x, y, new Path(x, y));
    
    int directions[] = {0, 1, 2, 3};
    std::shuffle(directions, directions + 4, rng);
    
    for (int i = 0; i < 4; i++) {
        int dx = 0, dy = 0;
        switch (directions[i]) {
            case 0: dy = -1; break;
            case 1: dx = 1;  break;
            case 2: dy = 1;  break;
            case 3: dx = -1; break;
        }
        
        int nx = x + (dx * 2);
        int ny = y + (dy * 2);
        
        if (nx > 0 && nx < grid.getWidth() - 1 && 
            ny > 0 && ny < grid.getHeight() - 1 && 
            grid.getCell(nx, ny)->isObstacle) {
            
            grid.setCell(x + dx, y + dy, new Path(x + dx, y + dy));
            
            carvePath(grid, nx, ny);
        }
    }
}

void MazeGenerator::placeGoal(Grid& grid) {
    int width = grid.getWidth();
    int height = grid.getHeight();
    
    for (int y = height - 2; y > 0; --y) {
        for (int x = width - 2; x > 0; --x) {
            if (!grid.getCell(x, y)->isObstacle) {
                grid.setCell(x, y, new Goal(x, y));
                grid.setGoal(x,y);
                return;
            }
        }
    }
}