#include "signalerstatuswidget.h"
#include "macrostring.h"

#include "signalerbasiceditdlg.h"
#include "signalerhandler.h"
#include "signaleronlinesettingdlg.h"
#include "synccommand.h"

#include <QLabel>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

SignalerStatusWidget::SignalerStatusWidget(const QString &name, QWidget *parent)
    :QWidget(parent), widget_name_(name)
{
    signaler_edit_dlg_ = new SignalerbasiceditDlg(this);
    signaler_online_dlg_ = new SignalerOnlineSettingDlg(this);
    handler_ = new SignalerHandler;

    InitPage();
    InitSignalSlots();
//    UpdateTable();
}

const QString &SignalerStatusWidget::widget_name()
{
    return widget_name_;
}

void SignalerStatusWidget::Initialize()
{
    handler_->init();
    UpdateTable();
}

void SignalerStatusWidget::OnOkButtonClicked()
{
    QMessageBox::information(this, STRING_TIP, "OK", STRING_OK);
}

void SignalerStatusWidget::OnCancelButtonClicked()
{
    QMessageBox::information(this, STRING_TIP, "Cancel", STRING_OK);
}

void SignalerStatusWidget::OnAddActionClicked()
{
    int table_row = signaler_table_->rowCount();
    signaler_table_->setRowCount(table_row + 1);
    QString str("-");
    for (int i = 0; i < signaler_table_->columnCount(); i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(str);
        item->setTextAlignment(Qt::AlignCenter);
        signaler_table_->setItem(table_row, i, item);
    }
}

void SignalerStatusWidget::OnEditActionClicked()
{
    int row = signaler_table_->currentRow();
    if (row < 0)
    {
        return;
    }
    int id = signaler_table_->item(row, 0)->text().toInt();
    if (signaler_edit_dlg_ == NULL)
    {
        return;
    }
    signaler_edit_dlg_->Initialize(id, handler_);
}

void SignalerStatusWidget::OnDeleteActionClicked()
{
    int row = signaler_table_->currentRow();
    if (row < 0)
    {
        return;
    }
    int id = signaler_table_->item(row, 0)->text().toInt();
    if (handler_->remove_signaler(id) || id == 0)
    {
        signaler_table_->removeRow(row);
    }
    else
    {
        QMessageBox::warning(this, STRING_WARNING, STRING_UI_SIGNALER_DELETE_FAILED, STRING_OK);
    }
}

void SignalerStatusWidget::OnSaveActionClicked()
{
    if (!handler_->save_data())
    {
		QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_SAVE_FAILED + "!", STRING_OK);
    }
}

void SignalerStatusWidget::OnAdvancedActionClicked()
{
	int row = signaler_table_->currentRow();
	if (row < 0)
	{
		return;
	}
	int id = signaler_table_->item(row, 0)->text().toUInt();
    if (signaler_online_dlg_ == NULL || id == 0)
	{
		return;
	}
    QString ip = signaler_table_->item(row, 3)->text().trimmed();
    ip = Trimmed(ip);
    unsigned int port = signaler_table_->item(row, 4)->text().toUInt();
    signaler_online_dlg_->Initialize(ip, port);
}

void SignalerStatusWidget::OnCustomContextMenuRequested(QPoint)
{
    if (context_menu_ != NULL)
    {
        context_menu_->exec(QCursor::pos());
    }
}

void SignalerStatusWidget::OnTableCellDoubleClicked(int row, int col)
{
    if (col == 1)
    {
        QString ip = signaler_table_->item(row, 3)->text().trimmed();
        unsigned int port = signaler_table_->item(row, 4)->text().toUInt();
        signaler_online_dlg_->Initialize(ip, port);
        return;
    }
    int id = signaler_table_->item(row, 0)->text().toInt();
    if (signaler_online_dlg_ == NULL)
    {
        return;
    }
    signaler_edit_dlg_->Initialize(id, handler_);
}

void SignalerStatusWidget::OnTableRowUpdateSlot(int)
{
    UpdateTable();
}

void SignalerStatusWidget::OnConnectedSlot()
{
    qDebug() << "connected with signaler";
    int row = signaler_table_->currentRow();
    if (row < 0)
    {
        return;
    }
    int id = signaler_table_->item(row, 0)->text().toInt();
    handler_->set_signaler_status(id, SignalerParam::Online);
    QTableWidgetItem *item_status = signaler_table_->item(row, 1);
    item_status->setText(STRING_UI_ONLINE);
    item_status->setTextColor(get_status_text_color(SignalerParam::Online));
}

