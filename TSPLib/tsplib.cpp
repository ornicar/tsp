
#include "tsplib.h"
#include "graham.cpp"
#include <math.h>

int TSPLib::m_nbPoints = 0 ;
int TSPLib::m_nbEdges = 0 ;
Point *TSPLib::m_points = NULL ;
int *TSPLib::m_path = NULL ;

int TSPLib::m_nbFreePoints = 0 ;
int *TSPLib::m_freePoints = NULL ;
int *TSPLib::m_bestPoints = NULL ;
double *TSPLib::m_bestPointsDistance = NULL ;



//----------------------------------------------------------------------------------
void
TSPLib::computePath( void )
{
    // compute enveloppe
    computeEnvelope();

    // reduce enveloppe
    reduceEnvelope( m_nbPoints ) ;
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
    m_nbEdges = m_nbPoints ;
}

//----------------------------------------------------------------------------------
double
TSPLib::getPathLength( void )
{
    if ( m_nbEdges < 2 )
    {
        return 0.0;
    }

    int i;
    double length = 0.0, dx, dy;
    Point point1, point2;
    for(i=0; i<m_nbEdges; i++)
    {
        point1 = m_points[m_path[i]] ;
        point2 = m_points[m_path[(i+1)%m_nbEdges]] ;

        dx = point2.x - point1.x ;
        dy = point2.y - point1.y ;
        length += sqrt( (dx*dx)+(dy*dy) ) ;
    }

    return length;
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
        m_nbEdges = 0 ;

        delete[] m_points ;
        m_points = NULL ;
        m_nbPoints = 0 ;

        delete[] m_freePoints ;
        m_freePoints = NULL ;
        m_nbFreePoints = 0 ;

        delete[] m_bestPoints ;
        m_bestPoints = NULL ;

        delete[] m_bestPointsDistance ;
        m_bestPointsDistance = NULL ;
    }
}

