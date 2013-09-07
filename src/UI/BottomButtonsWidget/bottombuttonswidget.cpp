#include "bottombuttonswidget.h"
#include "macrostring.h"
#include <QHBoxLayout>
#include <QMessageBox>


BottomButtonsWidget::BottomButtonsWidget(QWidget *parent)
    : QWidget(parent)
{
    InitPage();

    add_button_->setBackgroundRole(QPalette::Button);
    edit_button_->setBackgroundRole(QPalette::Button);
    delete_button_->setBackgroundRole(QPalette::Button);
    save_button_->setBackgroundRole(QPalette::Button);
}

QPushButton *BottomButtonsWidget::addButtonPtr()
{
    return add_button_;
}

QPushButton *BottomButtonsWidget::editButtonPtr()
{
    return edit_button_;
}

QPushButton *BottomButtonsWidget::deleteButtonPtr()
{
    return delete_button_;
}

QPushButton *BottomButtonsWidget::saveButtonPtr()
{
    return save_button_;
}

void BottomButtonsWidget::OnAddButtonClicked()
{
    QMessageBox::information(this, STRING_TIP, "Add", STRING_OK);
    return;
}

void BottomButtonsWidget::OnEditButtonClicked()
{
    QMessageBox::information(this, STRING_TIP, "Edit", STRING_OK);
    return;
}

void BottomButtonsWidget::OnDeleteButtonClicked()
{
    QMessageBox::information(this, STRING_TIP, "Delete", STRING_OK);
    return;
}

void BottomButtonsWidget::OnSaveButtonClicked()
{
    QMessageBox::information(this, STRING_TIP, "Save", STRING_OK);
    return;
}

void BottomButtonsWidget::InitPage()
{
    QHBoxLayout* hlayout = new QHBoxLayout;

    add_button_ = new QPushButton(STRING_UI_ADD);
    edit_button_ = new QPushButton(STRING_UI_EDIT);
    delete_button_ = new QPushButton(STRING_UI_DELETE);
    save_button_ = new QPushButton(STRING_UI_SAVE);

	add_button_->setMinimumWidth(80);
	edit_button_->setMinimumWidth(80);
	delete_button_->setMinimumWidth(80);
	save_button_->setMinimumWidth(80);

	hlayout->setStretch(0, 1);
	hlayout->setStretch(1, 2);
	hlayout->setStretch(2, 1);
	hlayout->setStretch(3, 2);
	hlayout->setStretch(4, 1);
	hlayout->setStretch(5, 2);
	hlayout->setStretch(6, 1);
	hlayout->setStretch(7, 2);
	hlayout->setStretch(8, 1);

    hlayout->addStretch(1);
    hlayout->addWidget(add_button_);
    hlayout->addStretch(1);
    hlayout->addWidget(edit_button_);
    hlayout->addStretch(1);
    hlayout->addWidget(delete_button_);
    hlayout->addStretch(1);
    hlayout->addWidget(save_button_);
    hlayout->addStretch(1);
    setLayout(hlayout);
}
