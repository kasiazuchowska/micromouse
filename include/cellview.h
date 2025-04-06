#ifndef CELLVIEW_H
#define CELLVIEW_H

#include <QWidget>

/// @brief Klasa reprezentująca wygląd pojedynczej komórki
class CellView : public QWidget {
public:
    CellView(QWidget* parent = nullptr);
    
    /// @brief ustawienie czy w danym miejscu jest sciana
    /// @param value 
    void setWall(bool value);

    /// @brief ustawienie czy w danym miejscu jest cel
    /// @param value 
    void setGoal(bool value);

    /// @brief ustawienie czy w danym miejscu jest mysz
    /// @param value 
    void setMouse(bool value);

    /// @brief ustawienie ile razy zostala odwiedzona
    /// @param count ilosc odwiedzen
    void setVisitCount(int count);
    
protected:
    /// @brief ustawia odpowiedni wyglad komorki w zaleznosci od tego co sie w niej znajduje, im wiecej odwiedzen tym ciemniejszy kolor sciezki
    /// @param event 
    void paintEvent(QPaintEvent* event) override;
    
private:
    bool isWall;
    bool isGoal;
    bool isMouse;
    int visitCount;
};

#endif