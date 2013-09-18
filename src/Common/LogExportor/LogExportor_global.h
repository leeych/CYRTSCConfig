#ifndef LOGEXPORTOR_GLOBAL_H
#define LOGEXPORTOR_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LOGEXPORTOR_LIBRARY)
#  define LOGEXPORTORSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LOGEXPORTORSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LOGEXPORTOR_GLOBAL_H
