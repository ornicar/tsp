#ifndef TSPMAINWINDOW_H
#define TSPMAINWINDOW_H

#include <QtGui/QMainWindow>

namespace Ui
{
    class TSPMainWindow;
}

class TSPMainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::TSPMainWindow *ui;

public:

    TSPMainWindow(QWidget *parent = 0);
    ~TSPMainWindow();

    bool readFile( const QString & path ) ;

public slots :

    bool openFile( QString path="" ) ;
    void run( void ) ;
};

#endif // TSPMAINWINDOW_H
