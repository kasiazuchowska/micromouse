#include "game.h"

Game::Game(int width, int height, QObject* parent)
    : QObject(parent), grid(width, height), ui(nullptr) {
    
    connect(&timer, &QTimer::timeout, this, &Game::step);
}

Game::~Game() {
}

void Game::start() {
    mazeGenerator.generate(grid);
    
    mouse = std::make_unique<Mouse>(&grid, 1, 1);
    
    timer.start(100);

    emit update();
}

void Game::stop() {
    timer.stop();
}

void Game::step() {
    mouse->move();
    
    emit update();
    
    if (checkWinCondition()) {
        stop();
        emit gameWon();
    }
}

bool Game::checkWinCondition() {
    Cell* cell = grid.getCell(mouse->getX(), mouse->getY());
    return cell && cell->isGoal;
}