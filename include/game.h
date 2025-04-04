#ifndef GAME_H
#define GAME_H

#include "grid.h"
#include "mouse.h"
#include "ui.h"
#include "mazegenerator.h"
#include <QTimer>

class UI;

class Game : public QObject {
    Q_OBJECT
    
public:
    Game(int width, int height, QObject* parent = nullptr);
    ~Game();
    
    void start();
    void stop();
    
    Grid* getGrid() { return &grid; }
    Mouse* getMouse() { return mouse.get(); }
    
signals:
    void update();
    void gameWon();
    
private slots:
    void step();
    
private:
    Grid grid;
    std::unique_ptr<Mouse> mouse;
    UI* ui;
    MazeGenerator mazeGenerator;
    QTimer timer;
    
    bool checkWinCondition();
};

#endif