#include "ui.h"
#include "cellview.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "game.h"

UI::UI(Game* game, QWidget* parent)
    : QWidget(parent), game(game) {
    
    // Create main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Create status label
    statusLabel = new QLabel("Press Start to begin");
    mainLayout->addWidget(statusLabel);
    
    // Create grid layout for maze display
    gridLayout = new QGridLayout();
    gridLayout->setSpacing(1);
    mainLayout->addLayout(gridLayout);
    
    // Create start button
    startButton = new QPushButton("Start");
    mainLayout->addWidget(startButton);
    
    // Connect signals/slots
    connect(startButton, &QPushButton::clicked, [this]() {
        startButton->setEnabled(false);
        statusLabel->setText("Mouse is exploring the maze...");
        this->game->start();
    });
    
    connect(game, &Game::update, this, &UI::render);
    connect(game, &Game::gameWon, this, &UI::handleGameWon);
    
    // Create maze display
    createMazeDisplay();
    
    // Set window properties
    setWindowTitle("Mouse Maze");
    resize(600, 600);
}

void UI::createMazeDisplay() {
    // Clear existing grid
    QLayoutItem* item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    // Get grid dimensions
    Grid* grid = game->getGrid();
    int width = grid->getWidth();
    int height = grid->getHeight();
    
    // Create cell views
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
    
    // Get mouse position and visited cells
    int mouseX = mouse->getX();
    int mouseY = mouse->getY();
    const auto& visited = mouse->getVisited();
    
    // Update cell views
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Cell* cell = grid->getCell(x, y);
            CellView* cellView = static_cast<CellView*>(gridLayout->itemAtPosition(y, x)->widget());
            
            cellView->setWall(cell->isObstacle);
            cellView->setGoal(cell->isGoal);
            cellView->setMouse(x == mouseX && y == mouseY);
            
            // Set visit count (instead of boolean visited state)
            int visitCount = 0;
            if (y < visited.size() && x < visited[y].size()) {
                visitCount = visited[y][x];
            }
            cellView->setVisitCount(visitCount);
        }
    }
}

void UI::handleGameWon() {
    statusLabel->setText("Success! The mouse found the cheese!");
    startButton->setEnabled(true);
    startButton->setText("Restart");
}