#ifndef TIMINGPLANWIDGET_H
#define TIMINGPLANWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QMenu>
//#include "timingplanwidget_global.h"

class BottomButtonsWidget;
class TimingeditDlg;
class TimingHandler;

class /*TIMINGPLANWIDGETSHARED_EXPORT*/ TimingplanWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TimingplanWidget(const QString& name, QWidget *parent = 0);
    ~TimingplanWidget();
    const QString& widget_name();
    void UpdateTable();

signals:
    void timingEditedSignal();

public slots:
    void OnAddActionClicked();
    void OnEditActionClicked();
    void OnDeleteActionClicked();
    void OnSaveActionClicked();

    void OnCustomContextMenuRequested(QPoint);
    void OnTableCellDoubleClicked(int row, int col);
    void OnUpdateTableRowSlot(unsigned char id);
    void OnUpdateDataSlot();
    void OnUpdateTimingCycleSlot();

    void OnInitDatabase(void *db_ptr);

    void updateSlot();

protected:
    void resizeEvent(QResizeEvent *);

private:
    void InitPage();
    void InitSignalSlots();

    void InitTable();
    void InitTableHeader();
    void InitContextMenu();

    void UpdateCycletime();

private:
    TimingHandler* handler_;
    QString widget_name_;

private:
    TimingeditDlg* timing_edit_dlg_;
    BottomButtonsWidget* buttons_widget_;
    QTableWidget* timing_table_;

    QMenu* context_menu_;
    QAction* add_action_;
    QAction* edit_action_;
    QAction* delete_action_;
    QAction* save_action_;
};

#endif // TIMINGPLANWIDGET_H
