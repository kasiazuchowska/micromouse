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

// Check which direction leads to the least visited cell
int Mouse::findLeastVisitedDirection() {
    int leastVisitedDir = -1;
    int minVisitCount = 1000;
    int originalDir = direction;
    
    // Check each direction
    for (int dir = 0; dir < 4; dir++) {
        direction = dir;
        updateSensors();
        
        if (canMoveForward()) {
            // Calculate the new position
            int dx = 0, dy = 0;
            switch (direction) {
                case 0: dy = -1; break; // up
                case 1: dx = 1; break;  // right
                case 2: dy = 1; break;  // down
                case 3: dx = -1; break; // left
            }
            
            int newX = x + dx;
            int newY = y + dy;
            
            // If this cell has been visited fewer times than our current minimum
            if (visited[newY][newX] < minVisitCount) {
                minVisitCount = visited[newY][newX];
                leastVisitedDir = dir;
            }
        }
    }
    
    // Restore original direction
    direction = originalDir;
    updateSensors();
    
    return leastVisitedDir;
}

void Mouse::move() {
    // Find the least visited accessible cell
    int leastVisitedDir = findLeastVisitedDirection();
    
    // If we found a direction with least visited cell, go there
    if (leastVisitedDir != -1) {
        direction = leastVisitedDir;
        updateSensors();
        moveForward();
        return;
    }

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