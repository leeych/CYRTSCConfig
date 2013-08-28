#include <QVBoxLayout>
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>

#include "filelistwidget.h"
#include "macrostring.h"


FileListWidget::FileListWidget(QWidget *parent)
    : QWidget(parent)
{
	reader_writer_ = new FileReaderWriter;
    InitPage();
    InitSignalSlots();
}

void FileListWidget::OnAddFileAction()
{
    QString file_name = QFileDialog::getOpenFileName(0, STRING_UI_OPEN_FILE, "./", "Data(*.dat);;All File(*.*)");
    if (file_name.isNull() || file_name.isEmpty())
    {
        return;
    }
    bool state = reader_writer_->ReadFile(file_name.toStdString().data());
    if (state)
    {
        file_list_widget_->addItem(file_name);
        emit updateTabPageSignal();
    }
}

void FileListWidget::OnNewFileAction()
{
    QString file_name = QFileDialog::getSaveFileName(0, STRING_UI_NEW_FILE, "./", "Data(*.dat);;All File(*.*)");
	if (file_name.isNull() || file_name.isEmpty())
	{
		return;
	}
    bool state = reader_writer_->WriteFile(file_name.toStdString().data());
    if (state)
    {
        file_list_widget_->addItem(file_name);
    }
}

void FileListWidget::OnRemoveFileAction()
{
    QListWidgetItem *item = file_list_widget_->currentItem();
    if (item == NULL)
    {
        return;
    }
    file_list_widget_->removeItemWidget(item);
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
