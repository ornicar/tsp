#ifndef TSPMAINWINDOW_H
#define TSPMAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QGraphicsScene>

#define TSP_POINT_SIZE 0.02

namespace Ui
{
    class TSPMainWindow;
}

class TSPMainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::TSPMainWindow *ui;
    QGraphicsScene  m_scene ;

public:

    TSPMainWindow(QWidget *parent = 0);
    ~TSPMainWindow();

    bool readFile( const QString & path ) ;

public slots :

    bool openFile( QString path="" ) ;
    void run( void ) ;
    void refreshView( void ) ;
};

#endif // TSPMAINWINDOW_H
