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
    m_scene.setSceneRect( -0.05*TSP_SCALE_FACTOR, -0.05*TSP_SCALE_FACTOR, 1.1*TSP_SCALE_FACTOR, 1.1*TSP_SCALE_FACTOR ) ;
    ui->pQt_graphicsView->setScene( &m_scene ) ;
    ui->pQt_graphicsView->setResizeAnchor( QGraphicsView::AnchorViewCenter ) ;

    //
    connect( ui->pQt_actionOpen, SIGNAL(triggered()), this, SLOT(openFile()) ) ;
    connect( ui->pQt_actionRun, SIGNAL(triggered()), this, SLOT(run()) ) ;
    connect( ui->pQt_actionComputeEnveloppe, SIGNAL(triggered()), this, SLOT(computeEnvelope()) ) ;
    connect( ui->pQt_actionSplitEdge, SIGNAL(triggered()), this, SLOT(splitNextEdges()) ) ;

    //
    this->readFile( "defi25.csv" ) ;
}

//----------------------------------------------------------------------------------
TSPMainWindow::~TSPMainWindow()
{
    delete ui;
}

//----------------------------------------------------------------------------------
void
TSPMainWindow::computeEnvelope( void )
{
    TSPLib::computeEnvelope() ;

    refreshView();
}

//----------------------------------------------------------------------------------
void
TSPMainWindow::splitNextEdges( void )
{
    int nbSteps = ui->pQt_spinNbSteps->value() ;
    TSPLib::reduceEnvelope( nbSteps ) ;

    refreshView();
}

//----------------------------------------------------------------------------------
void
TSPMainWindow::run( void )
{
    TSPLib::computePath() ;
    refreshView();
}

//----------------------------------------------------------------------------------
void
TSPMainWindow::refreshView( void )
{
    m_scene.clear();

    ui->pQt_label_distance->setText(QString::number(TSPLib::getPathLength()));

    int nbPoints = TSPLib::getNbPoints() ;
    Point *points = TSPLib::getPoints() ;
    int *path = TSPLib::getPath() ;
    int nbEdges = TSPLib::getNbEdges() ;

    // add points
    int iPoint ;
    double offset = 0 ;//0.5*TSP_POINT_SIZE ;
    QBrush brush ;
    brush.setStyle( Qt::SolidPattern ) ;
    QPen pen = QPen(QColor(Qt::green)) ;
    QFont font ;
    font.setPointSizeF( 1.5 ) ;

    QPoint textPos ;
    for( iPoint=0; iPoint<nbPoints; iPoint++ )
    {
        double xPos = TSP_SCALE_FACTOR*(points[iPoint].x-offset) ;
        double yPos = TSP_SCALE_FACTOR*(points[iPoint].y-offset) ;

        QGraphicsRectItem *pPointItem = m_scene.addRect( xPos, yPos, TSP_POINT_SIZE, TSP_POINT_SIZE, pen, brush ) ;

        QGraphicsSimpleTextItem *pTextItem = new QGraphicsSimpleTextItem( QString::number(iPoint), NULL, &m_scene ) ;
        pTextItem->setPos( xPos+1.5, yPos ) ;
        pTextItem->setFont( font ) ;
    }

    // add edges
    QPainterPath painterPath ;
    // init path
    if( nbEdges>0 )
    {
        double xPos = TSP_SCALE_FACTOR*( points[path[0]].x-offset ) ;
        double yPos = TSP_SCALE_FACTOR*( points[path[0]].y-offset ) ;
        painterPath.moveTo( xPos, yPos ) ;

        // connect following points
        for( iPoint=1; iPoint<nbEdges; iPoint++ )
        {
            xPos = TSP_SCALE_FACTOR*( points[path[iPoint]].x-offset ) ;
            yPos = TSP_SCALE_FACTOR*( points[path[iPoint]].y-offset ) ;

            painterPath.lineTo( xPos, yPos ) ;
        }

        // connect last edge
        xPos = TSP_SCALE_FACTOR*( points[path[0]].x-offset ) ;
        yPos = TSP_SCALE_FACTOR*( points[path[0]].y-offset ) ;
        painterPath.lineTo( xPos, yPos ) ;
    }

    pen.setColor( QColor(Qt::red) ) ;
    brush.setStyle( Qt::NoBrush ) ;
    m_scene.addPath( painterPath, pen, brush ) ;

    ui->pQt_graphicsView->fitInView( 0.0, 0.0, TSP_SCALE_FACTOR, TSP_SCALE_FACTOR, Qt::KeepAspectRatio ) ;
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
