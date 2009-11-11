
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
            static int *m_path ;

    public :

            static void setInput( int nbPoints, Point * points ) ;
            static int* getPath( void ) ;
            static int getNbPoints( void ) ;
            static Point* getPoints( void ) ;

    protected :

            static void freeMemory( void ) ;
            static void computeEnveloppe( void ) ;
            static void computePath( void ) ;
            static void computeRandomPath( void ) ;
            static void printPath( void );
} ;

#endif // _TSPLIB_H_
