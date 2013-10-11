#include <QVBoxLayout>
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QHeaderView>

#include "filelistwidget.h"
#include "macrostring.h"
#include "datainitializer.h"


FileListWidget::FileListWidget(QWidget *parent)
    : QWidget(parent)
{
	reader_writer_ = new FileReaderWriter;
    data_resetter_ = new DataInitializer;
	file_count_ = 0;
    InitPage();
    InitSignalSlots();
}

void FileListWidget::OnAddFileAction()
{
    open_file_name_ = QFileDialog::getOpenFileName(0, STRING_UI_OPEN_FILE, "./", "Data(*.dat);;All File(*.*)");
    if (open_file_name_.isNull() || open_file_name_.isEmpty())
    {
        return;
    }
    bool state = reader_writer_->ReadFile(open_file_name_.toStdString().data());
    if (state)
    {
		int index = open_file_name_.lastIndexOf("/");
        QString file_short_name = open_file_name_.right(open_file_name_.length() - index - 1);
//        file_list_widget_->addItem(file_short_name);

        AddTableItem(file_count_, 0, QString::number(file_count_));
        AddTableItem(file_count_, 1, file_short_name);
        file_path_map_.insert(file_count_, open_file_name_);
		file_count_++;

        curr_file_name_ = open_file_name_;
        emit updateTabPageSignal();
        emit updateFilePathSignal(curr_file_name_);
    }
    UpdateUI();
}

void FileListWidget::OnNewFileAction()
{
    new_file_name_ = QFileDialog::getSaveFileName(0, STRING_UI_NEW_FILE, "./", "Data(*.dat);;All File(*.*)");
	if (new_file_name_.isNull() || new_file_name_.isEmpty())
	{
		return;
	}
    bool state = reader_writer_->WriteFile(new_file_name_.toStdString().data());
    if (state)
    {
        curr_file_name_ = new_file_name_;
//        file_list_widget_->addItem(new_file_name_);
        int index = new_file_name_.lastIndexOf("/");
        QString file_short_name = new_file_name_.right(new_file_name_.length() - index - 1);
        AddTableItem(file_count_, 0, QString::number(file_count_));
        AddTableItem(file_count_, 1, file_short_name);
		file_path_map_.insert(file_count_, new_file_name_);
		file_count_++;
		UpdateUI();
        emit updateTabPageSignal();
		emit updateFilePathSignal("");
    }
    else
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_FILEMANAGER_NEW_FILE_FAILED, STRING_OK);
        return;
    }
}

void FileListWidget::OnSaveAsFileAction()
{
    QString file_name = QFileDialog::getSaveFileName(NULL, STRING_UI_SAVEAS, "./", "Data(*.dat);;All File(*.*)");
    if (file_name.isNull() || file_name.isEmpty())
    {
        return;
    }
    bool status = reader_writer_->WriteFile(file_name.toStdString().c_str());
    UpdateUI();
    if (!status)
    {
        QMessageBox::information(this, STRING_TIP, STRING_MAIN_SAVE_FAILED, STRING_OK);
        return;
    }
}

void FileListWidget::OnRemoveFileAction()
{
    int row = file_table_->currentRow();
	if (row < 0)
	{
		return;
	}
	int id = file_table_->item(row, 0)->text().toUInt();
    QString file_name = file_path_map_.value(id);
    if (ResetDataDisp(file_name))
    {
        curr_file_name_ = "";
//        delete file_list_widget_->takeItem(row);
        file_table_->removeRow(row);
		file_count_--;
		file_path_map_.remove(file_count_);
		
        emit updateTabPageSignal();
		emit updateFilePathSignal("");
    }
    UpdateUI();
}

void FileListWidget::OnDeleteFileAction()
{
	int row_cnt = file_table_->rowCount();
	if (row_cnt == 1)
	{
		int id = file_table_->item(0, 0)->text().toUInt();
		QString file_name = file_path_map_.value(id);
		if (DeleteFile(file_name))
		{
			file_table_->removeRow(0);
		}
		return;
	}

    int row = file_table_->currentRow();
    if (row < 0)
    {
        return;
    }
    int id = file_table_->item(row, 0)->text().toInt();
    QString file_name = file_path_map_.value(id);
	if (DeleteFile(file_name))
	{
		file_table_->removeRow(row);
	}
}

