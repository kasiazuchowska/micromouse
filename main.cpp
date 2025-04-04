#include <QApplication>
#include "game.h"
#include "ui.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    Game game(21, 21);
    UI ui(&game);
    
    ui.show();
    
    return app.exec();
}