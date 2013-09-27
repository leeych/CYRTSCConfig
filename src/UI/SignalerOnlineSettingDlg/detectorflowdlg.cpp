#include "detectorflowdlg.h"
#include "macrostring.h"
#include "synccommand.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>


DetectorFlowDlg::DetectorFlowDlg(QWidget *parent) :
    QDialog(parent)
{
    sync_cmd_ = SyncCommand::GetInstance();
    InitPage();
    InitSignalSlots();
}

DetectorFlowDlg::~DetectorFlowDlg()
{
}

void DetectorFlowDlg::Initialize()
{
    UpdateUI();
    exec();
}

void DetectorFlowDlg::OnReadFlowButtonClicked()
{
    sync_cmd_->GetDetectorFlowData(this, SLOT(OnCmdGetDetectorData(QByteArray&)));
}

void DetectorFlowDlg::OnClearFowButtonClicked()
{
    sync_cmd_->ClearDetectorFlowInfo(this, SLOT(OnCmdClearDetectorInfo(QByteArray&)));
}

void DetectorFlowDlg::OnOkButtonClicked()
{
    accept();
}

void DetectorFlowDlg::OnCmdGetDetectorData(QByteArray &array)
{
    Q_UNUSED(array);
}

void DetectorFlowDlg::OnCmdClearDetectorInfo(QByteArray &array)
{
    Q_UNUSED(array);
}

void DetectorFlowDlg::OnCmdGetDetectorRealTimeInfo(QByteArray &array)
{
    Q_UNUSED(array);
}

void DetectorFlowDlg::OnConnectError(const QString &str)
{
    QMessageBox::information(this, STRING_TIP, str, STRING_OK);
    return;
}

void DetectorFlowDlg::InitPage()
{
    setWindowTitle(STRING_UI_SIGNALER_DETECTOR_TITLE);

    detector_tree_ = new QTreeWidget;
    QStringList detector_header;
    detector_header << STRING_UI_SIGNALER_DETECTOR_ID;
    InitTree(detector_tree_, detector_header);
//    detector_tree_->setMaximumWidth(100);

    flow_tree_ = new QTreeWidget;
    QStringList flow_header;
    flow_header << STRING_UI_SIGNALER_DETECTOR_FLOW << STRING_UI_SIGNALER_DETECTOR_DATETIME
                   << STRING_UI_SIGNALER_DETECTOR_NUM << STRING_UI_SIGNALER_DETECTOR_PHASE;
    InitTree(flow_tree_, flow_header);
    start_time_editor_ = new QDateTimeEdit;
    end_time_editor_ = new QDateTimeEdit;
    QLabel *total_flow_label = new QLabel(STRING_UI_SIGNALER_DETECTOR_TOTAL_FLOW + ":");
    total_flow_lineedit_ = new QLineEdit;
    total_flow_lineedit_->setContextMenuPolicy(Qt::NoContextMenu);

    QHBoxLayout *timeedit_hlayout = new QHBoxLayout;
    timeedit_hlayout->addWidget(start_time_editor_);
    timeedit_hlayout->addWidget(new QLabel("--"));
    timeedit_hlayout->addWidget(end_time_editor_);
//    timeedit_hlayout->addStretch(1);
    timeedit_hlayout->addWidget(total_flow_label);
    timeedit_hlayout->addWidget(total_flow_lineedit_);

    QVBoxLayout *right_vlayout = new QVBoxLayout;
    right_vlayout->addWidget(flow_tree_);
    right_vlayout->addLayout(timeedit_hlayout);

    QHBoxLayout *tree_hlayout = new QHBoxLayout;
    tree_hlayout->addWidget(detector_tree_);
    tree_hlayout->addLayout(right_vlayout);

    tree_hlayout->setStretch(0, 1);
    tree_hlayout->setStretch(1, 3);

    read_flow_button_ = new QPushButton(STRING_UI_SGINALER_DETECTOR_READ_FLOW);
    clear_flow_button_ = new QPushButton(STRING_UI_SIGNALER_DETECTOR_CLEAR_FLOW);
    ok_button_ = new QPushButton(STRING_OK);

    QHBoxLayout *button_hlayout = new QHBoxLayout;
    button_hlayout->addWidget(read_flow_button_);
    button_hlayout->addWidget(clear_flow_button_);
    button_hlayout->addStretch(1);
    button_hlayout->addWidget(ok_button_);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(tree_hlayout);
    vlayout->addLayout(button_hlayout);
    setLayout(vlayout);
}

void DetectorFlowDlg::InitSignalSlots()
{
    connect(read_flow_button_, SIGNAL(clicked()), this, SLOT(OnReadFlowButtonClicked()));
    connect(clear_flow_button_, SIGNAL(clicked()), this, SLOT(OnClearFowButtonClicked()));
    connect(ok_button_, SIGNAL(clicked()), this, SLOT(OnOkButtonClicked()));

    connect(SyncCommand::GetInstance(), SIGNAL(connectErrorStrSignal(QString)), this, SLOT(OnConnectError(QString)));
}

void DetectorFlowDlg::UpdateUI()
{
}

void DetectorFlowDlg::InitTree(QTreeWidget *tree, const QStringList &header)
{
    int col_count = header.size();
    tree->setColumnCount(col_count);
    tree->setHeaderLabels(header);

    tree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tree->setSelectionBehavior(QTreeWidget::SelectRows);
    tree->setSelectionMode(QAbstractItemView::SingleSelection);
    QPalette pal;
    pal.setColor(QPalette::Base, QColor(233, 246, 254));
    tree->setPalette(pal);
    tree->clearFocus();
    tree->setStyleSheet("QHeaderView::section{background-color: rgb(184, 219, 255); text-align:center;}");
    QHeaderView *header_view = tree->header();
    header_view->setDefaultAlignment(Qt::AlignCenter);
}
