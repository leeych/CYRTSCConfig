#ifndef BUTTONWIDGET_H
#define BUTTONWIDGET_H

#include <QWidget>
#include <QPushButton>

class ButtonWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ButtonWidget(QWidget *parent = 0);
    ~ButtonWidget();

    QPushButton *addButtonPtr();
    QPushButton *editButtonPtr();
    QPushButton *delButtonPtr();
    QPushButton *saveButtonPtr();
    QPushButton *advancedButtonPtr();
    
signals:
    
public slots:

private:
    void InitPage();
    
private:
    QPushButton *add_button_, *edit_button_, *delete_button_;
    QPushButton *save_button_, *advanced_button_;
};

#endif // BUTTONWIDGET_H
