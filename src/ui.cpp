#include "ui.h"
#include "cellview.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "game.h"

UI::UI(Game* game, QWidget* parent)
    : QWidget(parent), game(game) {
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    statusLabel = new QLabel("Nacisnij start aby rozpoczac");
    mainLayout->addWidget(statusLabel);
    
    gridLayout = new QGridLayout();
    gridLayout->setSpacing(1);
    mainLayout->addLayout(gridLayout);
    
    startButton = new QPushButton("Start");
    mainLayout->addWidget(startButton);
    
    connect(startButton, &QPushButton::clicked, [this]() {
        startButton->setEnabled(false);
        statusLabel->setText("Mysz szuka celu");
        this->game->start();
    });
    
    connect(game, &Game::update, this, &UI::render);
    connect(game, &Game::gameWon, this, &UI::handleGameWon);
    
    createMazeDisplay();
    
    setWindowTitle("Micromouse");
    resize(600, 600);
}

void UI::createMazeDisplay() {
    QLayoutItem* item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    Grid* grid = game->getGrid();
    int width = grid->getWidth();
    int height = grid->getHeight();
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            CellView* cellView = new CellView();
            gridLayout->addWidget(cellView, y, x);
        }
    }
}

void UI::render() {
    Grid* grid = game->getGrid();
    Mouse* mouse = game->getMouse();
    
    int width = grid->getWidth();
    int height = grid->getHeight();
    
    int mouseX = mouse->getX();
    int mouseY = mouse->getY();
    const auto& visited = mouse->getVisited();
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Cell* cell = grid->getCell(x, y);
            CellView* cellView = static_cast<CellView*>(gridLayout->itemAtPosition(y, x)->widget());
            
            cellView->setWall(cell->isObstacle);
            cellView->setGoal(cell->isGoal);
            cellView->setMouse(x == mouseX && y == mouseY);
            
            int visitCount = 0;
            if (y < visited.size() && x < visited[y].size()) {
                visitCount = visited[y][x];
            }
            cellView->setVisitCount(visitCount);
        }
    }
}

void UI::handleGameWon() {
    statusLabel->setText("WYGRANA!!!");
    startButton->setEnabled(true);
    startButton->setText("Restart");
}