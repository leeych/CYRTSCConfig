#include "signalerstatuswidget.h"
#include "macrostring.h"
#include <QLabel>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>

SignalerStatusWidget::SignalerStatusWidget(const QString &name, QWidget *parent)
    :QWidget(parent), widget_name_(name)
{
    signaler_edit_dlg_ = new SignalerbasiceditDlg;
    InitPage();
    InitSignalSlots();
    UpdateTable();
}

const QString &SignalerStatusWidget::widget_name()
{
    return widget_name_;
}

void SignalerStatusWidget::OnOkButtonClicked()
{
}

void SignalerStatusWidget::OnCancelButtonClicked()
{
}

void SignalerStatusWidget::OnAddActionClicked()
{
}

void SignalerStatusWidget::OnEditActionClicked()
{
    if (signaler_edit_dlg_!= NULL)
    {
        signaler_edit_dlg_->Initialize();
    }
}

void SignalerStatusWidget::OnDeleteActionClicked()
{
}

void SignalerStatusWidget::OnCustomContextMenuRequested(QPoint)
{
    if (context_menu_ != NULL)
    {
        context_menu_->exec(QCursor::pos());
    }
}

void SignalerStatusWidget::InitPage()
{
    InitContextMenu();
    InitTable();
    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addWidget(signaler_table_);
    setLayout(vlayout);
}

void SignalerStatusWidget::InitSignalSlots()
{
    connect(signaler_table_, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(OnCustomContextMenuRequested(QPoint)));
    connect(add_action_, SIGNAL(triggered()), this, SLOT(OnAddActionClicked()));
    connect(edit_action_, SIGNAL(triggered()), this, SLOT(OnEditActionClicked()));
    connect(delete_action_, SIGNAL(triggered()), this, SLOT(OnDeleteActionClicked()));
}

void SignalerStatusWidget::InitTable()
{
    signaler_table_ = new QTableWidget;
    signaler_table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    signaler_table_->setSelectionBehavior(QTableWidget::SelectRows);
    signaler_table_->setSelectionMode(QAbstractItemView::SingleSelection);
    signaler_table_->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: rgb(184, 219, 255)}");
    signaler_table_->horizontalHeader()->setClickable(false);

    QPalette pal;
    pal.setColor(QPalette::Base, QColor(233, 246, 254));
    signaler_table_->setPalette(pal);
    signaler_table_->verticalHeader()->setHidden(true);
    signaler_table_->clearFocus();
    signaler_table_->setShowGrid(true);
    signaler_table_->horizontalHeader()->setStretchLastSection(true);
	signaler_table_->setContextMenuPolicy(Qt::CustomContextMenu);

    InitTableHeader();
}

void SignalerStatusWidget::InitTableHeader()
{
    signaler_table_->setColumnCount(9);
    signaler_table_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    int width = size().width();
    signaler_table_->setColumnWidth(0, width / 9);
    signaler_table_->setColumnWidth(1, width / 9);
    signaler_table_->setColumnWidth(2, width / 9);
    signaler_table_->setColumnWidth(3, width / 9);
    signaler_table_->setColumnWidth(4, width / 9);
    signaler_table_->setColumnWidth(5, width / 9);
    signaler_table_->setColumnWidth(6, width / 9);
    signaler_table_->setColumnWidth(7, width / 9);
    signaler_table_->setColumnWidth(8, width / 9);
    QStringList headers;
    headers << STRING_UI_SIGNALER_STATUS << STRING_UI_SIGNALER_NAME << STRING_UI_SIGNALER_IP
            << STRING_UI_SIGNALER_PORT << STRING_UI_SIGNALER_POSITION << STRING_UI_SIGNALER_BRANCH
               << STRING_UI_SIGNALER_MODE << STRING_UI_SIGNALER_VERSION << STRING_UI_SIGNALER_MARKUP;
    signaler_table_->setHorizontalHeaderLabels(headers);
}

void SignalerStatusWidget::InitContextMenu()
{
    context_menu_ = new QMenu;

    add_action_ = new QAction(STRING_UI_ADD, context_menu_);
    edit_action_ = new QAction(STRING_UI_EDIT, context_menu_);
    delete_action_ = new QAction(STRING_UI_DELETE, context_menu_);

    context_menu_->addAction(add_action_);
    context_menu_->addAction(edit_action_);
    context_menu_->addAction(delete_action_);
    QString menu_qss = "QMenu{background-color: rgb(255,255,255);border: 1px solid rgb(164,209,251)}"
                           "QMenu::item{background-color:transparent;color:#000000}"
                           "QMenu::item:selected{background-color:rgb(164,209,251);color:rgb(255,101,3)}";
    context_menu_->setStyleSheet(menu_qss);
}

void SignalerStatusWidget::UpdateTable()
{
    int table_row = 10;
    QString str;
    signaler_table_->setRowCount(table_row);
    for (int i = 0; i < table_row; i++)
    {
        QTableWidgetItem* item_status = new QTableWidgetItem;
        str = "ok";
        item_status->setText(str);
        item_status->setTextAlignment(Qt::AlignCenter);
        signaler_table_->setItem(i, 0, item_status);

        QTableWidgetItem* item_name = new QTableWidgetItem;
        str = "signaler" + i;
        item_name->setText(str);
        item_name->setTextAlignment(Qt::AlignCenter);
        signaler_table_->setItem(i, 1, item_name);

        QTableWidgetItem* item_ip = new QTableWidgetItem;
        str = "192.168.1.100";
        item_ip->setText(str);
        item_ip->setTextAlignment(Qt::AlignCenter);
        signaler_table_->setItem(i, 2, item_ip);

        QTableWidgetItem* item_port = new QTableWidgetItem;
        str = "12032";
        item_port->setText(str);
        item_port->setTextAlignment(Qt::AlignCenter);
        signaler_table_->setItem(i, 3, item_port);

        QTableWidgetItem* item_position = new QTableWidgetItem;
        str = "local";
        item_position->setText(str);
        item_position->setTextAlignment(Qt::AlignCenter);
        signaler_table_->setItem(i, 4, item_position);

        QTableWidgetItem* item_branch = new QTableWidgetItem;
        str.sprintf("%d", rand() % 10);
        item_branch->setText(str);
        item_branch->setTextAlignment(Qt::AlignCenter);
        signaler_table_->setItem(i, 5, item_branch);

        QTableWidgetItem* item_mode = new QTableWidgetItem;
        str = "1.0";
        item_mode->setText(str);
        item_mode->setTextAlignment(Qt::AlignCenter);
        signaler_table_->setItem(i, 6, item_mode);

        QTableWidgetItem* item_version = new QTableWidgetItem;
        str = "1.0.0";
        item_version->setText(str);
        item_version->setTextAlignment(Qt::AlignCenter);
        signaler_table_->setItem(i, 7, item_version);

        QTableWidgetItem* item_mark = new QTableWidgetItem;
        str = "";
        item_mark->setText(str);
        item_mark->setTextAlignment(Qt::AlignCenter);
        signaler_table_->setItem(i, 8, item_mark);
    }
}

SignalerStatusWidget::~SignalerStatusWidget()
{

}
