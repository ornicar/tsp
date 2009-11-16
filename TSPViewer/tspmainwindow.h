#ifndef TSPMAINWINDOW_H
#define TSPMAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPathItem>

#include "tsplibobserver.h"

#define TSP_POINT_SIZE      (0.1)
#define TSP_SCALE_FACTOR    (100.0)

class QGraphicsPathItem ;

namespace Ui
{
    class TSPMainWindow;
}

class TSPMainWindow :   public QMainWindow,
                        public TSPLibObserver
{
    Q_OBJECT

    private:
        Ui::TSPMainWindow *ui;
        QGraphicsScene  m_scene ;
        QGraphicsPathItem m_path ;

    public:

        TSPMainWindow(QWidget *parent = 0);
        ~TSPMainWindow();

        bool readFile( const QString & path ) ;

        // pattern observer
        virtual void Update( TSPLibObserved * pObserved ) ;

    public slots :

        bool openCSVFile( QString path="" ) ;
        void genVertices( void ) ;
        void genLevel( void ) ;

    protected slots :

        void refreshView( void ) ;
        void refreshSceneRect( void ) ;
        void refreshPath( void ) ;
        void refreshVertices( void ) ;
};

#endif // TSPMAINWINDOW_H
