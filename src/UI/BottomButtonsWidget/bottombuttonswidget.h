#ifndef BOTTOMBUTTONSWIDGET_H
#define BOTTOMBUTTONSWIDGET_H

#include "bottombuttonswidget_global.h"

#include <QWidget>
#include <QPushButton>

class BOTTOMBUTTONSWIDGETSHARED_EXPORT BottomButtonsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BottomButtonsWidget(QWidget *parent = 0);

    QPushButton *addButtonPtr();
    QPushButton *editButtonPtr();
    QPushButton *deleteButtonPtr();
    QPushButton *saveButtonPtr();

signals:

public slots:
    void OnAddButtonClicked();
    void OnEditButtonClicked();
    void OnDeleteButtonClicked();
    void OnSaveButtonClicked();

private:
    void InitPage();

private:
    QPushButton *add_button_, *edit_button_, *delete_button_, *save_button_;
};

#endif // BOTTOMBUTTONSWIDGET_H
