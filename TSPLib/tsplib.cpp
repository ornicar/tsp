
#include "tsplib.h"


int TSPLib::m_nbPoints = 0 ;
Point *TSPLib::m_points = NULL ;
int **TSPLib::m_edges = NULL ;


//----------------------------------------------------------------------------------
void
TSPLib::freeMemory( void )
{
    if( m_nbPoints>0 )
    {
        if( m_edges!=NULL )
        {
            int i ;
            for( i=0; i<m_nbPoints; i++ )
            {
                delete[] m_edges[i] ;
            }
            delete[] m_edges ;
            m_edges = NULL ;
        }


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
        m_edges = new int*[m_nbPoints] ;

        int i ;
        for( i=0; i<m_nbPoints; i++ )
        {
            // copy points location
            m_points[i].x = points[i].x ;
            m_points[i].y = points[i].y ;

            // allocate memory for edges
            m_edges[i] = new int[2] ;
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
TSPLib::getConfiguration( void )
{
    return NULL ;
}

//----------------------------------------------------------------------------------
void
TSPLib::computeEnveloppe( void )
{
}
