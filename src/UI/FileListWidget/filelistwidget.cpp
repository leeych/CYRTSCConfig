#include <QVBoxLayout>
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>

#include "filelistwidget.h"
#include "macrostring.h"
#include "datainitializer.h"


FileListWidget::FileListWidget(QWidget *parent)
    : QWidget(parent)
{
	reader_writer_ = new FileReaderWriter;
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
        file_list_widget_->addItem(open_file_name_.right(open_file_name_.length() - index - 1));
        emit updateTabPageSignal();
    }
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
        file_list_widget_->addItem(new_file_name_);
        emit updateTabPageSignal();
    }
}

void FileListWidget::OnRemoveFileAction()
{
	int row_cnt = file_list_widget_->count();
	if (row_cnt == 1)
	{
		file_list_widget_->takeItem(0);
		return;
	}
	int row = file_list_widget_->currentRow();
	if (row < 0)
	{
		return;
	}
    file_list_widget_->takeItem(row);
    // TODO: if the .dat file is in display on the tab pages, clear them !
}

void FileListWidget::OnDeleteFileAction()
{
    QListWidgetItem *item = file_list_widget_->currentItem();
    if (item == NULL)
    {
        return;
    }
    QString file_name = item->text();
    if (!file_name.isEmpty() && !QFile::exists(file_name))
    {
        if (QFile::remove(file_name))
        {
            file_list_widget_->removeItemWidget(item);
        }
        else
        {
            QMessageBox::information(NULL, STRING_TIP, STRING_UI_DELETE_FILE, STRING_OK);
            return;
        }
    }
    // TODO: if the .dat file is in display on the tab pages, clear them !
}

void FileListWidget::OnClearListAction()
{
    file_list_widget_->clear();
    // TODO: clear all the tab pages
	DataInitializer data_reset;
	data_reset.ResetDatabase();
}

void FileListWidget::InitPage()
{
    file_list_widget_ = new QListWidget;
    file_list_widget_->setStyleSheet("background-color: #E9F6FE;");

    read_file_button_ = new QToolButton;
    read_file_button_->setPopupMode(QToolButton::MenuButtonPopup);
    QMenu* menu_add = new QMenu;
    add_file_action_ = new QAction(STRING_UI_FILELIST_ADD, menu_add);
    create_file_action_ = new QAction(STRING_UI_FILELIST_CREATE, menu_add);
    menu_add->addAction(add_file_action_);
    menu_add->addAction(create_file_action_);
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

    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addWidget(file_list_widget_);
    vlayout->addLayout(hlayout);
    QFrame* frame = new QFrame;
    frame->setLayout(vlayout);
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(frame);
	layout->setMargin(0);
	layout->setSpacing(0);
    setLayout(layout);
    QString frame_qss =
       "QFrame{border-width:1px;border-style:solid;border-color:LightSkyBlue;margin-top:1px;}";
    setStyleSheet(frame_qss);
}

void FileListWidget::InitSignalSlots()
{
    connect(add_file_action_, SIGNAL(triggered()), this, SLOT(OnAddFileAction()));
    connect(create_file_action_, SIGNAL(triggered()), this, SLOT(OnNewFileAction()));
    connect(remove_file_action_, SIGNAL(triggered()), this, SLOT(OnRemoveFileAction()));
    connect(delete_file_action_, SIGNAL(triggered()), this, SLOT(OnDeleteFileAction()));
    connect(clear_list_action_, SIGNAL(triggered()), this, SLOT(OnClearListAction()));
}

FileListWidget::~FileListWidget()
{
    delete reader_writer_;
}

void FileListWidget::SaveDataFile()
{
	reader_writer_->WriteFile(open_file_name_.toStdString().c_str());
}