void SignalerStatusWidget::OnDisconnectedSlot()
{
    qDebug() << "disconnect from signaler";
    int row = signaler_table_->currentRow();
    if (row < 0)
    {
        return;
    }
    int id = signaler_table_->item(row, 0)->text().toInt();
    handler_->set_signaler_status(id, SignalerParam::Offline);
    QTableWidgetItem *item_status = signaler_table_->item(row, 1);
    item_status->setText(STRING_UI_OFFLINE);
    item_status->setTextColor(get_status_text_color(SignalerParam::Offline));
}

void SignalerStatusWidget::InitPage()
{
    InitContextMenu();
    InitTable();
    button_widget_ = new ButtonWidget;
    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addWidget(signaler_table_);
    vlayout->addWidget(button_widget_);
    setLayout(vlayout);
}

void SignalerStatusWidget::InitSignalSlots()
{
    connect(signaler_table_, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(OnCustomContextMenuRequested(QPoint)));
    connect(add_action_, SIGNAL(triggered()), this, SLOT(OnAddActionClicked()));
    connect(edit_action_, SIGNAL(triggered()), this, SLOT(OnEditActionClicked()));
    connect(delete_action_, SIGNAL(triggered()), this, SLOT(OnDeleteActionClicked()));
    connect(advanced_button_, SIGNAL(triggered()), this, SLOT(OnAdvancedActionClicked()));

    connect(button_widget_->addButtonPtr(), SIGNAL(clicked()), this, SLOT(OnAddActionClicked()));
    connect(button_widget_->editButtonPtr(), SIGNAL(clicked()), this, SLOT(OnEditActionClicked()));
    connect(button_widget_->delButtonPtr(), SIGNAL(clicked()), this, SLOT(OnDeleteActionClicked()));
    connect(button_widget_->saveButtonPtr(), SIGNAL(clicked()), this, SLOT(OnSaveActionClicked()));
    connect(button_widget_->advancedButtonPtr(), SIGNAL(clicked()), this, SLOT(OnAdvancedActionClicked()));

    connect(signaler_table_, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(OnTableCellDoubleClicked(int,int)));
    connect(signaler_edit_dlg_, SIGNAL(updateSignalerSignal(int)), this, SLOT(OnTableRowUpdateSlot(int)));

    connect(SyncCommand::GetInstance(), SIGNAL(connectedSignal()), this, SLOT(OnConnectedSlot()));
    connect(SyncCommand::GetInstance(), SIGNAL(disconnectedSignal()), this, SLOT(OnDisconnectedSlot()));
    connect(SyncCommand::GetInstance(), SIGNAL(connectedSignal()), signaler_online_dlg_, SLOT(OnConnectedSlot()));
    connect(SyncCommand::GetInstance(), SIGNAL(disconnectedSignal()), signaler_online_dlg_, SLOT(OnDisconnectedSlot()));
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
    signaler_table_->setColumnCount(10);
    signaler_table_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    int width = size().width();
    signaler_table_->setColumnWidth(0, 1);
    signaler_table_->setColumnWidth(1, width / 9);
    signaler_table_->setColumnWidth(2, width / 9);
    signaler_table_->setColumnWidth(3, width / 9);
    signaler_table_->setColumnWidth(4, width / 9);
    signaler_table_->setColumnWidth(5, width / 9);
    signaler_table_->setColumnWidth(6, width / 9);
    signaler_table_->setColumnWidth(7, width / 9);
    signaler_table_->setColumnWidth(8, width / 9);
    signaler_table_->setColumnWidth(9, width / 9);
    signaler_table_->setColumnHidden(0, true);
    QStringList headers;
    headers << "0" << STRING_UI_SIGNALER_STATUS << STRING_UI_SIGNALER_NAME << STRING_UI_SIGNALER_IP
            << STRING_UI_SIGNALER_PORT << STRING_UI_SIGNALER_POSITION << STRING_UI_SIGNALER_BRANCH
               << STRING_UI_SIGNALER_MODE << STRING_UI_SIGNALER_VERSION << STRING_UI_SIGNALER_MARKUP;
    signaler_table_->setHorizontalHeaderLabels(headers);
}

void SignalerStatusWidget::AddTableRow(int index, const SignalerParam &signaler)
{
    Q_UNUSED(index);
    Q_UNUSED(signaler);
}

