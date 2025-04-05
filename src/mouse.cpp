#include "mouse.h"
#include <algorithm>
#include <random>
#include <queue>
#include <utility>
#include <cmath>

Mouse::Mouse(Grid* grid, int startX, int startY)
    : grid(grid), x(startX), y(startY), direction(1) {
    // Initialize visited array
    visited.resize(grid->getHeight());
    for (int y = 0; y < grid->getHeight(); ++y) {
        visited[y].resize(grid->getWidth(), 0);  // Changed to int for visit count
    }
    
    // Mark starting position as visited
    visited[y][x] = 1;
    
    // Initialize path history
    pathHistory.push_back({x, y});
    
    // Initialize sensors
    updateSensors();
    
    // Store the maze size
    width = grid->getWidth();
    height = grid->getHeight();
    
    // Initialize deadends map
    deadends.resize(height);
    for (int y = 0; y < height; ++y) {
        deadends[y].resize(width, false);
    }
}

Mouse::~Mouse() {
}

void Mouse::updateSensors() {
    vision = std::make_unique<VisionSense>(grid, x, y, direction);
    smell = std::make_unique<SmellSense>(grid, x, y);
}

bool Mouse::canMoveForward() {
    return vision->detect();
}

void Mouse::turnLeft() {
    direction = (direction + 3) % 4;
    updateSensors();
}

void Mouse::turnRight() {
    direction = (direction + 1) % 4;
    updateSensors();
}

void Mouse::moveForward() {
    if (canMoveForward()) {
        // Convert direction to delta
        int dx = 0, dy = 0;
        switch (direction) {
            case 0: dy = -1; break; // up
            case 1: dx = 1; break;  // right
            case 2: dy = 1; break;  // down
            case 3: dx = -1; break; // left
        }
        
        x += dx;
        y += dy;
        visited[y][x]++; // Increment visit count
        pathHistory.push_back({x, y});
        
        // If we've visited this cell too many times, mark it as potentially problematic
        if (visited[y][x] > 3) {
            deadends[y][x] = true;
        }
        
        updateSensors();
    }
}

// Get neighboring cells that are valid moves
std::vector<std::pair<int, int>> Mouse::getValidNeighbors(int cellX, int cellY) {
    std::vector<std::pair<int, int>> neighbors;
    
    // Check all four directions
    const int dx[4] = {0, 1, 0, -1};
    const int dy[4] = {-1, 0, 1, 0}; // up, right, down, left
    
    for (int i = 0; i < 4; i++) {
        int newX = cellX + dx[i];
        int newY = cellY + dy[i];
        
        // Check if it's within bounds
        if (newX >= 0 && newX < width && newY >= 0 && newY < height) {
            // Need to check if there's a wall - simulate vision sensor
            int tempDir = i;
            direction = tempDir;
            updateSensors();
            
            if (canMoveForward()) {
                neighbors.push_back({newX, newY});
            }
        }
    }
    
    // Restore direction and sensors
    updateSensors();
    
    return neighbors;
}

// New method: Find least visited neighbor
std::pair<int, int> Mouse::findLeastVisitedNeighbor() {
    std::vector<std::pair<int, int>> neighbors = getValidNeighbors(x, y);
    std::pair<int, int> leastVisited = {-1, -1};
    int minVisits = std::numeric_limits<int>::max();
    
    for (const auto& neighbor : neighbors) {
        int nx = neighbor.first;
        int ny = neighbor.second;
        
        // Skip deadends
        if (deadends[ny][nx]) continue;
        
        if (visited[ny][nx] < minVisits) {
            minVisits = visited[ny][nx];
            leastVisited = neighbor;
        }
    }
    
    return leastVisited;
}

