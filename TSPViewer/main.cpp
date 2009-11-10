#include <QtGui/QApplication>
#include "tspmainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TSPMainWindow w;
    w.show();
    return a.exec();
}
