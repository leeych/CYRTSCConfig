#include "signalerbasiceditdlg.h"
#include "macrostring.h"
#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

SignalerbasiceditDlg::SignalerbasiceditDlg(QWidget *parent) :
    QDialog(parent)
{
    InitPage();
    InitSignalSlots();
}

void SignalerbasiceditDlg::Initialize(int signaler_id, SignalerHandler *handler)
{
    curr_signaler_id_ = signaler_id;
    handler_ = handler;
    UpdateUI();
    exec();
}

void SignalerbasiceditDlg::InitPage()
{
    QLabel* id_label = new QLabel(STRING_UI_SIGNALER_ID);
    id_label_ = new QLabel("12");

    QLabel* name_label = new QLabel(STRING_UI_SIGNALER_NAME);
    QLabel* ip_label = new QLabel(STRING_UI_SIGNALER_IP);
    QLabel* port_label = new QLabel(STRING_UI_SIGNALER_PORT);
    QLabel* pos_label = new QLabel(STRING_UI_SIGNALER_POSITION);
    QLabel* branch_label = new QLabel(STRING_UI_SIGNALER_BRANCH);
    QLabel* mode_label = new QLabel(STRING_UI_SIGNALER_MODE);
    QLabel* version_label = new QLabel(STRING_UI_SIGNALER_VERSION);
    QLabel* mark_label = new QLabel(STRING_UI_SIGNALER_MARKUP);

    name_lineedit_ = new QLineEdit;
    ip_lineedit_ = new QLineEdit;
    port_lineedit_ = new QLineEdit;
    pos_lineedit_ = new QLineEdit;
    branch_lineedit_ = new QLineEdit;
    mode_lineedit_ = new QLineEdit;
    version_lineedit_ = new QLineEdit;
    markup_lineedit_ = new QLineEdit;

    ok_button_ = new QPushButton(STRING_OK);
    cancel_button_ = new QPushButton(STRING_CANCEL);

    QGroupBox* set_grp = new QGroupBox(STRING_UI_SIGNALER_PARAM_SET);
    QHBoxLayout* fir_hlayout = new QHBoxLayout;
    fir_hlayout->addWidget(id_label);
    fir_hlayout->addWidget(id_label_);
    fir_hlayout->addStretch(3);

    QGridLayout* glayout = new QGridLayout;
    glayout->addWidget(name_label, 0, 0, 1, 1);
    glayout->addWidget(name_lineedit_, 0, 1, 1, 3);
    glayout->addWidget(ip_label, 1, 0, 1, 1);
    glayout->addWidget(ip_lineedit_, 1, 1, 1, 1);
    glayout->addWidget(port_label, 1, 2, 1, 1);
    glayout->addWidget(port_lineedit_, 1, 3, 1, 1);
    glayout->addWidget(pos_label, 2, 0, 1, 1);
    glayout->addWidget(pos_lineedit_, 2, 1, 1, 3);
    glayout->addWidget(branch_label, 3, 0, 1, 1);
    glayout->addWidget(branch_lineedit_, 3, 1, 1, 1);
    glayout->addWidget(mode_label, 4, 0, 1, 1);
    glayout->addWidget(mode_lineedit_, 4, 1, 1, 1);
    glayout->addWidget(version_label, 5, 0, 1, 1);
    glayout->addWidget(version_lineedit_, 5, 1, 1, 3);
    glayout->addWidget(mark_label, 6, 0, 1, 1);
    glayout->addWidget(markup_lineedit_, 6, 1, 1, 3);
    set_grp->setLayout(glayout);

    QHBoxLayout* button_hlayout = new QHBoxLayout;
    button_hlayout->addStretch(2);
    button_hlayout->addWidget(ok_button_);
    button_hlayout->addWidget(cancel_button_);

    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addLayout(fir_hlayout);
    vlayout->addWidget(set_grp);
    vlayout->addLayout(button_hlayout);
    setLayout(vlayout);
}

void SignalerbasiceditDlg::InitSignalSlots()
{
}

void SignalerbasiceditDlg::UpdateUI()
{
}

SignalerbasiceditDlg::~SignalerbasiceditDlg()
{

}
