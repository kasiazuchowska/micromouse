#include "mouse.h"
#include <algorithm>
#include <random>

Mouse::Mouse(Grid* grid, int startX, int startY)
    : grid(grid), x(startX), y(startY), direction(1) {
    // Initialize visited array
    visited.resize(grid->getHeight());
    for (int y = 0; y < grid->getHeight(); ++y) {
        visited[y].resize(grid->getWidth(), false);
    }
    
    // Mark starting position as visited
    visited[y][x] = true;
    
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
        visited[y][x] = true;
        updateSensors();
    }
}

void Mouse::move() {
    // Mouse movement algorithm - using senses to navigate
    
    // Get smell intensity
    double smellIntensity = smell->detect();
    
    // Simple right-hand-wall-following algorithm with smell influence
    // If smell is strong enough, try to move towards the goal
    if (smellIntensity > 0.8) {
        // Try all directions to find the best path towards the goal
        int originalDirection = direction;
        double bestSmell = 0.0;
        int bestDirection = -1;
        
        for (int i = 0; i < 4; i++) {
            direction = i;
            updateSensors();
            
            if (canMoveForward()) {
                // Move temporarily to check smell
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
    
    // Default to wall-following if smell-based approach doesn't work
    if (canMoveForward()) {
        // Can move forward, try turning right first
        turnRight();
        
        if (!canMoveForward()) {
            // Can't turn right, go back to forward
            turnLeft();
            moveForward();
        } else {
            // Can turn right, so do it
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