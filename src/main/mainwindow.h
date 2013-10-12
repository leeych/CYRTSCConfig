#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>
#include <QToolBar>
#include <QSplitter>
#include <QFrame>
#include <QMap>
#include <QLabel>

class MTabWidget;
class ConfigoptWidget;
class FileListWidget;
class ScheduleTableWidget;
class TimingplanWidget;
class PhasetableWidget;
class TimesectiontableWidget;
class PhasetimingtableWidget;
class PhaseconflicttableWidget;
class ChanneltableWidget;
class DetectortableWidget;
class UnitparamtableWidget;
class TscAboutDlg;
class SignalerStatusWidget;


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
    void OnConfigEditedSlot();

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
    bool is_edited_;

private:
    QLabel* status_tip_label_;
    QToolBar* main_toolbar_;
    QToolButton* save_button_;
    QToolButton* communication_button_;
    QToolButton* config_button_;
    QToolButton* help_button_;

    QSplitter* main_splitter_;
    QSplitter* left_splitter_;

    ConfigoptWidget* config_widget_;
    FileListWidget* file_list_widget_;
    SignalerStatusWidget* signaler_tab_page_;
    MTabWidget* main_tab_window_;

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
    TscAboutDlg *about_dlg_;
};

#endif // MAINWINDOW_H
