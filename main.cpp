#include <QApplication>
#include "game.h"
#include "ui.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    Game game(25, 25);
    UI ui(&game);
    
    ui.show();
    
    return app.exec();
}