#ifndef MTABLEWIDGET_H
#define MTABLEWIDGET_H

#include <QTableWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QPushButton>

class MTableWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit MTableWidget(QWidget *parent = 0);
    
    void SetRowHighlight(int id);

    virtual QString& widget_name();
    virtual void CreateTable(int w);
    virtual void UpdateList();

    void setAddButtonName(int itype);

protected:
    void resizeEvent(QResizeEvent * e);

signals:
    void sendSelectedId(int);

public slots:
    void onCustomContextMenuRequested(const QPoint &pos);

public:

private:
    void CreateMenus();
    void CreateButtons();
    void InitTable();

protected:
    int row_number;
    QString widget_name_;
    int table_width_;

    QTableWidget* main_table_;
    QMenu* popmenu_;
    QAction* add_action_;
    QAction* edit_action_;
    QAction* delete_action_;

    QPushButton* add_button_;
    QPushButton* edit_button_;
    QPushButton* delete_button_;
};

#endif // MTABLEWIDGET_H
