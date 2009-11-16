
#ifndef _TSPLIB_H_
#define _TSPLIB_H_

#include "tsplib_global.h"
#include "tsplibobserver.h"
#include <stdlib.h>


class TSPLIB_EXPORT TSPLib : public TSPLibObserved
{
    protected :

            static  TSPLib* p_instance ;

            int      m_nbPoints ;
            Point*   m_points ;
            int      m_nbEdges ;
            int*     m_path ;

            bool*    m_freePoints ;
            double** m_edge2Point ;

    protected :

                    TSPLib( void ) ;
                    ~TSPLib( void ) ;

    public :

    static  TSPLib* instance( void ) ;

            void     setInput( int nbPoints, Point * points ) ;
            void     computePath( void ) ;
            int      getNbEdges( void ) ;
            int*     getPath( void ) ;
            int      getNbPoints( void ) ;
            Point*   getPoints( void ) ;
            double   getPathLength( void ) ;

    protected :

            void     freeMemory( void ) ;
            void     computeEnvelope( void ) ;
            void     reduceEnvelope( int nbSteps ) ;
            void     localOptimization( void ) ;
            void     updateFreePoints( void ) ;
            void     computeNearestPoints( int iStart=0, int nbEdges=1 ) ;
            void     removeFreePoint( int pointToRemove ) ;
            double   computeDistance( int idx1, int idx2 ) ;
            int      getEdgeWithLowerCost( int & nearestPoint ) ;
            void     printPath( void );
            void     printFreePoints( void ) ;
            bool     intersect( int idx1, int idx2, int idx3, int idx4 ) ;
} ;

#endif // _TSPLIB_H_
