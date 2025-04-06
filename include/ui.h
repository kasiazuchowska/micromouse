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
    /// @brief konstruktor, ustawia podstawowy wyglad aplikacji, podpina odpowiednie sygnaly do slotow
    /// @param game gra
    /// @param parent 
    UI(Game* game, QWidget* parent = nullptr);
    
public slots:
    /// @brief ustawia wyglad komorek labiryntu w zaleznosci od ich aktualnego stanu
    void render();
    /// @brief wylacza gre i umozliwia jej ponowny start
    void handleGameWon();
    
private:
    Game* game;
    QGridLayout* gridLayout;
    QLabel* statusLabel;
    QPushButton* startButton;
    
    /// @brief metoda pomocnicza do stworzenia poczatkowego wygladu labiryntu
    void createMazeDisplay();
};

#endif