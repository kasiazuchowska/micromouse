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
    
    // Fill grid with walls
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grid.setCell(x, y, new Wall(x, y));
        }
    }
    
    // Start carving paths from position (1,1)
    carvePath(grid, 1, 1);
    
    // Place goal at far end of maze
    placeGoal(grid);
}

void MazeGenerator::carvePath(Grid& grid, int x, int y) {
    // Convert current cell to path
    grid.setCell(x, y, new Path(x, y));
    
    // Define possible directions (up, right, down, left)
    int directions[] = {0, 1, 2, 3};
    std::shuffle(directions, directions + 4, rng);
    
    // Try each direction in random order
    for (int i = 0; i < 4; i++) {
        int dx = 0, dy = 0;
        switch (directions[i]) {
            case 0: dy = -1; break; // Up
            case 1: dx = 1;  break; // Right
            case 2: dy = 1;  break; // Down
            case 3: dx = -1; break; // Left
        }
        
        // Calculate position two cells away
        int nx = x + (dx * 2);
        int ny = y + (dy * 2);
        
        // Check if position is valid and unvisited
        if (nx > 0 && nx < grid.getWidth() - 1 && 
            ny > 0 && ny < grid.getHeight() - 1 && 
            grid.getCell(nx, ny)->isObstacle) {
            
            // Carve path by removing wall between cells
            grid.setCell(x + dx, y + dy, new Path(x + dx, y + dy));
            
            // Continue carving from new position (recursive DFS)
            carvePath(grid, nx, ny);
        }
    }
}

void MazeGenerator::placeGoal(Grid& grid) {
    int width = grid.getWidth();
    int height = grid.getHeight();
    
    // Try to place goal at bottom-right corner first
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