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
    connect( ui->pQt_actionOpen, SIGNAL(triggered()), this, SLOT(openCSVFile()) ) ;
    connect( ui->pQt_buttonGenVertices, SIGNAL(clicked()), this, SLOT(genVertices()) ) ;
    connect( ui->pQt_buttonGenLevel, SIGNAL(clicked()), this, SLOT(genLevel()) ) ;

    //
    connect( ui->pQt_spinSceneWidth, SIGNAL(valueChanged(int)), this, SLOT(refreshView()) ) ;
    connect( ui->pQt_spinSceneHeight, SIGNAL(valueChanged(int)), this, SLOT(refreshView()) ) ;

    // scene related
    QPen pen ;
    QBrush brush ;

    pen.setColor( QColor(Qt::red) ) ;
    brush.setColor( QColor(Qt::lightGray) ) ;
    brush.setStyle( Qt::SolidPattern ) ;
    m_scene.addItem( &m_path ) ;
    m_path.setZValue( 1.0 ) ;
    m_path.setPen( pen ) ;
    m_path.setBrush( brush ) ;
}

//----------------------------------------------------------------------------------
TSPMainWindow::~TSPMainWindow()
{
    delete ui;
}

//----------------------------------------------------------------------------------
void
TSPMainWindow::genVertices( void )
{
    int i, nbVertices = ui->pQt_spinNbVertices->value() ;
    int sceneWidth = ui->pQt_spinSceneWidth->value() ;
    int sceneHeight = ui->pQt_spinSceneHeight->value() ;
    Point* vertices = new Point[nbVertices] ;

    // init random seed
    qsrand( time(NULL) ) ;

    for( i=0; i<nbVertices; i++ )
    {
        vertices[i].x = sceneWidth * (double(qrand())/RAND_MAX) ;
        vertices[i].y = sceneHeight * (double(qrand())/RAND_MAX) ;
    }

    // set lib input
    TSPLib::instance()->setInput( nbVertices, vertices ) ;

    // display generated points
    refreshView() ;

    // free memory
    delete[] vertices ;
}

//----------------------------------------------------------------------------------
void
TSPMainWindow::genLevel( void )
{
    TSPLib::instance()->Attach( this ) ;

        TSPLib::instance()->computePath() ;

    TSPLib::instance()->Detach( this ) ;

    refreshView() ;
}

//----------------------------------------------------------------------------------
void
TSPMainWindow::refreshSceneRect( void )
{
    // scene bounding rect
    int sceneWidth = ui->pQt_spinSceneWidth->value() ;
    int sceneHeight = ui->pQt_spinSceneHeight->value() ;

    //
    m_scene.setBackgroundBrush( QBrush(QColor(Qt::darkBlue)) ) ;
}

//----------------------------------------------------------------------------------
void
TSPMainWindow::refreshPath( void )
{
    // retrieve info
    Point *points = TSPLib::instance()->getPoints() ;
    int *path = TSPLib::instance()->getPath() ;
    int nbEdges = TSPLib::instance()->getNbEdges() ;

    // draw edges
    QPainterPath painterPath ;
    // init path
    if( nbEdges>0 )
    {
        double xPos = points[path[0]].x ;
        double yPos = points[path[0]].y ;
        painterPath.moveTo( xPos, yPos ) ;

        // connect following points
        int iPoint ;
        for( iPoint=1; iPoint<nbEdges; iPoint++ )
        {
            xPos = points[path[iPoint]].x ;
            yPos = points[path[iPoint]].y ;

            painterPath.lineTo( xPos, yPos ) ;
        }

        // connect last edge
        xPos = points[path[0]].x ;
        yPos = points[path[0]].y ;
        painterPath.lineTo( xPos, yPos ) ;
    }

    m_path.setPath( painterPath ) ;
}

//----------------------------------------------------------------------------------
void
TSPMainWindow::refreshVertices( void )
{
    // add points
    int iPoint, nbPoints = TSPLib::instance()->getNbPoints() ;
    Point *points = TSPLib::instance()->getPoints() ;
    double xPos, yPos ;
    QPoint textPos ;
    QGraphicsRectItem *pRectItem ;
    QFont font ;
    font.setPointSizeF( 1.5 ) ;
    QPen pen(QColor(Qt::green)) ;
    QBrush brush(QColor(Qt::green)) ;

    for( iPoint=0; iPoint<nbPoints; iPoint++ )
    {
        xPos = points[iPoint].x ;
        yPos = points[iPoint].y ;

        pRectItem = new QGraphicsRectItem( xPos, yPos, TSP_POINT_SIZE, TSP_POINT_SIZE, NULL, &m_scene ) ;
        pRectItem->setZValue( 2.0 ) ;
        pRectItem->setPen( pen ) ;
        pRectItem->setBrush( brush ) ;

        /*
        QGraphicsSimpleTextItem *pTextItem = new QGraphicsSimpleTextItem( QString::number(iPoint), NULL, &m_scene ) ;
        pTextItem->setPos( xPos+1.5, yPos ) ;
        pTextItem->setFont( font ) ;
        */
    }
}

//----------------------------------------------------------------------------------
void
TSPMainWindow::refreshView( void )
{
    refreshSceneRect() ;
    refreshPath() ;
    refreshVertices() ;


    // scene bounding rect
    int sceneWidth = ui->pQt_spinSceneWidth->value() ;
    int sceneHeight = ui->pQt_spinSceneHeight->value() ;

    ui->pQt_graphicsView->fitInView( 0.0, 0.0, sceneWidth, sceneHeight, Qt::KeepAspectRatio ) ;
}

//----------------------------------------------------------------------------------
bool
TSPMainWindow::openCSVFile( QString path )
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
            TSPLib::instance()->setInput( nbPoints, points ) ;

            // free memory
            delete[] points ;

            // refresh view
            refreshView() ;
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

//----------------------------------------------------------------------------------
void
TSPMainWindow::Update( TSPLibObserved * pObserved )
{
    TSPLib *pLib = dynamic_cast<TSPLib*>(pObserved) ;

    qApp->processEvents() ;
    refreshPath() ;
}
