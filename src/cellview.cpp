#include "cellview.h"
#include <QPainter>
#include <QColor>

CellView::CellView(QWidget* parent) 
    : QWidget(parent), isWall(false), isGoal(false), isMouse(false), visitCount(0) {
    setMinimumSize(20, 20);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void CellView::setWall(bool value) { 
    isWall = value; 
    update(); 
}

void CellView::setGoal(bool value) { 
    isGoal = value; 
    update(); 
}

void CellView::setMouse(bool value) { 
    isMouse = value; 
    update(); 
}

void CellView::setVisitCount(int count) { 
    visitCount = count; 
    update(); 
}

void CellView::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    
    if (isWall) {
        painter.fillRect(rect(), Qt::black);
    } else if (isGoal) {
        painter.fillRect(rect(), Qt::green);
    } else if (isMouse) {
        painter.fillRect(rect(), Qt::red);
    } else if (visitCount > 0) {
        int intensity = std::max(255 - (visitCount * 30), 100);
        painter.fillRect(rect(), QColor(intensity, intensity, 255));
    } else {
        painter.fillRect(rect(), Qt::white);
    }
    
    painter.setPen(Qt::gray);
    painter.drawRect(rect().adjusted(0, 0, -1, -1));
    
    if (visitCount >= 1) {
        painter.setPen(Qt::black);
        painter.drawText(rect(), Qt::AlignCenter, QString::number(visitCount));
    }
}