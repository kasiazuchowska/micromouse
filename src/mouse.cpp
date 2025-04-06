#include "mouse.h"
#include <algorithm>
#include <random>
#include <queue>
#include <utility>
#include <cmath>

Mouse::Mouse(Grid* grid, int startX, int startY)
    : grid(grid), x(startX), y(startY), direction(1) {
    visited.resize(grid->getHeight());
    for (int y = 0; y < grid->getHeight(); ++y) {
        visited[y].resize(grid->getWidth(), 0);
    }
    
    visited[y][x] = 1;
    
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
        int dx = 0, dy = 0;
        switch (direction) {
            case 0: dy = -1; break;
            case 1: dx = 1; break;
            case 2: dy = 1; break;
            case 3: dx = -1; break;
        }
        
        x += dx;
        y += dy;
        visited[y][x]++;
        updateSensors();
    }
}

int Mouse::findLeastVisitedDirection() {
    int leastVisitedDir = -1;
    int minVisitCount = 1000;
    int originalDir = direction;
    
    for (int dir = 0; dir < 4; dir++) {
        direction = dir;
        updateSensors();
        
        if (canMoveForward()) {
            int dx = 0, dy = 0;
            switch (direction) {
                case 0: dy = -1; break;
                case 1: dx = 1; break;
                case 2: dy = 1; break;
                case 3: dx = -1; break;
            }
            
            int newX = x + dx;
            int newY = y + dy;
            
            if (visited[newY][newX] < minVisitCount) {
                minVisitCount = visited[newY][newX];
                leastVisitedDir = dir;
            }
        }
    }
    
    direction = originalDir;
    updateSensors();
    
    return leastVisitedDir;
}

void Mouse::move() {
    int leastVisitedDir = findLeastVisitedDirection();
    
    if (leastVisitedDir != -1) {
        direction = leastVisitedDir;
        updateSensors();
        moveForward();
        return;
    }

    double smellIntensity = smell->detect();
    
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
                    case 0: dy = -1; break;
                    case 1: dx = 1; break;
                    case 2: dy = 1; break;
                    case 3: dx = -1; break;
                }
                
                SmellSense tempSmell(grid, x + dx, y + dy);
                double newSmell = tempSmell.detect();
                
                if (newSmell > bestSmell) {
                    bestSmell = newSmell;
                    bestDirection = i;
                }
            }
        }
        
        direction = originalDirection;
        updateSensors();
        
        if (bestDirection != -1) {
            direction = bestDirection;
            updateSensors();
            moveForward();
            return;
        }
    }
    
    if (canMoveForward()) {
        turnRight();
        
        if (canMoveForward()) {
            moveForward();
        } else {
            turnLeft();
            moveForward();
        }
    } else {
        turnLeft();
        
        if (!canMoveForward()) {
            turnLeft();
        }
    }
}