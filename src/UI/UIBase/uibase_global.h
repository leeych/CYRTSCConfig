#ifndef UIBASE_GLOBAL_H
#define UIBASE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(UIBASE_LIBRARY)
#  define UIBASESHARED_EXPORT Q_DECL_EXPORT
#else
#  define UIBASESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // UIBASE_GLOBAL_H
