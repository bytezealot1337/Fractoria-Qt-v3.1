#include "fractoria.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Fractoria w;
    w.show();

    return a.exec();
}
