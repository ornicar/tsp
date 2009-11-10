#include "tspmainwindow.h"
#include "ui_tspmainwindow.h"
#include "tsplib.h"

#include <QFileDialog>
#include <QTextStream>


//----------------------------------------------------------------------------------
TSPMainWindow::TSPMainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::TSPMainWindow)
{
    ui->setupUi(this);

    //
    connect(ui->pQt_actionOpen, SIGNAL(triggered()), this, SLOT(openFile())) ;
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
    TSPLib::computePath() ;
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
            while( stream.atEnd()==false )
            {
                QString line = stream.readLine() ;
                QStringList coords = line.split( ';', QString::SkipEmptyParts ) ;
                points[iPoint].x = coords.at(0).toDouble() ;
                points[iPoint].y = coords.at(1).toDouble() ;

                iPoint++ ;
            }

            //
            TSPLib::setInput( nbPoints, points ) ;

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
