#ifndef SIGNALERSTATUSWIDGET_H
#define SIGNALERSTATUSWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QMenu>
#include <QAction>
#include "signalerstatuswidget_global.h"
#include "buttonwidget.h"
#include "signalerparam.h"


class SignalerbasiceditDlg;
class SignalerHandler;
class SignalerOnlineSettingDlg;

class SIGNALERSTATUSWIDGETSHARED_EXPORT SignalerStatusWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SignalerStatusWidget(const QString& name, QWidget* parent = 0);
	~SignalerStatusWidget();
    const QString& widget_name();
    void Initialize();
    void UpdateTable();

signals:

public slots:
    void OnOkButtonClicked();
    void OnCancelButtonClicked();

    void OnAddActionClicked();
    void OnEditActionClicked();
    void OnDeleteActionClicked();
    void OnSaveActionClicked();
    void OnAdvancedActionClicked();
    void OnCustomContextMenuRequested(QPoint);
    void OnTableCellDoubleClicked(int, int);
    void OnTableRowUpdateSlot(int);

    void OnConnectedSlot();
    void OnDisconnectedSlot();

private:
    void InitPage();
    void InitSignalSlots();

    void InitTable();
    void InitTableHeader();
    void AddTableRow(int index, const SignalerParam &signaler);
    void InitContextMenu();

    QString trimmed(QString &str);
    QString get_status_desc(SignalerParam::SignalerStatus status);
    QColor get_status_text_color(SignalerParam::SignalerStatus status);

private:
    QString widget_name_;
    SignalerHandler *handler_;

private:
    SignalerbasiceditDlg* signaler_edit_dlg_;
    SignalerOnlineSettingDlg *signaler_online_dlg_;

    QTableWidget* signaler_table_;
    QPushButton *ok_button_, *cancel_button_;
    ButtonWidget *button_widget_;

    QMenu* context_menu_;
    QAction* add_action_, *edit_action_, *delete_action_, *advanced_button_;
};

#endif // SIGNALERSTATUSWIDGET_H