void SignalerStatusWidget::InitContextMenu()
{
    context_menu_ = new QMenu;

    add_action_ = new QAction(STRING_UI_ADD, context_menu_);
    edit_action_ = new QAction(STRING_UI_EDIT, context_menu_);
    delete_action_ = new QAction(STRING_UI_DELETE, context_menu_);
    advanced_button_ = new QAction(STRING_UI_ADVANCED, context_menu_);

    context_menu_->addAction(add_action_);
    context_menu_->addAction(edit_action_);
    context_menu_->addAction(delete_action_);
    context_menu_->addAction(advanced_button_);
    QString menu_qss = "QMenu{background-color: rgb(255,255,255);border: 1px solid rgb(164,209,251)}"
                    "QMenu::item{background-color:transparent;color:#000000}"
                    "QMenu::item:selected{background-color:rgb(164,209,251);color:rgb(255,101,3)}";
    context_menu_->setStyleSheet(menu_qss);
}

QString SignalerStatusWidget::Trimmed(QString &str)
{
    int index = str.indexOf(" ");
    if (index < 0)
    {
        return str;
    }
    str.remove(index, 1);
    return Trimmed(str);
}

QString SignalerStatusWidget::get_status_desc(SignalerParam::SignalerStatus status)
{
    QString str = "-";
    if (status == SignalerParam::Online)
    {
        str = STRING_UI_ONLINE;
    }
    else
    {
        str = STRING_UI_OFFLINE;
    }
    return str;
}

QColor SignalerStatusWidget::get_status_text_color(SignalerParam::SignalerStatus status)
{
    if (status == SignalerParam::Online)
    {
        return QColor(0, 255, 0);
    }
    else
    {
        return QColor(255, 0, 0);
    }
}

void SignalerStatusWidget::UpdateTable()
{
	signaler_table_->clear();
	InitTableHeader();
    QList<SignalerParam> signaler_list = handler_->get_signaler_list();
    int table_row = signaler_list.size();
    QString str;
    signaler_table_->setRowCount(table_row);
    for (int i = 0; i < table_row; i++)
    {
        QTableWidgetItem *item_id = new QTableWidgetItem;
        str = QString::number(signaler_list.at(i).signaler_id);
        item_id->setText(str);
        item_id->setTextAlignment(Qt::AlignCenter);
        signaler_table_->setItem(i, 0, item_id);

        QTableWidgetItem* item_status = new QTableWidgetItem;
        str = get_status_desc(signaler_list.at(i).signaler_status);
        item_status->setText(str);
        item_status->setTextColor(get_status_text_color(signaler_list.at(i).signaler_status));
        item_status->setTextAlignment(Qt::AlignCenter);
        signaler_table_->setItem(i, 1, item_status);

        QTableWidgetItem* item_name = new QTableWidgetItem;
        str = signaler_list.at(i).signaler_name;
        item_name->setText(str);
        item_name->setTextAlignment(Qt::AlignCenter);
        signaler_table_->setItem(i, 2, item_name);

        QTableWidgetItem* item_ip = new QTableWidgetItem;
        str = signaler_list.at(i).signaler_ip;
        item_ip->setText(str);
        item_ip->setTextAlignment(Qt::AlignCenter);
        signaler_table_->setItem(i, 3, item_ip);

        QTableWidgetItem* item_port = new QTableWidgetItem;
        str = QString::number(signaler_list.at(i).signaler_port);
        item_port->setText(str);
        item_port->setTextAlignment(Qt::AlignCenter);
        signaler_table_->setItem(i, 4, item_port);

        QTableWidgetItem* item_position = new QTableWidgetItem;
        str = signaler_list.at(i).location_desc;
        item_position->setText(str);
        item_position->setTextAlignment(Qt::AlignCenter);
        signaler_table_->setItem(i, 5, item_position);

        QTableWidgetItem* item_branch = new QTableWidgetItem;
        str = signaler_list.at(i).branch_road;
        item_branch->setText(str);
        item_branch->setTextAlignment(Qt::AlignCenter);
        signaler_table_->setItem(i, 6, item_branch);

        QTableWidgetItem* item_mode = new QTableWidgetItem;
        str = signaler_list.at(i).signaler_mode;
        item_mode->setText(str);
        item_mode->setTextAlignment(Qt::AlignCenter);
        signaler_table_->setItem(i, 7, item_mode);

        QTableWidgetItem* item_version = new QTableWidgetItem;
        str = signaler_list.at(i).software_version;
        item_version->setText(str);
        item_version->setTextAlignment(Qt::AlignCenter);
        signaler_table_->setItem(i, 8, item_version);

        QTableWidgetItem* item_mark = new QTableWidgetItem;
        str = signaler_list.at(i).remark;
        item_mark->setText(str);
        item_mark->setTextAlignment(Qt::AlignCenter);
        signaler_table_->setItem(i, 9, item_mark);
    }
}

SignalerStatusWidget::~SignalerStatusWidget()
{

}
