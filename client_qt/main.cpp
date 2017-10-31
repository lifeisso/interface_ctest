#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont font  = a.font();
    font.setFamily("unifont");

    font.setPixelSize(16);

   a.setFont(font);
    MainWindow w;
    w.show();

    return a.exec();
}
