
#include "tsplib.h"
#include "graham.cpp"
#include "xlines.h"
#include <math.h>


TSPLib* TSPLib::p_instance = NULL ;

//----------------------------------------------------------------------------------
TSPLib::TSPLib( void )
{
    m_nbPoints = 0 ;
    m_nbEdges = 0 ;
    m_points = NULL ;
    m_path = NULL ;

    m_freePoints = NULL ;
    m_edge2Point = NULL ;
}

//----------------------------------------------------------------------------------
TSPLib::~TSPLib( void )
{
    freeMemory() ;
}

//----------------------------------------------------------------------------------
TSPLib*
TSPLib::instance( void )
{
    if( p_instance==NULL )
    {
        p_instance = new TSPLib() ;
    }

    return p_instance ;
}

//----------------------------------------------------------------------------------
void
TSPLib::computePath( void )
{
    // remove any existing path
    m_nbEdges = 0 ;

    // compute enveloppe
    computeEnvelope() ;

    // reduce enveloppe
    reduceEnvelope( m_nbPoints ) ;

    // optimize path
    localOptimization() ;
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
        // path
        delete[] m_path ;
        m_path = NULL ;
        m_nbEdges = 0 ;

        // points
        delete[] m_points ;
        m_points = NULL ;
        m_nbPoints = 0 ;

        // free points
        delete[] m_freePoints ;
        m_freePoints = NULL ;

        // edge to point distance map
        for( int i=0; i<m_nbPoints; i++ )
        {
            delete[] m_edge2Point[i] ;
        }
        delete[] m_edge2Point ;
        m_edge2Point = NULL ;
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
        m_freePoints = new bool[m_nbPoints] ;
        m_edge2Point = new double*[m_nbPoints] ;
        m_path = new int[m_nbPoints] ;

        int i ;
        for( i=0; i<m_nbPoints; i++ )
        {
            // copy points location
            m_points[i].x = points[i].x ;
            m_points[i].y = points[i].y ;

            // at beginning every point is free
            m_freePoints[i] = true ;

            // allocate 2nd dimension of distance map array
            m_edge2Point[i] = new double[m_nbPoints] ;
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
    for(i=0; i<m_nbPoints; i++)
    {
        if( m_freePoints[i]==true )
        {
            fprintf( stderr, "%d, ", i);
        }
    }
    fprintf( stderr, "\n" ) ;
}

//----------------------------------------------------------------------------------
bool
TSPLib::intersect( int idx1, int idx2, int idx3, int idx4 )
{
    double x, y ;

    int result = lines_intersect(   m_points[idx1].x, m_points[idx1].y,
                                    m_points[idx2].x, m_points[idx2].y,
                                    m_points[idx3].x, m_points[idx3].y,
                                    m_points[idx4].x, m_points[idx4].y,
                                    x, y ) ;
    if( result==DO_INTERSECT )
    {
        return true ;
    }

    return false ;
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

    if( m_path!=NULL )
    {
        delete[] m_path ;
        m_path = NULL ;
    }
    m_path = g.getPath(m_nbEdges) ;

    // update free points
    updateFreePoints() ;

    // compute nearest point for each edge
    computeNearestPoints( 0, m_nbEdges ) ;
}

//----------------------------------------------------------------------------------
void
TSPLib::updateFreePoints( void )
{
    int iPoint ;
    for( iPoint=0; iPoint<m_nbPoints; iPoint++ )
    {
        m_freePoints[iPoint] = true ;
    }
    for( iPoint=0; iPoint<m_nbEdges; iPoint++ )
    {
        m_freePoints[m_path[iPoint]] = false ;
    }
}

//----------------------------------------------------------------------------------
double
TSPLib::computeDistance( int idx1, int idx2 )
{
    double dx = m_points[idx2].x - m_points[idx1].x ;
    double dy = m_points[idx2].y - m_points[idx1].y ;
    return sqrt( (dx*dx)+(dy*dy) ) ;
}

//----------------------------------------------------------------------------------
void
TSPLib::computeNearestPoints( int iStart, int nbEdges )
{
    //
    int iEdge, idx1, idx2, idx3, idx4, iPoint ;
    double dist0, dist1, dist2, computedDistance ;

    int i ;
    for( i=0; i<nbEdges; i++ )
    {
        iEdge = (iStart+i)%m_nbEdges ;

        // retrieve current indices
        idx1 = m_path[(iEdge)%m_nbEdges] ;
        idx2 = m_path[(iEdge+1)%m_nbEdges] ;

        // compute current distance of edge
        dist0 = computeDistance( idx1, idx2 ) ;

        // look for best free point
        for( iPoint=0; iPoint<m_nbPoints; iPoint++ )
        {
            // init min distance
            computedDistance = TSP_DISTANCE_MAX ;

            if( m_freePoints[iPoint]==true )
            {
                // test intersection : no intersection allowed
                int iOtherEdge ;
                bool intersection = false ;
                for( iOtherEdge=0; iOtherEdge<m_nbEdges; iOtherEdge++ )
                {
                    if( iOtherEdge!=iEdge )
                    {
                        idx3 = m_path[iOtherEdge] ;
                        idx4 = m_path[(iOtherEdge+1)%m_nbEdges] ;
                        if( (idx1!=idx3) && (idx1!=idx4) && (idx2!=idx3) && (idx2!=idx4) )
                        {
                            if( intersect( idx1, iPoint, idx3, idx4 )==true ||
                                intersect( iPoint, idx2, idx3, idx4 )==true )
                            {
                                intersection = true ;
                                break ;
                            }
                        }
                    }
                }

                if( intersection==false )
                {
                    // compute from idx1 to free point
                    dist1 = computeDistance( idx1, iPoint ) ;

                    // compute from free point to idx2
                    dist2 = computeDistance( iPoint, idx2 ) ;

                    // computed distance is (potentially new distance) minus (actual distance)
                    computedDistance = (dist1+dist2)-(dist0) ;
                }
            }

            // save best free point
            m_edge2Point[iEdge][iPoint] = computedDistance ;
        }
    }
}

//----------------------------------------------------------------------------------
void
TSPLib::removeFreePoint( int pointToRemove )
{
    // remove from free points set
    m_freePoints[pointToRemove] = false ;

    // update best points from concerned edges
    int iEdge ;
    for( iEdge=0; iEdge<m_nbEdges; iEdge++)
    {
        m_edge2Point[iEdge][pointToRemove] = TSP_DISTANCE_MAX ;
    }
}

//----------------------------------------------------------------------------------
int
TSPLib::getEdgeWithLowerCost( int & nearestPoint )
{
    int iEdge, iPoint, edgeWithLowerCost = -1 ;
    double distance, min = TSP_DISTANCE_MAX ;

    nearestPoint = -1 ;

    for( iEdge=0; iEdge<m_nbEdges; iEdge++ )
    {
        for( iPoint=0; iPoint<m_nbPoints; iPoint++ )
        {
            distance = m_edge2Point[iEdge][iPoint] ;

            if( distance<min )
            {
                min = distance ;
                edgeWithLowerCost = iEdge ;
                nearestPoint = iPoint ;
            }
        }
    }

    return edgeWithLowerCost ;
}

//----------------------------------------------------------------------------------
void
TSPLib::reduceEnvelope( int nbSteps )
{
    while( nbSteps>0 && m_nbEdges<m_nbPoints )
    {
        // get edge to split
        int nearestPoint ;
        int edgeToSplit = getEdgeWithLowerCost( nearestPoint ) ;

        if( edgeToSplit>=0 )
        {
            // split the edge
            //---------------
            // shift left edges (and related stuff) one step ahead 'til edge to split
            int iEdge ;
            for( iEdge=m_nbEdges; iEdge>edgeToSplit+1; iEdge-- )
            {
                m_path[iEdge] = m_path[iEdge-1] ;

                // TO DO : décaler les nearest points calculés un cran à droite
                compilation bug ;
            }
            // insert new point
            m_path[edgeToSplit+1] = nearestPoint ;
            // update edge count
            m_nbEdges++ ;

            if( (m_nbEdges%15)==0 )
            {
                // notify update
                Notify() ;
            }

            // this point is not free anymore
            removeFreePoint( nearestPoint ) ;

            // compute nearest points for the 2 new edges
            computeNearestPoints( edgeToSplit, 2 ) ;

            // decrease step count
            nbSteps-- ;
        }
        else
        {
            // no edge to split
            break ;
        }
    }
}


//----------------------------------------------------------------------------------
void
TSPLib::localOptimization( void )
{
    if( m_nbEdges>4 )
    {
        int iEdge, idx1, idx2, idx3, idx4 ;
        double distNow, distIf ;

        for( iEdge=0; iEdge<m_nbEdges; iEdge++ )
        {
            idx1 = m_path[iEdge] ;
            idx2 = m_path[(iEdge+1)%m_nbEdges] ;
            idx3 = m_path[(iEdge+2)%m_nbEdges] ;
            idx4 = m_path[(iEdge+3)%m_nbEdges] ;

            // compute actual distance
            distNow =   computeDistance( idx1, idx2 ) +
                        computeDistance( idx2, idx3 ) +
                        computeDistance( idx3, idx4 ) ;

            // compute distance if we swapped 2 points in path
            distIf =    computeDistance( idx1, idx3 ) +
                        computeDistance( idx3, idx2 ) +
                        computeDistance( idx2, idx4 ) ;

            //
            if( distIf<distNow )
            {
                if( intersect( idx1, idx3, idx2, idx4 )==false )
                {
                    // swap
                    m_path[(iEdge+1)%m_nbEdges] = idx3 ;
                    m_path[(iEdge+2)%m_nbEdges] = idx2 ;
                }
            }
        }
    }
}
