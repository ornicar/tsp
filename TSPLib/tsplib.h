
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
            static int **m_edges ;

    public :

            static void setInput( int nbPoints, Point * points ) ;
            static void computePath( void ) ;
            static int* getConfiguration( void ) ;

    protected :

            static void freeMemory( void ) ;
            static void computeEnveloppe( void ) ;
} ;

#endif // _TSPLIB_H_
