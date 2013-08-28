#include "phaseconflicteditdlg.h"
#include "macrostring.h"
#include <QGridLayout>
#include <QGroupBox>
#include <QDebug>

PhaseconflicteditDlg::PhaseconflicteditDlg(QWidget *parent) :
    QDialog(parent)
{
    InitPage();
    InitSignalSlots();
}

void PhaseconflicteditDlg::Initialize(unsigned char phase_id, PhaseconflictHandler *handler)
{
    current_phase_id_ = phase_id;
    handler_ = handler;
    UpdateUI();
    exec();
}

void PhaseconflicteditDlg::OnOkButtonClicked()
{
    if (SaveData())
    {
        accept();
        emit updateTableRowSignal(current_phase_id_);
    }
}

void PhaseconflicteditDlg::OnCancelButtonClicked()
{
    close();
}

void PhaseconflicteditDlg::InitPage()
{
	setWindowTitle(STRING_UI_PHASE_CONFLICT + "-" + STRING_UI_EDIT);
    QLabel* curr_phase_label = new QLabel(STRING_UI_PHASE_CURRENT_ID);
    QGroupBox* conflict_grp = new QGroupBox(STRING_UI_PHASE_CONFLICT_CONID);
    curr_phase_label_ = new QLabel("1");

    QString str;
    for (int i = 1; i <= 32; i++)
    {
        str.sprintf("%d", i);
        QCheckBox* chk = new QCheckBox(str);
        conflict_list_.append(chk);
    }
    QGridLayout* grid_layout = new QGridLayout;
    int cnt = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (cnt < conflict_list_.size())
            {
                grid_layout->addWidget(conflict_list_.at(cnt), i, j, 1, 1);
                cnt++;
            }
        }
    }

    ok_button_ = new QPushButton(STRING_OK);
    cancel_button_ = new QPushButton(STRING_CANCEL);

    QHBoxLayout* curr_hlayout = new QHBoxLayout;
    curr_hlayout->addWidget(curr_phase_label);
    curr_hlayout->addWidget(curr_phase_label_);
    curr_hlayout->addStretch(2);
    curr_hlayout->addWidget(ok_button_);
    curr_hlayout->addWidget(cancel_button_);

    conflict_grp->setLayout(grid_layout);
    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addLayout(curr_hlayout);
    vlayout->addWidget(conflict_grp);
    setLayout(vlayout);
}

void PhaseconflicteditDlg::InitSignalSlots()
{
    connect(ok_button_, SIGNAL(clicked()), this, SLOT(OnOkButtonClicked()));
    connect(cancel_button_, SIGNAL(clicked()), this, SLOT(OnCancelButtonClicked()));
}

void PhaseconflicteditDlg::UpdateUI()
{
    ResetUI();
    QString str;
    str.sprintf("%d", current_phase_id_);
    curr_phase_label_->setText(str);
    if (current_phase_id_ > 0)
    {
        conflict_list_.at(current_phase_id_ - 1)->setEnabled(false);
    }

    PhaseConflictParam phase_err;
    if (!handler_->get_phase_conflict(current_phase_id_, phase_err))
    {
        return;
    }

    // rotate left 32
    unsigned int conflict_ids = phase_err.conflict_phase_id;
    for (int i = 31; i >= 0; i--)
    {
        if ((conflict_ids & 0x080000000) == 0x080000000)
        {
            conflict_list_.at(i)->setChecked(true);
        }
        conflict_ids = conflict_ids << 1;
    }
}

void PhaseconflicteditDlg::ResetUI()
{
    for (int i = 0; i < conflict_list_.size(); i++)
    {
        conflict_list_.at(i)->setChecked(false);
        conflict_list_.at(i)->setEnabled(true);
    }
}

bool PhaseconflicteditDlg::SaveData()
{
    unsigned int conflict_ids = 0;
    for (int i = 0; i < 32; i++)
    {
        conflict_ids = conflict_ids >> 1;
        if (conflict_list_.at(i)->isChecked())
        {
            conflict_ids |= 0x080000000;
            qDebug("%X", conflict_ids);
        }
        qDebug("%X", conflict_ids);
    }
    handler_->set_phase_conflict(current_phase_id_, conflict_ids);
    return true;
}

PhaseconflicteditDlg::~PhaseconflicteditDlg()
{

}
