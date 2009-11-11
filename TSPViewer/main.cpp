#include <QtGui/QApplication>
#include "tspmainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TSPMainWindow w;
    w.showMaximized();
    return a.exec();
}
