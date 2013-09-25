#ifndef CHANNELTABLEWIDGET_H
#define CHANNELTABLEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QMenu>
#include <QAction>
#include "channeleditdlg.h"
#include "channelhandler.h"
#include "bottombuttonswidget.h"
//#include "channeltablewidget_global.h"

class /*CHANNELTABLEWIDGETSHARED_EXPORT*/ ChanneltableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChanneltableWidget(const QString& name, QWidget* parent = 0);
	~ChanneltableWidget();
    const QString& widget_name();
    void UpdateTable();

signals:

public slots:
    void OnTableCellDoubleClicked(int row, int col);
    void OnAddActionClicked();
    void OnEditActionClicked();
    void OnDeleteActionClicked();
    void OnSaveActionClicked();
    void OnUpdateTableRowSlot(unsigned char channel_id);
    void OnCustomContextMenuRequested(QPoint);

    void OnUpdateDataSlot();
    void OnUpdateChannelCtrlsrcSlot();
    void OnInitDatabase(void *db_ptr);

private:
    void InitPage();
    void InitSignalSlots();

    void InitTable();
    void InitTableHeader();
    void InitContextMenu();

	QString get_channel_direction_desc(unsigned char direction);
	QString get_channel_lane_desc(unsigned char lane);

private:
    QString widget_name_;
    ChannelHandler* handler_;

private:
    QMenu* context_menu_;
    QAction* add_action_, *edit_action_, *delete_action_, *save_action_;

    ChanneleditDlg* channel_edit_dlg_;
    BottomButtonsWidget* buttons_widget_;
    QTableWidget* channel_table_;
};

#endif // CHANNELTABLEWIDGET_H
