#ifndef UI_H
#define UI_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include "game.h"

class Game;

class UI : public QWidget {
    Q_OBJECT
    
public:
    UI(Game* game, QWidget* parent = nullptr);
    
public slots:
    void render();
    void handleGameWon();
    
private:
    Game* game;
    QGridLayout* gridLayout;
    QLabel* statusLabel;
    QPushButton* startButton;
    
    void createMazeDisplay();
};

#endif