// New method: A* pathfinding to backtrack to unexplored areas
std::vector<std::pair<int, int>> Mouse::findPathToLeastVisited() {
    // Priority queue for A* search
    std::priority_queue<std::tuple<int, int, int>, 
                        std::vector<std::tuple<int, int, int>>, 
                        std::greater<std::tuple<int, int, int>>> pq;
    
    // Maps for parent nodes and costs
    std::vector<std::vector<std::pair<int, int>>> parent(height, 
                                                        std::vector<std::pair<int, int>>(width, {-1, -1}));
    std::vector<std::vector<int>> gScore(height, std::vector<int>(width, std::numeric_limits<int>::max()));
    
    // Find the least visited cell in the entire maze
    std::pair<int, int> target = {-1, -1};
    int minVisits = std::numeric_limits<int>::max();
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Check if this cell is accessible and not a deadend
            if (!deadends[y][x] && visited[y][x] > 0 && visited[y][x] < minVisits) {
                std::vector<std::pair<int, int>> neighbors = getValidNeighbors(x, y);
                for (const auto& neigh : neighbors) {
                    if (visited[neigh.second][neigh.first] == 0) {
                        minVisits = visited[y][x];
                        target = {x, y};
                        break;
                    }
                }
            }
        }
    }
    
    // If no target found, return empty path
    if (target.first == -1 || (target.first == x && target.second == y)) {
        return {};
    }
    
    // Start A* search
    pq.push({0, x, y});
    gScore[y][x] = 0;
    
    while (!pq.empty()) {
        auto [cost, curX, curY] = pq.top();
        pq.pop();
        
        if (curX == target.first && curY == target.second) {
            // Reconstruct path
            std::vector<std::pair<int, int>> path;
            std::pair<int, int> current = target;
            
            while (current.first != -1 && current.second != -1) {
                path.push_back(current);
                current = parent[current.second][current.first];
            }
            
            std::reverse(path.begin(), path.end());
            return path;
        }
        
        std::vector<std::pair<int, int>> neighbors = getValidNeighbors(curX, curY);
        
        for (const auto& neighbor : neighbors) {
            int nx = neighbor.first;
            int ny = neighbor.second;
            
            // Skip deadends
            if (deadends[ny][nx]) continue;
            
            int newCost = gScore[curY][curX] + 1 + visited[ny][nx];
            
            if (newCost < gScore[ny][nx]) {
                parent[ny][nx] = {curX, curY};
                gScore[ny][nx] = newCost;
                
                // Heuristic: Manhattan distance
                int h = std::abs(nx - target.first) + std::abs(ny - target.second);
                pq.push({newCost + h, nx, ny});
            }
        }
    }
    
    return {}; // No path found
}

