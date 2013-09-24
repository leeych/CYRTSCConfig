#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>
#include <QToolBar>
#include <QSplitter>
#include <QFrame>
#include <QMap>
#include <QLabel>

#include "mtabwidget.h"
#include "configoptwidget.h"
#include "filelistwidget.h"

#include "scheduletablewidget.h"
#include "timingplanwidget.h"
#include "phasetablewidget.h"
#include "timesectiontablewidget.h"
#include "phasetimingtablewidget.h"
#include "phaseconflicttablewidget.h"
#include "channeltablewidget.h"
#include "detectortablewidget.h"
#include "unitparamtablewidget.h"
#include "signalerstatuswidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void OnConfigoptSlot(const QString& page_name);
    void OnUpdateStatusBarSlot(const QString &file_name);

private slots:
    void OnUpdateTabPageSlot();

    void OnCommunicationToolButtonClicked();
    void OnConfigurationToolButtonClicked();
    void OnHelpToolButtonClicked();
    void OnSaveToolButtonClicked();

private:
    void InitPage();
    void InitSignalSlots();
    void CreateToolBar();
    void CreateFileMenus();
    void CreateTabPages();
    void CreateStatusBar();

    void ChangeTabPage(const QString& page_name);
    void resizeEvent(QResizeEvent *);
    void contextMenuEvent(QContextMenuEvent *event);

private:
    QMap<QString, QWidget*> tab_page_map_;
    QString curr_page_name_;

private:
    QLabel* tip_;
    QToolBar* main_toolbar_;
    QToolButton* save_button_;
    QToolButton* communication_button_;
    QToolButton* config_button_;
    QToolButton* help_button_;

    QSplitter* main_splitter_;
    QSplitter* left_splitter_;

    ConfigoptWidget* config_widget_;
    FileListWidget* file_list_widget_;

    MTabWidget* main_tab_window_;
//    QTabWidget *main_tab_window_;
    // Tab pages
    ScheduleTableWidget* schedule_tab_page_;
    TimesectiontableWidget* time_section_tab_page_;
    TimingplanWidget* timing_plan_tab_page_;
    PhasetimingtableWidget* phase_timing_tab_page_;
    PhasetableWidget* phase_table_tab_page_;
    PhaseconflicttableWidget* phase_conflict_tab_page_;
    ChanneltableWidget* channel_tab_page_;
    DetectortableWidget* detector_tab_page_;
    UnitparamtableWidget* unitparam_tab_page_;

    SignalerStatusWidget* signaler_tab_page_;
};

#endif // MAINWINDOW_H
