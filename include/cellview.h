#ifndef CELLVIEW_H
#define CELLVIEW_H

#include <QWidget>

class CellView : public QWidget {
public:
    CellView(QWidget* parent = nullptr);
    
    void setWall(bool value);
    void setGoal(bool value);
    void setMouse(bool value);
    void setVisitCount(int count);
    
protected:
    void paintEvent(QPaintEvent* event) override;
    
private:
    bool isWall;
    bool isGoal;
    bool isMouse;
    int visitCount;
};

#endif