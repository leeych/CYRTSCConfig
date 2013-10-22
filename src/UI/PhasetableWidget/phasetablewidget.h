#ifndef PHASETABLEWIDGET_H
#define PHASETABLEWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QMenu>
//#include "phasetablewidget_global.h"
#include "phasehandler.h"


class PhaseeditDlg;
class BottomButtonsWidget;

class /*PHASETABLEWIDGETSHARED_EXPORT*/ PhasetableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PhasetableWidget(const QString& name, QWidget* parent = 0);
	~PhasetableWidget();
    const QString &widget_name();
    void UpdateTree();

signals:
    void updateChannelCtrlsrcSignal();
    void phaseEditedSignal();

public slots:
    void OnTreeItemDoubleClicked(QTreeWidgetItem*,int);

    void OnAddActionClicked();
    void OnEditActionClicked();
    void OnDeleteActionClicked();
    void OnSaveActionClicked();
    void OnUpdateTreeItemSlot(unsigned char id);
    void OnCustomContextMenuRequested(QPoint);

    void OnUpdateDataSlot();
    void OnInitDatabase(void *ptr);

    void updateSlot();

private:
    void InitPage();
    void InitSignalSlots();

    void InitTree();
    void InitContextMenu();

	QString get_phase_type_desc(unsigned char phase_type);
	QString get_phase_ctrled_channels_desc(unsigned int channel_ids);

private:
    QString widget_name_;
    PhaseHandler* handler_;

private:
    QMenu* context_menu_;
    QAction* add_action_, *edit_action_, *delete_action_, *save_action_;

    PhaseeditDlg* phase_edit_dlg_;
    BottomButtonsWidget* buttons_widget_;
    QTreeWidget* tree_widget_;
};

#endif // PHASETABLEWIDGET_H
