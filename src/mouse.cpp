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
        visited[y].resize(grid->getWidth(), 0);  // Track visit count instead of just true/false
    }
    
    // Mark starting position as visited
    visited[y][x] = 1;
    
    // Initialize sensors
    updateSensors();
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
        updateSensors();
    }
}

// Check if a move in a given direction leads to a less visited cell
bool Mouse::isLessVisitedDirection(int dir) {
    // Store original direction
    int originalDir = direction;
    
    // Set new direction
    direction = dir;
    updateSensors();
    
    if (canMoveForward()) {
        // Get the potential new position
        int dx = 0, dy = 0;
        switch (direction) {
            case 0: dy = -1; break; // up
            case 1: dx = 1; break;  // right
            case 2: dy = 1; break;  // down
            case 3: dx = -1; break; // left
        }
        
        int newX = x + dx;
        int newY = y + dy;
        
        // Restore original direction
        direction = originalDir;
        updateSensors();
        
        // Check if the new position is less visited
        return visited[newY][newX] < visited[y][x];
    }
    
    // Restore original direction and return false if can't move
    direction = originalDir;
    updateSensors();
    return false;
}

void Mouse::move() {
    // Get smell intensity
    double smellIntensity = smell->detect();
    
    // If smell is strong enough, try to move towards the goal
    if (smellIntensity > 0.3) {
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
                
                // Check smell at potential new position
                SmellSense tempSmell(grid, x + dx, y + dy);
                double newSmell = tempSmell.detect();
                
                if (newSmell > bestSmell) {
                    bestSmell = newSmell;
                    bestDirection = i;
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
    
    // Check if we're stuck in a cell we've visited multiple times
    if (visited[y][x] > 2) {
        // Look for any direction that leads to a less visited cell
        bool foundLessVisited = false;
        
        // Try each direction to find a less visited cell
        for (int i = 0; i < 4; i++) {
            if (isLessVisitedDirection(i)) {
                direction = i;
                updateSensors();
                moveForward();
                foundLessVisited = true;
                break;
            }
        }
        
        // If we found a less visited direction, we're done
        if (foundLessVisited) {
            return;
        }
        
        // If no less visited cell was found, make a random choice to break out of loops
        std::vector<int> possibleDirs;
        for (int i = 0; i < 4; i++) {
            direction = i;
            updateSensors();
            if (canMoveForward()) {
                possibleDirs.push_back(i);
            }
        }
        
        if (!possibleDirs.empty()) {
            // Choose a random valid direction
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(0, possibleDirs.size() - 1);
            int randomIndex = distrib(gen);
            
            direction = possibleDirs[randomIndex];
            updateSensors();
            moveForward();
            return;
        }
    }
    
    // Default to wall-following if other approaches don't work
    if (canMoveForward()) {
        // Can move forward, try turning right first (wall-following logic)
        turnRight();
        
        if (canMoveForward()) {
            // Can turn right, so do it
            moveForward();
        } else {
            // Can't turn right, go back to forward
            turnLeft();
            moveForward();
        }
    } else {
        // Can't move forward, turn left
        turnLeft();
        
        // If we still can't move, turn again
        if (!canMoveForward()) {
            turnLeft();
        }
    }
}