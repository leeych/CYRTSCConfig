#include <QtGui>
#include "mtablewidget.h"

MTableWidget::MTableWidget(QWidget *parent)
    : QScrollArea(parent), widget_name_("")
{
    InitTable();
}

QString &MTableWidget::widget_name()
{
    return widget_name_;
}

void MTableWidget::setAddButtonName(int itype)
{
    Q_UNUSED(itype);
}

void MTableWidget::CreateTable(int w)
{
    Q_UNUSED(w);
}

void MTableWidget::UpdateList()
{
}

void MTableWidget::SetRowHighlight(int id)
{
    if(id == UN_INITID_INT)
    {
        return;
    }

    int rowcount = main_table_->rowCount();

    QTableWidgetItem* tmpitem;
    for(int i = 0;i < rowcount;i++)
    {
        tmpitem = main_table_->item(i, 0);
        if(tmpitem != 0)
        {
            if(tmpitem->text().toInt() == id)
            {
                main_table_->setCurrentCell(i,0);
                break;
            }
        }
    }
}

void MTableWidget::resizeEvent(QResizeEvent *e)
{
    QScrollArea::resizeEvent(e);
    int wunit = e->size().width()/88;
    main_table_->setColumnWidth(0, wunit*3);
    main_table_->setColumnWidth(1,  wunit*9);
    main_table_->setColumnWidth(2,  wunit*9);
    main_table_->setColumnWidth(3,  wunit*4);
    main_table_->setColumnWidth(4,  wunit*5);
    main_table_->setColumnWidth(5,  wunit*8);
    main_table_->setColumnWidth(6,  wunit*8);
    main_table_->setColumnWidth(7,  wunit*8);
    main_table_->setColumnWidth(8,  wunit*9);
    main_table_->setColumnWidth(9,  wunit*4);
    main_table_->setColumnWidth(10,  wunit*4);
    main_table_->setColumnWidth(11,  wunit*3);
    main_table_->setColumnWidth(12,  wunit*9);
    main_table_->setColumnWidth(13,  e->size().width() - wunit*83);
}

void MTableWidget::onCustomContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    if(popmenu_ != NULL)
    {
        popmenu_->exec(QCursor::pos());
    }
}

void MTableWidget::InitTable()
{
    main_table_ = new QTableWidget;
    main_table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    main_table_->setSelectionBehavior(QTableWidget::SelectRows);
    main_table_->setSelectionMode(QAbstractItemView::SingleSelection);
    //header's background color
    main_table_->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: rgb(184, 219, 255)}");
    main_table_->horizontalHeader()->setClickable(false);

    QPalette pal;
    pal.setColor(QPalette::Base, QColor(233, 246, 254));
    main_table_->setPalette(pal);
    main_table_->setStyleSheet("font:12px");
//    table->setAlternatingRowColors(true);
    //remove row number
    main_table_->verticalHeader()->setHidden(true);
    main_table_->setContextMenuPolicy(Qt::CustomContextMenu);
    main_table_->clearFocus();
    setStyleSheet("border : 0px;");
}
