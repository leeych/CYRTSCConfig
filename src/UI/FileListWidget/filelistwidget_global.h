#ifndef FILELISTWIDGET_GLOBAL_H
#define FILELISTWIDGET_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FILELISTWIDGET_LIBRARY)
#  define FILELISTWIDGETSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FILELISTWIDGETSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FILELISTWIDGET_GLOBAL_H
