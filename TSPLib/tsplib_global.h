#ifndef _TSPLIB_GLOBAL_H_
#define _TSPLIB_GLOBAL_H_

#include <QtCore/QtGlobal>


#if defined(TSPLIB_LIBRARY)
#  define TSPLIB_EXPORT Q_DECL_EXPORT
#else
#  define TSPLIB_EXPORT Q_DECL_IMPORT
#endif

#define TSP_DISTANCE_MAX    (999999.0)

typedef struct tPoint
{
    double x ;
    double y ;
} Point ;

#endif
