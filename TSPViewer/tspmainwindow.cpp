#include "tspmainwindow.h"
#include "ui_tspmainwindow.h"
#include "tsplib.h"

#include <QFileDialog>
#include <QTextStream>
#include <QGraphicsRectItem>
#include <QPainterPath>
#include <QGraphicsPathItem>


//----------------------------------------------------------------------------------
TSPMainWindow::TSPMainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::TSPMainWindow)
{
    ui->setupUi(this);

    // resize scene view
    m_scene.setSceneRect( 0.0, 0.0, 100.0, 100.0 ) ;
    ui->pQt_graphicsView->setScene( &m_scene ) ;

    //
    connect(ui->pQt_actionOpen, SIGNAL(triggered()), this, SLOT(openFile())) ;

    //
    this->readFile( "../defi250.csv" ) ;
}

//----------------------------------------------------------------------------------
TSPMainWindow::~TSPMainWindow()
{
    delete ui;
}

//----------------------------------------------------------------------------------
void
TSPMainWindow::run( void )
{
    int *path = TSPLib::getPath() ;
}

//----------------------------------------------------------------------------------
void
TSPMainWindow::refreshView( void )
{
    m_scene.clear();

    int nbPoints = TSPLib::getNbPoints() ;
    Point *points = TSPLib::getPoints() ;
    int *path = TSPLib::getPath() ;
    int nbEdges = TSPLib::getNbEdges() ;

    // add points
    int iPoint ;
    for( iPoint=0; iPoint<nbPoints; iPoint++ )
    {
        QGraphicsRectItem *pPointItem = m_scene.addRect( 100*points[iPoint].x, 100*points[iPoint].y, 3, 3 ) ;
        //QGraphicsRectItem *pPointItem = m_scene.addRect( iPoint, iPoint, 0.05, 0.05 ) ;
    }

    // add edges
    QPainterPath painterPath ;
    // init path
    if( nbEdges>0 )
    {
        painterPath.moveTo( points[path[0]].x, points[path[0]].y ) ;
    }
    // connect following points
    for( iPoint=1; iPoint<nbEdges; iPoint++ )
    {
        painterPath.lineTo( 100*points[path[iPoint]].x, 100*points[path[iPoint]].y ) ;
    }

    m_scene.addPath( painterPath, QPen(QColor(Qt::red)), QBrush() ) ;

    ui->pQt_graphicsView->setSceneRect( 0,0,100,100 ) ;
}

//----------------------------------------------------------------------------------
bool
TSPMainWindow::openFile( QString path )
{
    if( path.isEmpty()==true )
    {
        // open file dialog
        path =
        QFileDialog::getOpenFileName( this,
                                      "Open file",
                                      QApplication::applicationDirPath(),
                                      "CSV File (*.csv)" ) ;
    }

    return readFile( path ) ;
}

//----------------------------------------------------------------------------------
bool
TSPMainWindow::readFile( const QString & path )
{
    QFile file( path ) ;

    if( file.exists()==true )
    {
        if( file.open( QIODevice::ReadOnly )==true )
        {
            QTextStream stream( &file ) ;

            // read points count
            int nbPoints ;
            stream >> nbPoints ;

            // allocate point array
            Point *points = new Point[nbPoints] ;

            // read cities' location
            int iPoint=0 ;
            QString line ;
            while( stream.atEnd()==false )
            {
                stream >> line ;
                if( line.isNull()==false )
                {
                    QStringList coords = line.split( ';', QString::SkipEmptyParts ) ;
                    points[iPoint].x = coords.at(0).toDouble() ;
                    points[iPoint].y = coords.at(1).toDouble() ;

                    iPoint++ ;
                }
                else
                {
                    qWarning( "fin fichier\n" ) ;
                }
            }

            //
            TSPLib::setInput( nbPoints, points ) ;

            // refresh view
            refreshView() ;

            // free memory
            delete[] points ;
        }
        else
        {
            qWarning( "Could not open file" ) ;
        }
    }
    else
    {
        qWarning( "File does not exist" ) ;
    }
    return false ;
}
