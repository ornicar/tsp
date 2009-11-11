#include "tspmainwindow.h"
#include "ui_tspmainwindow.h"
#include "tsplib.h"

#include <QFileDialog>
#include <QTextStream>
#include <QGraphicsRectItem>


//----------------------------------------------------------------------------------
TSPMainWindow::TSPMainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::TSPMainWindow)
{
    ui->setupUi(this);

    // resize scene view
    m_scene.setSceneRect( 0.0, 0.0, 1.0, 1.0 ) ;
    ui->pQt_graphicsView->setScene( &m_scene ) ;
    ui->pQt_graphicsView->setSceneRect( 0.0, 0.0, 1.0, 1.0 ) ;

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
    m_scene.clear() ;
    m_scene.setSceneRect( 0.0, 0.0, 1.0, 1.0 ) ;
    ui->pQt_graphicsView->setSceneRect( 0.0, 0.0, 1.0, 1.0 ) ;

    int nbPoints = TSPLib::getNbPoints() ;
    Point *points = TSPLib::getPoints() ;
    int *path = TSPLib::getPath() ;

    int iPoint ;
    for( iPoint=0; iPoint<nbPoints; iPoint++ )
    {
        QGraphicsRectItem *pPointItem = m_scene.addRect( points[iPoint].x, points[iPoint].y, 0.05, 0.05 ) ;
        //QGraphicsRectItem *pPointItem = m_scene.addRect( iPoint, iPoint, 0.05, 0.05 ) ;
    }

    ui->pQt_graphicsView->fitInView( 0.0, 0.0, 1.0, 1.0 ) ;
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
