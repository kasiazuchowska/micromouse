#ifndef GAME_H
#define GAME_H

#include "grid.h"
#include "mouse.h"
#include "ui.h"
#include "mazegenerator.h"
#include <QTimer>

class UI;

/// @brief klasa reprezentujaca cala gre
class Game : public QObject {
    Q_OBJECT
    
public:
    /// @brief konstruktor, inicjalizuje wartosci, podlacza timer
    /// @param width szerokosc siatki z gra
    /// @param height wysokosc siatki z gra
    /// @param parent obiekt qt
    Game(int width, int height, QObject* parent = nullptr);
    ~Game();
    
    /// @brief generuje siatke, ustawia timer i mysz
    void start();

    /// @brief wylacza timer
    void stop();
    
    Grid* getGrid() { return &grid; }
    Mouse* getMouse() { return mouse.get(); }
    
signals:
    /// @brief sygnal wysylany do ui mowiacy ze nalezy odswiezyc wyglad gry
    void update();

    /// @brief sygnal wysylany do ui mowiacy ze gra sie skonczyla
    void gameWon();
    
private slots:
    /// @brief przesuwa mysz oraz sprawdza czy nie nastapila wygrana
    void step();
    
private:
    Grid grid;
    std::unique_ptr<Mouse> mouse;
    UI* ui;
    MazeGenerator mazeGenerator;
    QTimer timer;
    

    /// @brief sprawdza czy mysz znajduje sie w celu
    /// @return czy mysz nzajduje sie u celu
    bool checkWinCondition();
};

#endif