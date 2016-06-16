#include "arduinoLEDs.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ColorSelect w;
    w.setWindowTitle("Arduino LED Strip Controller");
    w.show();

    return a.exec();
}
