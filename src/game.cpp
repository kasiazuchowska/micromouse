#include "game.h"

Game::Game(int width, int height, QObject* parent)
    : QObject(parent), grid(width, height), ui(nullptr) {
    
    connect(&timer, &QTimer::timeout, this, &Game::step);
}

Game::~Game() {
}

void Game::start() {
    // Generate maze
    mazeGenerator.generate(grid);
    
    // Create mouse at starting position (1,1)
    mouse = std::make_unique<Mouse>(&grid, 1, 1);
    
    // Start the game loop
    timer.start(100); // 10 steps per second
    
    emit update();
}

void Game::stop() {
    timer.stop();
}

void Game::step() {
    // Move the mouse
    mouse->move();
    
    // Update the UI
    emit update();
    
    // Check if the game is won
    if (checkWinCondition()) {
        stop();
        emit gameWon();
    }
}

bool Game::checkWinCondition() {
    Cell* cell = grid.getCell(mouse->getX(), mouse->getY());
    return cell && cell->isGoal;
}