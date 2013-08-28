#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QToolButton>
#include <QAction>
#include "filelistwidget_global.h"
#include "filereaderwriter.h"

class FILELISTWIDGETSHARED_EXPORT FileListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileListWidget(QWidget* parent = 0);
	~FileListWidget();

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

    void UpdateUI();

private:
	FileReaderWriter *reader_writer_;

private:
    QListWidget* file_list_widget_;
    QToolButton* read_file_button_;
    QToolButton* delete_file_button_;

    QAction* add_file_action_;
    QAction* create_file_action_;

    QAction* remove_file_action_;
    QAction* delete_file_action_;
    QAction* clear_list_action_;
};

#endif // FILELISTWIDGET_H
