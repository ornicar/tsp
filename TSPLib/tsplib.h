
#ifndef _TSPLIB_H_
#define _TSPLIB_H_

#include "tsplib_global.h"
#include <stdlib.h>


typedef struct tPoint
{
    double x ;
    double y ;
} Point ;


class TSPLIB_EXPORT TSPLib
{
    protected :

            static int m_nbPoints ;
            static Point *m_points ;
            static int m_nbEdges ;
            static int *m_path ;

            static int m_nbFreePoints ;
            static int *m_freePoints ;
            static int *m_bestPoints ;
            static double *m_bestPointsDistance ;

    public :

            static void setInput( int nbPoints, Point * points ) ;
            static void computePath( void ) ;
            static int getNbEdges( void ) ;
            static int* getPath( void ) ;
            static int getNbPoints( void ) ;
            static Point* getPoints( void ) ;

    protected :

            static void freeMemory( void ) ;
            static void computeEnveloppe( void ) ;
            static void reduceEnveloppe( void ) ;
            static void updateFreePointsSet( void ) ;
            static void computeNearestPoints( int iStart=0, int nbEdges=1 ) ;
            static int getEdgeWithLowerCost( void ) ;
            static void computeRandomPath( void ) ;
            static void printPath( void );
} ;

#endif // _TSPLIB_H_
