#include <QtCore/QtGlobal>


#if defined(TSPLIB_LIBRARY)
#  define TSPLIB_EXPORT Q_DECL_EXPORT
#else
#  define TSPLIB_EXPORT Q_DECL_IMPORT
#endif
