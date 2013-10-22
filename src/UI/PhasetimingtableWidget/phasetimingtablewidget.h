#ifndef PHASETIMINGTABLEWIDGET_H
#define PHASETIMINGTABLEWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QMenu>
#include <QAction>
//#include "phasetimingtablewidget_global.h"
#include "phasetiminghandler.h"

class BottomButtonsWidget;
class PhasetimingeditDlg;


class /*PHASETIMINGTABLEWIDGETSHARED_EXPORT*/ PhasetimingtableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PhasetimingtableWidget(const QString& name, QWidget* parent = 0);
	~PhasetimingtableWidget();
    const QString& widget_name();
    void UpdateTree();

signals:
    void updateTimingCycleSignal();
    void stageTimingEditedSignal();

public slots:
    void OnAddActionClicked();
    void OnEditActionClicked();
    void OnDeleteActionClicked();
    void OnSaveActionClicked();
    void OnTreeItemDoubleClicked(QTreeWidgetItem*, int);
	void OnUpdateTreeSlot(unsigned char phase_timing_id);
    void OnCustomContextMenuRequested(QPoint);

    void OnUpdateDataSlot();
    void OnInitDatabase(void *db_ptr);

    void updateSlot();

private:
    void InitPage();
    void InitSignalSlots();
    void InitTree();
    void InitContextMenu();

	void AddChild(QTreeWidgetItem *item, unsigned char phase_timing_id);

private:
    PhasetimingHandler* handler_;
    QString widget_name_;

private:
    QMenu *context_menu_;
    QAction *add_action_, *edit_action_, *delete_action_, *save_action_;

    PhasetimingeditDlg* phase_timing_edit_dlg_;
    BottomButtonsWidget* buttons_widget_;
    QTreeWidget* tree_widget_;
};

#endif // PHASETIMINGTABLEWIDGET_H
