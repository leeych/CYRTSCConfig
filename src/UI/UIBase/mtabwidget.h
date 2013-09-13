#ifndef MTABWIDGET_H
#define MTABWIDGET_H

#include <QTabWidget>
#include <QTabBar>
#include <QToolButton>
#include "uibase_global.h"

class UIBASESHARED_EXPORT MTabCloseButton : public QToolButton
{
    Q_OBJECT

public:
    explicit MTabCloseButton(QWidget *parent = 0);

signals:
    void clicked(QWidget* widget);

public slots:
    void onClicked();
};

class MTabBar : public QTabBar
{
    Q_OBJECT

public:
    explicit MTabBar(QWidget *parent = 0);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *);
    virtual QSize tabSizeHint(int index) const;
};

class MTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit MTabWidget(QWidget *parent = 0);
    void setDefaultTabName(const QString &strName);
    
    void addTabEx(QWidget *widget, const QString &str);
    void changeTab(const QString &strTabName);
    QString currentTabName();

    QString tabName(int index);
	void setTabBarFont(const QFont &font);

signals:
    void closeTabSignal(const QString& name);

public slots:
    void closeTab(int index);
    void closeButtonClicked(QWidget* widget);

private:
    MTabBar* tabBar;
    QString strDefaultTabName;
};

#endif // MTABWIDGET_H
