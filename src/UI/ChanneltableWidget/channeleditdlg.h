#ifndef CHANNELEDITDLG_H
#define CHANNELEDITDLG_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include "channelhandler.h"

class ChanneleditDlg : public QDialog
{
    Q_OBJECT
public:
    explicit ChanneleditDlg(QWidget *parent = 0);
	~ChanneleditDlg();
    void Initialize(unsigned char channel_id, ChannelHandler* handler);
    void SetHandler(ChannelHandler* handler);

signals:
    void updateTableRowSignal(unsigned char channel_id);
    
public slots:
    void OnOkButtonClicked();
    void OnCancelButtonClicked();
    
private:
    void InitPage();
    void InitSignalSlots();
    void UpdateUI();
    void ResetUI();

    bool SaveData();
    unsigned char SaveChannelType();
    unsigned char SaveChannelFlash();
	unsigned char get_channel_direction();
	unsigned char get_channel_lane();

private:
    unsigned char curr_channel_id_;
    ChannelHandler* handler_;

private:
    QComboBox* channel_id_cmb_, *ctrl_mode_cmb_;
    QComboBox* direction_cmb_, *lane_mode_cmb_;
    QPushButton *ok_button_, *cancel_button_;

    QCheckBox* yellow_flash_chk_, *red_flash_chk_, *alter_flash_chk_;
};

#endif // CHANNELEDITDLG_H
