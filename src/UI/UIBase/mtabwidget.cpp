#include <QtGui>
#include "mtabwidget.h"
#include "mutility.h"


MTabWidget::MTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    tabBar = new MTabBar;
    tabBar->setFocusPolicy(Qt::NoFocus);
    setTabBar(tabBar);
    QString dir;
    MUtility::getMainDir(dir);
    setStyleSheet(QLatin1String(MUtility::fetchQrc(dir + "tabwidget.qss")));
//    setStyleSheet("background-color:RGB(233, 246, 254);");
    connect(this,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
}

void MTabWidget::setDefaultTabName(const QString &strName)
{
    strDefaultTabName = strName;
}

void MTabWidget::addTabEx(QWidget *widget, const QString & str)
{
    int idx = addTab(widget,str);
    MTabCloseButton* closeButton = new MTabCloseButton();
    tabBar->setTabButton(idx, QTabBar::RightSide,((QWidget*)(closeButton)));
    connect(closeButton,SIGNAL(clicked(QWidget*)),this,SLOT(closeButtonClicked(QWidget*)));
}

void MTabWidget::changeTab(const QString &strTabName)
{
    int tabNount = count();
    if(tabNount == 0)
    {
        return;
    }
    for(int i = 0;i < tabNount;i++)
    {
        if(strTabName == tabText(i))
        {
            setCurrentIndex(i);
            break;
        }
    }
}

QString MTabWidget::currentTabName()
{
    int idx = currentIndex();
    if(idx != -1)
    {
        QString text = tabText(idx);
        return text;
    }

    return QString("");
}

QString MTabWidget::tabName(int index)
{
    if (index >= 0 && index < this->count())
    {
        return tabText(index);
    }
    return QString("");
}

void MTabWidget::closeTab(int index)
{
    QString str = tabText(index);
    if(str != strDefaultTabName)
    {
        removeTab(index);
    }
    emit closeTabSignal(str);
}

void MTabWidget::closeButtonClicked(QWidget *widget)
{
    int c = count();
    for(int i = 0;i < c;i++)
    {
        if(tabBar->tabButton(i, QTabBar::RightSide) == widget)
        {
            QString str = tabText(i);
            removeTab(i);
            emit closeTabSignal(str);
            break;
        }
    }
}

MTabBar::MTabBar(QWidget *parent) :
    QTabBar(parent)
{
}

void MTabBar::paintEvent(QPaintEvent* pe)
{
    QTabBar::paintEvent(pe);
}

QSize MTabBar::tabSizeHint(int index) const
{
    return QTabBar::tabSizeHint(index);
}

void MTabCloseButton::onClicked()
{
    emit clicked(this);
}

MTabCloseButton::MTabCloseButton(QWidget *parent)
    :QToolButton(parent)
{
    QString imageDir;
    MUtility::getImageDir(imageDir);
    QString closeButtonQss =
            "QToolButton:!hover:!pressed{border-image:url(" + imageDir + "X1.png)}"
            "QToolButton:hover{border-image:url(" + imageDir + "X3.png)}"
            "QToolButton:pressed{border-image:url(" + imageDir + "X2.png)}";
    setStyleSheet(closeButtonQss);
    setFixedWidth(16);
    setFixedHeight(16);
    connect(this,SIGNAL(clicked()),this,SLOT(onClicked()));
}