void FileListWidget::OnClearListAction()
{
    curr_file_name_ = "";
    data_resetter_->ResetDatabase();

	int table_row = file_table_->rowCount();
	for (int i = 0; i < table_row; i++)
	{
		file_table_->removeRow(i);
	}
    file_table_->clear();
	file_path_map_.clear();
	file_count_ = 0;
    emit updateTabPageSignal();
	emit updateFilePathSignal("");
    UpdateUI();
}

void FileListWidget::OnFileTableItemDoubleClicked(int row, int col)
{
    Q_UNUSED(col);
    if (row < 0)
    {
        return;
    }
    int id = file_table_->item(row, 0)->text().toInt();
    QString file_name = file_path_map_.value(id);
    if (!(file_name != curr_file_name_ && !file_name.isEmpty() && QFile::exists(file_name)))
    {
        return;
    }

    bool status = reader_writer_->ReadFile(file_name.toStdString().c_str());
    if (status)
    {
        curr_file_name_ = file_name;
        emit updateTabPageSignal();
        emit updateFilePathSignal(curr_file_name_);
    }
}

void FileListWidget::InitPage()
{
    InitFileTable();
//    file_list_widget_ = new QListWidget;
//    file_list_widget_->setStyleSheet("background-color: #E9F6FE;");

    read_file_button_ = new QToolButton;
    read_file_button_->setPopupMode(QToolButton::MenuButtonPopup);
    QMenu* menu_add = new QMenu;
    add_file_action_ = new QAction(STRING_UI_FILELIST_ADD, menu_add);
    create_file_action_ = new QAction(STRING_UI_FILELIST_CREATE, menu_add);
    save_as_action_ = new QAction(STRING_UI_SAVEAS, menu_add);
    menu_add->addAction(add_file_action_);
    menu_add->addAction(create_file_action_);
    menu_add->addAction(save_as_action_);
//    menu_add->setDefaultAction(add_file_action_);
    read_file_button_->setMenu(menu_add);
    read_file_button_->setDefaultAction(add_file_action_);

    delete_file_button_ = new QToolButton;
    QMenu* menu_remove = new QMenu;
    remove_file_action_ = new QAction(STRING_UI_FILELIST_REMOVE, menu_remove);
    delete_file_action_ = new QAction(STRING_UI_FILELIST_DELETE, menu_remove);
    clear_list_action_ = new QAction(STRING_UI_FILELIST_CLEAR, menu_remove);
    menu_remove->addAction(remove_file_action_);
    menu_remove->addAction(delete_file_action_);
    menu_remove->addSeparator();
    menu_remove->addAction(clear_list_action_);
//    menu_remove->setDefaultAction(remove_file_action_);
    delete_file_button_->setPopupMode(QToolButton::MenuButtonPopup);
    delete_file_button_->setMenu(menu_remove);
    delete_file_button_->setDefaultAction(remove_file_action_);

    QHBoxLayout* hlayout = new QHBoxLayout;
    hlayout->addWidget(read_file_button_);
    hlayout->addWidget(delete_file_button_);
    hlayout->setSpacing(0);

    QVBoxLayout* vlayout = new QVBoxLayout;
//    vlayout->addWidget(file_list_widget_);
    vlayout->addWidget(file_table_);
    vlayout->addLayout(hlayout);
    QFrame* frame = new QFrame;
    frame->setLayout(vlayout);
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(frame);
	layout->setMargin(0);
	layout->setSpacing(0);
    setLayout(layout);
    QString frame_qss =
       "QFrame{border-width:1px;border-style:solid;border-color:LightSkyBlue;margin-top:1px;}"
            "QToolButton {border: 1px solid #DCDCDC; border-radius: 3px;"
            "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde);"
            "min-width: 80px; min-height: 20px;}"
            "QToolButton:pressed { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa);}"
            "QToolButton:flat { border: none;}"
            "QToolButton:default { border-color: navy;}";
    setStyleSheet(frame_qss);
	UpdateUI();
}

