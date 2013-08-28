#ifndef SIGNALERSTATUSWIDGET_H
#define SIGNALERSTATUSWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QMenu>
#include <QAction>
#include "signalerstatuswidget_global.h"
#include "signalerbasiceditdlg.h"

class SIGNALERSTATUSWIDGETSHARED_EXPORT SignalerStatusWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SignalerStatusWidget(const QString& name, QWidget* parent = 0);
	~SignalerStatusWidget();
    const QString& widget_name();
    void UpdateTable();

signals:

public slots:
    void OnOkButtonClicked();
    void OnCancelButtonClicked();

    void OnAddActionClicked();
    void OnEditActionClicked();
    void OnDeleteActionClicked();
    void OnCustomContextMenuRequested(QPoint);

private:
    void InitPage();
    void InitSignalSlots();

    void InitTable();
    void InitTableHeader();

    void InitContextMenu();

private:
    QString widget_name_;

private:
    SignalerbasiceditDlg* signaler_edit_dlg_;
    QMenu* context_menu_;
    QAction* add_action_, *edit_action_, *delete_action_;

    QTableWidget* signaler_table_;
    QPushButton* ok_button_, *cancel_button_;
};

#endif // SIGNALERSTATUSWIDGET_H
