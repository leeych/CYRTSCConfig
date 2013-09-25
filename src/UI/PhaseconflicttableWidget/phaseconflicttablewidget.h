#ifndef PHASECONFLICTTABLEWIDGET_H
#define PHASECONFLICTTABLEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QMenu>
#include "phaseconflicttablewidget_global.h"
#include "phaseconflicteditdlg.h"
#include "phaseconflicthandler.h"
#include "bottombuttonswidget.h"

class PHASECONFLICTTABLEWIDGETSHARED_EXPORT PhaseconflicttableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PhaseconflicttableWidget(const QString& name, QWidget* parent = 0);
	~PhaseconflicttableWidget();
    const QString &widget_name();
    void UpdateTable();

signals:

public slots:
    void OnTableCellDoubleClicked(int row, int col);
    void OnUpdateTableRowSlot(unsigned char phase_id);
    void OnEditActionClicked();
    void OnSaveButtonClicked();
    void OnCustomContextMenuRequested(QPoint);

    void OnUpdateDataSlot();
    void OnInitDatabase(void *db_ptr);

private:
    void InitPage();
    void InitSignalSlots();

    void InitTable();
    void InitTableHeader();
    void InitContextMenu();

    QString get_conflict_desc(unsigned char phase_id);

private:
    QString widget_name_;
    PhaseconflictHandler* handler_;

private:
    QMenu* context_menu_;
    QAction* edit_action_;

    PhaseconflicteditDlg* phase_edit_dlg_;
    BottomButtonsWidget* buttons_widget_;

    QTableWidget* conflict_table_;
};

#endif // PHASECONFLICTTABLEWIDGET_H