void FileListWidget::InitSignalSlots()
{
    connect(add_file_action_, SIGNAL(triggered()), this, SLOT(OnAddFileAction()));
    connect(create_file_action_, SIGNAL(triggered()), this, SLOT(OnNewFileAction()));
    connect(save_as_action_, SIGNAL(triggered()), this, SLOT(OnSaveAsFileAction()));

    connect(remove_file_action_, SIGNAL(triggered()), this, SLOT(OnRemoveFileAction()));
    connect(delete_file_action_, SIGNAL(triggered()), this, SLOT(OnDeleteFileAction()));
    connect(clear_list_action_, SIGNAL(triggered()), this, SLOT(OnClearListAction()));

    connect(file_table_, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(OnFileTableItemDoubleClicked(int,int)));
}

void FileListWidget::InitFileTable()
{
    file_table_ = new QTableWidget;
    file_table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    file_table_->setSelectionBehavior(QTableWidget::SelectRows);
    file_table_->setSelectionMode(QAbstractItemView::SingleSelection);
    file_table_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    file_table_->verticalHeader()->setHidden(true);
	file_table_->horizontalHeader()->setHidden(true);
	file_table_->horizontalHeader()->setStretchLastSection(true);

    QPalette pal;
    pal.setColor(QPalette::Base, QColor(233, 246, 254));
    file_table_->setPalette(pal);
    file_table_->setColumnCount(2);
    file_table_->setColumnHidden(0, true);
	file_table_->setColumnWidth(0, 1);
    file_table_->resizeRowsToContents();
}

void FileListWidget::UpdateUI()
{
    bool status = !file_path_map_.isEmpty();

    save_as_action_->setEnabled(status);
    remove_file_action_->setEnabled(status);
    delete_file_action_->setEnabled(status);
    clear_list_action_->setEnabled(status);
}

FileListWidget::~FileListWidget()
{
    delete reader_writer_;
    delete data_resetter_;
}

bool FileListWidget::SaveDataFile()
{
	if (curr_file_name_.isEmpty())
	{
		QString file_name = QFileDialog::getSaveFileName(NULL, STRING_UI_SAVE, "./", "Data(*.dat);;All File(*.*)");
		if (file_name.isNull() || file_name.isEmpty())
		{
			return false;
		}
		int index = file_name.lastIndexOf("/");
		QString file_short_name = file_name.right(file_name.length() - index - 1);
		//        file_list_widget_->addItem(file_short_name);

		AddTableItem(file_count_, 0, QString::number(file_count_));
		AddTableItem(file_count_, 1, file_short_name);
		file_path_map_.insert(file_count_, open_file_name_);
		file_count_++;

		curr_file_name_ = file_name;
		open_file_name_ = file_name;
		emit updateTabPageSignal();
		emit updateFilePathSignal(curr_file_name_);
		UpdateUI();
		return reader_writer_->WriteFile(file_name.toStdString().c_str());
	}
    return reader_writer_->WriteFile(curr_file_name_.toStdString().c_str());
}

bool FileListWidget::ResetDataDisp(const QString &file_name)
{
    if (curr_file_name_.contains(file_name))
    {
        return data_resetter_->ResetDatabase();
    }
    return false;
}

void FileListWidget::AddTableItem(int row, int col, const QString &str)
{
    file_table_->setRowCount(row + 1);
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(str);
    file_table_->setItem(row, col, item);
}

bool FileListWidget::DeleteFile( const QString &file_name )
{
	if (!file_name.isEmpty() && QFile::exists(file_name))
	{
		if (QFile::remove(file_name))
		{
			if (ResetDataDisp(file_name))
			{
				UpdateUI();
				curr_file_name_ = "";
				//                delete file_list_widget_->takeItem(row);
				//file_table_->removeRow(row);
				file_count_--;
				file_path_map_.remove(file_count_);
				
				emit updateTabPageSignal();
				emit updateFilePathSignal("");
				UpdateUI();
				return true;
			}
		}
		else
		{
            QMessageBox::information(this, STRING_TIP, STRING_UI_DELETE_FILE, STRING_OK);
			return false;
		}
	}
	return false;
}
