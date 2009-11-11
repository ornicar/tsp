
#include "tsplib.h"


int TSPLib::m_nbPoints = 0 ;
Point *TSPLib::m_points = NULL ;
int *TSPLib::m_path = NULL ;


//--------hihi!!-breuzablorg--------------------------------------------------------------------------
void
TSPLib::freeMemory( void )
{
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
void
TSPLib::computePath( void )
{
}

//----------------------------------------------------------------------------------
int*
TSPLib::getPath( void )
{
    return m_path ;
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
