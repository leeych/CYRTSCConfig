#ifndef TIMESECTIONTABLEWIDGET_H
#define TIMESECTIONTABLEWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QMenu>
#include <QAction>
#include "timesectiontablewidget_global.h"
#include "timesectioneditdlg.h"
#include "timesectionhandler.h"
#include "bottombuttonswidget.h"

class TIMESECTIONTABLEWIDGETSHARED_EXPORT TimesectiontableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TimesectiontableWidget(const QString& name, QWidget* parent = 0);
    ~TimesectiontableWidget();
    const QString& widget_name();
    void UpdateTree();

signals:

public slots:
    void OnTreeItemDoubleClicked(QTreeWidgetItem*, int);
    void OnAddActionClicked();
    void OnEditActionClicked();
    void OnDeleteActionClicked();
    void OnSaveActionClicked();
    void OnUpdateTreeSlot(unsigned char time_section_id);
    void OnCustomContextMenuRequested(QPoint);

    void OnUpdateDataSlot();
    void OnInitDatabase(void *db_ptr);

private:
    void InitPage();
    void InitSignalSlots();
    void InitTree();

    void InitContextMenu();

    void AddChild(QTreeWidgetItem *item, unsigned char time_section_id);

    QString get_ctrl_mode_desc(unsigned char ctrl_mode);

private:
    TimesectionHandler* handler_;
    QString widget_name_;

private:
    QMenu* context_menu_;
    QAction* add_action_, *edit_action_, *delete_action_, *save_action_;

    TimesectioneditDlg* timesection_edit_dlg_;
    BottomButtonsWidget* buttons_widget_;
    QTreeWidget* section_tree_widget_;
};

#endif // TIMESECTIONTABLEWIDGET_H
