
#ifndef _TSPLIB_H_
#define _TSPLIB_H_

#include "tsplib_global.h"
#include <stdlib.h>


class TSPLIB_EXPORT TSPLib
{
    protected :

            static int m_nbPoints ;
            static Point *m_points ;
            static int m_nbEdges ;
            static int *m_path ;

            static bool *m_freePoints ;
            static int *m_bestPoints ;
            static double *m_bestPointsDistance ;

    public :

            static void setInput( int nbPoints, Point * points ) ;
            static void computePath( void ) ;
            static int getNbEdges( void ) ;
            static int* getPath( void ) ;
            static int getNbPoints( void ) ;
            static Point* getPoints( void ) ;

            static void computeEnvelope( void ) ;
            static void reduceEnvelope( int nbSteps ) ;

            static double getPathLength( void ) ;

    protected :

            static void freeMemory( void ) ;
            static void updateFreePoints( void ) ;
            static void computeNearestPoints( int iStart=0, int nbEdges=1 ) ;
            static int getEdgeWithLowerCost( void ) ;
            static void computeRandomPath( void ) ;
            static void printPath( void );
            static void printFreePoints( void ) ;
            static bool intersect( int idx1, int idx2, int idx3, int idx4 ) ;
} ;

#endif // _TSPLIB_H_
