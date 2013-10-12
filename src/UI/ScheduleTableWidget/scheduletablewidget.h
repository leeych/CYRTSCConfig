#ifndef SCHEDULETABLEWIDGET_H
#define SCHEDULETABLEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QMenu>
//#include "scheduletablewidget_global.h"

class BottomButtonsWidget;
class ScheduleDlg;
class ScheduleHandler;


class /*SCHEDULETABLEWIDGETSHARED_EXPORT*/ ScheduleTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ScheduleTableWidget(const QString& name, QWidget* parent = 0);
    ~ScheduleTableWidget();
    const QString& widget_name();
    void UpdateTable();

signals:
    void schedEditedSignal();

public slots:
    void OnAddActionClicked();
    void OnEditActionClicked();
    void OnDeleteActionClicked();
    void OnSaveActionClicked();

    void OnTableCellDoubleClicked(int row, int col);
    void OnUpdateTableSignal(unsigned char sched_id);
    void OnCustomContextMenuRequested(QPoint);

    void OnUpdateDataSlot();
    void OnInitDatabase(void *db_ptr);

protected:
    void resizeEvent(QResizeEvent *);

private:
    void InitPage();
    void InitSignalSlots();

    void InitTable();
    void InitTableHeader();
    void InitContextMenu();

    QString GetMonthDisp(unsigned short month);
    QString GetWeekDisp(unsigned char week);
    QString GetDayDisp(unsigned int day);

private:
    QString name_;
    ScheduleHandler* handler_;

private:
    ScheduleDlg* schedule_dlg_;
    QTableWidget* schedule_table_;
    BottomButtonsWidget* buttons_widget_;

    QPushButton* add_button_;
    QPushButton* edit_button_;
    QPushButton* delete_button_;
    QPushButton* save_button_;

    QMenu* context_menu_;
    QAction* add_action_;
    QAction* edit_action_;
    QAction* delete_action_;
    QAction* save_action_;
};

#endif // SCHEDULETABLEWIDGET_H
