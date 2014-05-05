#include "main.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char **argv) {
    new QApplication(argc, argv);

    Q_FOREACH(FontRenderer *fr, gFontRenderer)
        FontRenderer::registerRenderer(fr);

    (new MainWindow)->show();

    return qApp->exec();
}
