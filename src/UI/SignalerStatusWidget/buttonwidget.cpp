#include "buttonwidget.h"
#include "macrostring.h"

#include <QHBoxLayout>

ButtonWidget::ButtonWidget(QWidget *parent) :
    QWidget(parent)
{
    InitPage();
}

ButtonWidget::~ButtonWidget()
{
}

QPushButton *ButtonWidget::addButtonPtr()
{
    return add_button_;
}

QPushButton *ButtonWidget::editButtonPtr()
{
    return edit_button_;
}

QPushButton *ButtonWidget::delButtonPtr()
{
    return delete_button_;
}

QPushButton *ButtonWidget::saveButtonPtr()
{
    return save_button_;
}

QPushButton *ButtonWidget::advancedButtonPtr()
{
    return advanced_button_;
}

void ButtonWidget::InitPage()
{
    add_button_ = new QPushButton(STRING_UI_ADD);
    edit_button_ = new QPushButton(STRING_UI_EDIT);
    delete_button_ = new QPushButton(STRING_UI_DELETE);
    save_button_ = new QPushButton(STRING_UI_SAVE);
    advanced_button_ = new QPushButton(STRING_UI_ADVANCED);

    add_button_->setMinimumWidth(80);
    edit_button_->setMinimumWidth(80);
    delete_button_->setMinimumWidth(80);
    save_button_->setMinimumWidth(80);
    advanced_button_->setMinimumWidth(80);

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addStretch(1);
    hlayout->addWidget(add_button_);
    hlayout->addStretch(1);
    hlayout->addWidget(edit_button_);
    hlayout->addStretch(1);
    hlayout->addWidget(delete_button_);
    hlayout->addStretch(1);
    hlayout->addWidget(save_button_);
    hlayout->addStretch(1);
    hlayout->addWidget(advanced_button_);
    hlayout->addStretch(1);

    setLayout(hlayout);
}
