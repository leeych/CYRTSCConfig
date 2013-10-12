#ifndef DETECTORTABLEWIDGET_H
#define DETECTORTABLEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QMenu>
#include <QAction>
//#include "detectortablewidget_global.h"

class DetectoreditDlg;
class DetectorHandler;
class BottomButtonsWidget;

class /*DETECTORTABLEWIDGETSHARED_EXPORT*/ DetectortableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DetectortableWidget(const QString& name, QWidget* parent = 0);
	~DetectortableWidget();
    const QString& widget_name();
    void UpdateTable();

signals:
    void detectorEditedSignal();

public slots:
    void OnTableCellDoubleClicked(int row, int col);
    void OnAddActionClicked();
    void OnEditActionClicked();
    void OnDeleteActionClicked();
    void OnSaveActionClicked();
    void OnUpdateTableRowSlot(unsigned char detector_id);
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

    void AddTableRow();

	QString get_detector_phase_ids_desc(unsigned int phase_ids);
    QString get_detector_type_desc(unsigned char type);
    QString get_detector_direction(unsigned char direction);

private:
    DetectorHandler* handler_;
    QString widget_name_;

private:
    QMenu* context_menu_;
    QAction* add_action_, *edit_action_, *delete_action_, *save_action_;

    DetectoreditDlg* detector_edit_dlg_;
    BottomButtonsWidget* buttons_widget_;
    QTableWidget* detector_table_;
};

#endif // DETECTORTABLEWIDGET_H
