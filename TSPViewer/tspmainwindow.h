#ifndef TSPMAINWINDOW_H
#define TSPMAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QGraphicsScene>

#define TSP_POINT_SIZE      (1.0)
#define TSP_SCALE_FACTOR    (100.0)

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
    void computeEnvelope( void ) ;
    void splitNextEdges( void ) ;
    void run( void ) ;
    void refreshView( void ) ;
};

#endif // TSPMAINWINDOW_H