//----------------------------------------------------------------------------------
void
TSPLib::setInput( int nbPoints, Point * points )
{
    // free previously allocated memory
    freeMemory() ;

    m_nbFreePoints = m_nbPoints = nbPoints ;

    if( m_nbPoints>0 )
    {
        // allocate memory
        m_points = new Point[m_nbPoints] ;
        m_freePoints = new int[m_nbPoints] ;
        m_bestPoints = new int[m_nbPoints] ;
        m_bestPointsDistance = new double[m_nbPoints] ;
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
    return m_path ;
}

//----------------------------------------------------------------------------------
void
TSPLib::printPath( void )
{
    int i ;
    fprintf( stderr, "[TSPLib::printPath] --------------------------\n" ) ;
    for(i=0; i<m_nbEdges; i++)
    {
        fprintf( stderr, "%d, ", m_path[i]);
    }
    fprintf( stderr, "\n" ) ;
}

//----------------------------------------------------------------------------------
void
TSPLib::printFreePoints( void )
{
    int i ;
    fprintf( stderr, "[TSPLib::printFreePoints] --------------------------\n" ) ;
    for(i=0; i<m_nbFreePoints; i++)
    {
        fprintf( stderr, "%d, ", m_freePoints[i]);
    }
    fprintf( stderr, "\n" ) ;
}

//----------------------------------------------------------------------------------
int
TSPLib::getNbEdges( void )
{
    return m_nbEdges ;
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
TSPLib::computeEnvelope( void )
{
    GrahamScan g( m_nbPoints, m_points );

    g.partition_points();

    g.build_hull();

    m_path = g.getPath(m_nbEdges);

    // compute nearest point for each edge
    computeNearestPoints( 0, m_nbEdges ) ;
}

//----------------------------------------------------------------------------------
void
TSPLib::updateFreePointsSet( void )
{
    bool isFree ;
    int iEdge, iPoint ;
    m_nbFreePoints=0 ;

    // for each point we have
    for( iPoint=0; iPoint<m_nbPoints; iPoint++ )
    {
        isFree = true ;

        // look if an edge uses it
        for( iEdge=0; iEdge<m_nbEdges; iEdge++ )
        {
            if( m_path[iEdge]==iPoint )
            {
                isFree = false ;
                break ;
            }
        }

        // if not, its added to free points set
        if( isFree==true )
        {
            m_freePoints[m_nbFreePoints] = iPoint ;
            m_nbFreePoints++ ;
        }
    }
}

//----------------------------------------------------------------------------------
void
TSPLib::computeNearestPoints( int iStart, int nbEdges )
{
    // retrieve free points
    updateFreePointsSet() ;
    printFreePoints() ;

    //
    int iPoint, idx1, idx2, idxFree, iFreePoint, bestPoint ;
    double dx, dy, dist0, dist1, dist2, min ;

    for( iPoint=iStart; iPoint<nbEdges; iPoint++ )
    {
        // init min distance
        min = 9999999.0 ;
        bestPoint = -1 ;

        // retrieve current indices
        idx1 = m_path[iPoint] ;
        idx2 = m_path[(iPoint+1)%m_nbEdges] ;

        // compute current distance of edge (squared)
        dx = m_points[idx2].x - m_points[idx1].x ;
        dy = m_points[idx2].y - m_points[idx1].y ;
        dist0 = sqrt( (dx*dx)+(dy*dy) ) ;

        // look for best free point
        for( iFreePoint=0; iFreePoint<m_nbFreePoints; iFreePoint++ )
        {
            idxFree = m_freePoints[iFreePoint] ;

            // compute from idx1 to free point
            dx = m_points[idxFree].x - m_points[idx1].x ;
            dy = m_points[idxFree].y - m_points[idx1].y ;
            dist1 = sqrt( (dx*dx)+(dy*dy) ) ;

            // compute from free point to idx2
            dx = m_points[idx2].x - m_points[idxFree].x ;
            dy = m_points[idx2].y - m_points[idxFree].y ;
            dist2 = sqrt( (dx*dx)+(dy*dy) ) ;

            // save best result
            if( min>(dist1+dist2-dist0) )
            {
                min = dist1+dist2-dist0 ;
                bestPoint = idxFree ;
            }
        }

        // save best free point
        fprintf( stderr, "[TSPLib::computeNearestPoints] best for edge[%d,%d] = %d with %f\n", idx1, idx2, bestPoint, min ) ;
        m_bestPoints[iPoint] = bestPoint ;
        m_bestPointsDistance[iPoint] = min ;
    }
}

//----------------------------------------------------------------------------------
int
TSPLib::getEdgeWithLowerCost( void )
{
    int iCost, result = -1 ;
    double min = 999999.0 ;

    for( iCost=0; iCost<m_nbEdges; iCost++ )
    {
        if( m_bestPointsDistance[iCost]<min )
        {
            min = m_bestPointsDistance[iCost] ;
            result = iCost ;
        }
    }

    return result ;
}

//----------------------------------------------------------------------------------
void
TSPLib::reduceEnvelope( int nbSteps )
{
    while( nbSteps>0 && m_nbEdges<m_nbPoints )
    {
        // get edge to split
        int iEdge = getEdgeWithLowerCost() ;

        printPath();
        fprintf( stderr, "[TSPLib::reduceEnvelope] best edge = [%d %d]\n", m_path[iEdge], m_path[(iEdge+1)%m_nbEdges] ) ;
        if( m_nbEdges==m_nbPoints-1 )
        {
            fprintf( stderr, "[TSPLib::reduceEnvelope] last point\n" ) ;
        }

        if( iEdge>=0 )
        {
            // split the edge
            //---------------
            int iPoint ;
            printPath();
            // shift left edges and related stuff
            for( iPoint=m_nbEdges; iPoint>iEdge+1; iPoint-- )
            {
                m_path[iPoint] = m_path[iPoint-1] ;

                m_bestPoints[iPoint] = m_bestPoints[iPoint-1] ;
                m_bestPointsDistance[iPoint] = m_bestPointsDistance[iPoint-1] ;
            }
            // insert new point
            m_path[iEdge+1] = m_bestPoints[iEdge] ;

            // update edge count
            m_nbEdges++ ;
            printPath();

            // decrease step count
            nbSteps-- ;

            // update free points set
            //updateFreePointsSet() ;

            // compute nearest points for the 2 new edges
            //computeNearestPoints( iEdge, 1 ) ;
            //computeNearestPoints( iEdge+1, 1 ) ;
            computeNearestPoints( 0, m_nbEdges ) ;
        }
        else
        {
            // no edge to split
            break ;
        }
    }
}
