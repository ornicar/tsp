
#include "tsplib.h"


int TSPLib::m_nbPoints = 0 ;
Point *TSPLib::m_points = NULL ;
int *TSPLib::m_path = NULL ;



//----------------------------------------------------------------------------------
void
TSPLib::computePath( void )
{
    computeRandomPath();
}

//----------------------------------------------------------------------------------
void
TSPLib::computeRandomPath( void )
{
    m_path = new int[m_nbPoints] ;
    int i ;
    for(i=0; i<m_nbPoints; i++)
    {
        m_path[i] = i;
    }
}

//----------------------------------------------------------------------------------
void
TSPLib::freeMemory( void )
{
    // free memory
    if( m_nbPoints>0 )
    {
        delete[] m_path ;
        m_path = NULL ;

        delete[] m_points ;
        m_points = NULL ;
        m_nbPoints = 0 ;
    }
}

//----------------------------------------------------------------------------------
void
TSPLib::setInput( int nbPoints, Point * points )
{
    // free previously allocated memory
    freeMemory() ;

    m_nbPoints = nbPoints ;

    if( m_nbPoints>0 )
    {
        // allocate memory
        m_points = new Point[m_nbPoints] ;
        m_path = new int[m_nbPoints] ;

        int i ;
        for( i=0; i<m_nbPoints; i++ )
        {
            // copy points location
            m_points[i].x = points[i].x ;
            m_points[i].y = points[i].y ;
        }
    }
}

//----------------------------------------------------------------------------------
int*
TSPLib::getPath( void )
{
    computeRandomPath();

    printPath();

    return m_path ;
}

//----------------------------------------------------------------------------------
void
TSPLib::printPath( void )
{
    int i ;
    for(i=0; i<m_nbPoints; i++)
    {
        qDebug("%d, ", m_path[i]);
    }
}

//----------------------------------------------------------------------------------
Point*
TSPLib::getPoints( void )
{
    return m_points ;
}

//----------------------------------------------------------------------------------
int
TSPLib::getNbPoints( void )
{
    return m_nbPoints ;
}

//----------------------------------------------------------------------------------
void
TSPLib::computeEnveloppe( void )
{
}
