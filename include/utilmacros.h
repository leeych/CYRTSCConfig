#ifndef UTILMACROS_H
#define UTILMACROS_H
#include <QApplication>
#include <QDesktopWidget>


#ifdef WIDGET_CENTER
#define WIDGET_CENTRALIZE(widget) \
    QDesktopWidget *desktop = QApplication::desktop(); \
    widget->move((desktop->width()-widget->width())/2,(desktop->height()-widget->height())/2);
#else
#define WIDGET_CENTRALIZE(widget)
#endif

#endif // UTILMACROS_H