void Mouse::move() {
    // Get smell intensity to see if we're close to the goal
    double smellIntensity = smell->detect();
    
    // If near the goal, prioritize smell-based navigation
    if (smellIntensity > 0.8) {
        int originalDirection = direction;
        double bestSmell = 0.0;
        int bestDirection = -1;
        
        for (int i = 0; i < 4; i++) {
            direction = i;
            updateSensors();
            
            if (canMoveForward()) {
                int dx = 0, dy = 0;
                switch (direction) {
                    case 0: dy = -1; break; // up
                    case 1: dx = 1; break;  // right
                    case 2: dy = 1; break;  // down
                    case 3: dx = -1; break; // left
                }
                
                // Only consider cells that aren't marked as dead ends
                if (!deadends[y + dy][x + dx]) {
                    SmellSense tempSmell(grid, x + dx, y + dy);
                    double newSmell = tempSmell.detect();
                    
                    if (newSmell > bestSmell) {
                        bestSmell = newSmell;
                        bestDirection = i;
                    }
                }
            }
        }
        
        // Restore original direction
        direction = originalDirection;
        updateSensors();
        
        // If found a better direction with stronger smell, go there
        if (bestDirection != -1) {
            direction = bestDirection;
            updateSensors();
            moveForward();
            return;
        }
    }
    
    // Check if we're in a high visit count area (potential loop)
    if (visited[y][x] > 2) {
        // Try to find least visited neighbor first
        auto leastVisited = findLeastVisitedNeighbor();
        
        if (leastVisited.first != -1) {
            // Navigate to least visited neighbor
            int targetX = leastVisited.first;
            int targetY = leastVisited.second;
            
            // Figure out which direction to turn
            int dx = targetX - x;
            int dy = targetY - y;
            int targetDir;
            
            if (dx == 1) targetDir = 1;      // right
            else if (dx == -1) targetDir = 3; // left
            else if (dy == -1) targetDir = 0; // up
            else targetDir = 2;              // down
            
            // Turn until we face the right direction
            while (direction != targetDir) {
                turnRight();
            }
            
            moveForward();
            return;
        } else {
            // No good neighbor, try to find path to unexplored areas
            auto path = findPathToLeastVisited();
            
            if (!path.empty() && path.size() > 1) {
                // Take the first step of the path
                int targetX = path[1].first;
                int targetY = path[1].second;
                
                // Figure out which direction to turn
                int dx = targetX - x;
                int dy = targetY - y;
                int targetDir;
                
                if (dx == 1) targetDir = 1;      // right
                else if (dx == -1) targetDir = 3; // left
                else if (dy == -1) targetDir = 0; // up
                else targetDir = 2;              // down
                
                // Turn until we face the right direction
                while (direction != targetDir) {
                    turnRight();
                }
                
                moveForward();
                return;
            }
        }
    }
    
    // If all else fails, use modified wall-following that avoids visited areas
    std::vector<int> possibleDirs;
    int originalDir = direction;
    
    // Check right turn first (preferred in wall-following)
    turnRight();
    if (canMoveForward()) {
        int dx = 0, dy = 0;
        switch (direction) {
            case 0: dy = -1; break; // up
            case 1: dx = 1; break;  // right
            case 2: dy = 1; break;  // down
            case 3: dx = -1; break; // left
        }
        
        // Add to possible directions if not a deadend
        if (!deadends[y + dy][x + dx]) {
            possibleDirs.push_back(direction);
        }
    }
    
    // Check forward
    direction = originalDir;
    updateSensors();
    if (canMoveForward()) {
        int dx = 0, dy = 0;
        switch (direction) {
            case 0: dy = -1; break; // up
            case 1: dx = 1; break;  // right
            case 2: dy = 1; break;  // down
            case 3: dx = -1; break; // left
        }
        
        // Add to possible directions if not a deadend
        if (!deadends[y + dy][x + dx]) {
            possibleDirs.push_back(direction);
        }
    }
    
    // Check left turn
    turnLeft();
    if (canMoveForward()) {
        int dx = 0, dy = 0;
        switch (direction) {
            case 0: dy = -1; break; // up
            case 1: dx = 1; break;  // right
            case 2: dy = 1; break;  // down
            case 3: dx = -1; break; // left
        }
        
        // Add to possible directions if not a deadend
        if (!deadends[y + dy][x + dx]) {
            possibleDirs.push_back(direction);
        }
    }
    
    // Restore original direction
    direction = originalDir;
    updateSensors();
    
    if (!possibleDirs.empty()) {
        // Choose the direction with the least visits
        int bestDir = -1;
        int minVisits = std::numeric_limits<int>::max();
        
        for (int dir : possibleDirs) {
            direction = dir;
            updateSensors();
            
            if (canMoveForward()) {
                int dx = 0, dy = 0;
                switch (direction) {
                    case 0: dy = -1; break; // up
                    case 1: dx = 1; break;  // right
                    case 2: dy = 1; break;  // down
                    case 3: dx = -1; break; // left
                }
                
                if (visited[y + dy][x + dx] < minVisits) {
                    minVisits = visited[y + dy][x + dx];
                    bestDir = dir;
                }
            }
        }
        
        if (bestDir != -1) {
            direction = bestDir;
            updateSensors();
            moveForward();
            return;
        }
    }
    
    // If all fails, resort to basic turn and try again
    turnLeft();
}