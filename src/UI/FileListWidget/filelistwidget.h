#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QTableWidget>
#include <QToolButton>
#include <QAction>
#include <QMap>

#include "filelistwidget_global.h"
#include "filereaderwriter.h"
#include "datainitializer.h"

class FILELISTWIDGETSHARED_EXPORT FileListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileListWidget(QWidget* parent = 0);
	~FileListWidget();
	void SaveDataFile();

signals:
    void updateTabPageSignal();

public slots:
    void OnAddFileAction();
    void OnNewFileAction();

    void OnRemoveFileAction();
    void OnDeleteFileAction();
    void OnClearListAction();

private:
    void InitPage();
    void InitSignalSlots();

    void InitFileTable();
    void UpdateUI();

	bool ResetDataDisp(const QString &file_name);
    void AddTableItem(int row, int col, const QString &str);

private:
    unsigned int file_count_;
	FileReaderWriter *reader_writer_;
    DataInitializer *data_resetter_;

    QMap<unsigned int, QString> file_path_map_;  // (id, full file path)
    QString curr_file_name_;
	QString open_file_name_;
	QString new_file_name_;

private:
    QTableWidget *file_table_;
    QListWidget* file_list_widget_;
    QToolButton* read_file_button_;
    QToolButton* delete_file_button_;

    QAction* add_file_action_, *create_file_action_;
    QAction* remove_file_action_, *delete_file_action_, *clear_list_action_;
};

#endif // FILELISTWIDGET_H
