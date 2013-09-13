#include <QtGui>
#include "mutility.h"
#include "mainwindow.h"
#include "macrostring.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    CreateTabPages();
    InitPage();
    InitSignalSlots();
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnConfigoptSlot(const QString &page_name)
{
    ChangeTabPage(page_name);
}

void MainWindow::OnUpdateStatusBarSlot(const QString &file_name)
{
    tip_->setText(file_name);
}

void MainWindow::OnUpdateTabPageSlot()
{
    unitparam_tab_page_->OnUpdateDataSlot();
    unitparam_tab_page_->UpdateUI();

    schedule_tab_page_->OnUpdateDataSlot();
    schedule_tab_page_->UpdateTable();

    time_section_tab_page_->OnUpdateDataSlot();
    time_section_tab_page_->UpdateTree();

    timing_plan_tab_page_->OnUpdateDataSlot();
    timing_plan_tab_page_->UpdateTable();

    phase_timing_tab_page_->OnUpdateDataSlot();
    phase_timing_tab_page_->UpdateTree();

    phase_table_tab_page_->OnUpdateDataSlot();
    phase_table_tab_page_->UpdateTree();

    phase_conflict_tab_page_->OnUpdateDataSlot();
    phase_conflict_tab_page_->UpdateTable();

    channel_tab_page_->OnUpdateDataSlot();
    channel_tab_page_->UpdateTable();

    detector_tab_page_->OnUpdateDataSlot();
    detector_tab_page_->UpdateTable();
}

void MainWindow::OnCommunicationToolButtonClicked()
{
    left_splitter_->setVisible(false);
    ChangeTabPage(STRING_UI_SIGNALER);
    signaler_tab_page_->Initialize();
}

void MainWindow::OnConfigurationToolButtonClicked()
{
    left_splitter_->setVisible(true);
    ChangeTabPage(STRING_UI_SCHEDULE_PLAN);
}

void MainWindow::OnHelpToolButtonClicked()
{
    QMessageBox::information(this, STRING_TIP, "Help", STRING_OK);
    return;
}

void MainWindow::OnSaveToolButtonClicked()
{
	// make sure that all of the changes have already been saved to the disk
	unitparam_tab_page_->OnOkButtonClicked();
	schedule_tab_page_->OnSaveActionClicked();
	time_section_tab_page_->OnSaveActionClicked();
	phase_timing_tab_page_->OnSaveActionClicked();
	timing_plan_tab_page_->OnSaveActionClicked();
	phase_conflict_tab_page_->OnSaveButtonClicked();
	phase_table_tab_page_->OnSaveActionClicked();
	channel_tab_page_->OnSaveActionClicked();
	detector_tab_page_->OnSaveActionClicked();

	bool status = file_list_widget_->SaveDataFile();
	if (!status)
	{
        QMessageBox::information(this, STRING_TIP, STRING_MAIN_SAVE_FAILED, STRING_OK);
		return;
	}
	else
	{
        QMessageBox::information(this, STRING_TIP, STRING_MAIN_SAVE_SUCCESS, STRING_OK);
		return;
	}
}

void MainWindow::InitPage()
{
	setWindowTitle(STRING_MAIN_TITLE);
    CreateToolBar();
    CreateStatusBar();

    config_widget_ = new ConfigoptWidget;
    file_list_widget_ = new FileListWidget;
    main_tab_window_ = new MTabWidget;
	QFont font(STRING_FONT_SONGTI, 11);
	main_tab_window_->setFont(font);

    left_splitter_ = new QSplitter(Qt::Vertical);
    left_splitter_->addWidget(config_widget_);
    left_splitter_->addWidget(file_list_widget_);

	unitparam_tab_page_->PageFontSetting(font);
    main_tab_window_->addTab(unitparam_tab_page_, unitparam_tab_page_->widget_name());
    main_tab_window_->setMinimumWidth(size().width() * 4/5);

    main_splitter_ = new QSplitter(Qt::Horizontal);
    main_splitter_->addWidget(left_splitter_);
    main_splitter_->addWidget(main_tab_window_);

    QList<int> listleft;
    int szleft = size().height();
    listleft.append(szleft * 2/3);
    listleft.append(szleft * 1/3);
    left_splitter_->setSizes(listleft);

    setCentralWidget(main_splitter_);

    QString dir;
    MUtility::getStyleSheetDir(dir);
    setStyleSheet(QLatin1String(MUtility::fetchQrc(dir + "mainwindow.qss")));
}

void MainWindow::InitSignalSlots()
{
    connect(communication_button_, SIGNAL(clicked()), this, SLOT(OnCommunicationToolButtonClicked()));
    connect(config_button_, SIGNAL(clicked()), this, SLOT(OnConfigurationToolButtonClicked()));
    connect(help_button_, SIGNAL(clicked()), this, SLOT(OnHelpToolButtonClicked()));
    connect(save_button_, SIGNAL(clicked()), this, SLOT(OnSaveToolButtonClicked()));

    connect(config_widget_, SIGNAL(scheduleplanSignal(QString)), this, SLOT(OnConfigoptSlot(QString)));
    connect(config_widget_, SIGNAL(timesectionSignal(QString)), this, SLOT(OnConfigoptSlot(QString)));
    connect(config_widget_, SIGNAL(timingplanSignal(QString)), this, SLOT(OnConfigoptSlot(QString)));
    connect(config_widget_, SIGNAL(phasetableSignal(QString)), this, SLOT(OnConfigoptSlot(QString)));
    connect(config_widget_, SIGNAL(phasetimingSignal(QString)), this, SLOT(OnConfigoptSlot(QString)));
    connect(config_widget_, SIGNAL(phaseconflictSignal(QString)), this, SLOT(OnConfigoptSlot(QString)));
    connect(config_widget_, SIGNAL(channelSignal(QString)), this, SLOT(OnConfigoptSlot(QString)));
    connect(config_widget_, SIGNAL(detectorSignal(QString)), this, SLOT(OnConfigoptSlot(QString)));
    connect(config_widget_, SIGNAL(unitparamSignal(QString)), this, SLOT(OnConfigoptSlot(QString)));

    connect(file_list_widget_, SIGNAL(updateTabPageSignal()), this, SLOT(OnUpdateTabPageSlot()));
    connect(file_list_widget_, SIGNAL(updateFilePathSignal(QString)), this, SLOT(OnUpdateStatusBarSlot(QString)));
    connect(phase_timing_tab_page_, SIGNAL(updateTimingCycleSignal()), timing_plan_tab_page_, SLOT(OnUpdateTimingCycleSlot()));
    connect(phase_table_tab_page_, SIGNAL(updateChannelCtrlsrcSignal()), channel_tab_page_, SLOT(OnUpdateChannelCtrlsrcSlot()));
}

void MainWindow::CreateToolBar()
{
    main_toolbar_ = new QToolBar(this);
    main_toolbar_->setIconSize(QSize(48, 48));

    QString dir;
    MUtility::getImageDir(dir);
    communication_button_ = new QToolButton;
    communication_button_->setStatusTip(STRING_MAIN_COMMUNICATION);
    communication_button_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    communication_button_->setText(STRING_MAIN_COMMUNICATION);
    communication_button_->setIcon(QIcon(dir + "/icon/network.png"));

    config_button_ = new QToolButton;
    config_button_->setStatusTip(STRING_MAIN_CONFIGURATION);
    config_button_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    config_button_->setText(STRING_MAIN_CONFIGURATION);
    config_button_->setIcon(QIcon(dir + "/icon/setting.png"));

    help_button_ = new QToolButton;
    help_button_->setStatusTip(STRING_MAIN_HELP);
    help_button_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    help_button_->setText(STRING_MAIN_HELP);
    help_button_->setIcon(QIcon(dir + "/icon/help.png"));

    save_button_ = new QToolButton;
    save_button_->setStatusTip(STRING_UI_SAVE);
    save_button_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    save_button_->setText(STRING_UI_SAVE);
    save_button_->setIcon(QIcon(dir + "/icon/save.png"));

    main_toolbar_->addWidget(communication_button_);
    main_toolbar_->addWidget(config_button_);
    main_toolbar_->addWidget(save_button_);
    main_toolbar_->addWidget(help_button_);

    addToolBar(Qt::TopToolBarArea, main_toolbar_);
    main_toolbar_->setMovable(false);
}

void MainWindow::CreateTabPages()
{
    schedule_tab_page_ = new ScheduleTableWidget(STRING_UI_SCHEDULE_PLAN);
    time_section_tab_page_ = new TimesectiontableWidget(STRING_UI_TIME_SECTION);
    timing_plan_tab_page_ = new TimingplanWidget(STRING_UI_TIMING_PLAN);
    phase_timing_tab_page_ = new PhasetimingtableWidget(STRING_UI_PHASE_TIMING);
    phase_table_tab_page_ = new PhasetableWidget(STRING_UI_PHASE_TABLE);
    phase_conflict_tab_page_ = new PhaseconflicttableWidget(STRING_UI_PHASE_CONFLICT);
    channel_tab_page_ = new ChanneltableWidget(STRING_UI_CHANNEL);
    detector_tab_page_ = new DetectortableWidget(STRING_UI_DETECTOR);
    unitparam_tab_page_ = new UnitparamtableWidget(STRING_UI_UNIT_TABLE);
    signaler_tab_page_ = new SignalerStatusWidget(STRING_UI_SIGNALER);

    tab_page_map_.insert(schedule_tab_page_->widget_name(), schedule_tab_page_);
    tab_page_map_.insert(time_section_tab_page_->widget_name(), time_section_tab_page_);
    tab_page_map_.insert(timing_plan_tab_page_->widget_name(), timing_plan_tab_page_);
    tab_page_map_.insert(phase_timing_tab_page_->widget_name(), phase_timing_tab_page_);
    tab_page_map_.insert(phase_table_tab_page_->widget_name(), phase_table_tab_page_);
    tab_page_map_.insert(phase_conflict_tab_page_->widget_name(), phase_conflict_tab_page_);
    tab_page_map_.insert(channel_tab_page_->widget_name(), channel_tab_page_);
    tab_page_map_.insert(detector_tab_page_->widget_name(), detector_tab_page_);
    tab_page_map_.insert(unitparam_tab_page_->widget_name(), unitparam_tab_page_);
    tab_page_map_.insert(signaler_tab_page_->widget_name(), signaler_tab_page_);

    QMap<QString, QWidget*>::Iterator iter = tab_page_map_.begin();
    while (iter != tab_page_map_.end())
    {
        qDebug() << iter.key() << endl;
        ++iter;
    }
}

void MainWindow::CreateStatusBar()
{
    statusBar()->showMessage(STRING_MAIN_STATUS);
    statusBar()->setStyleSheet("background-color: rgb(233,246,254);font-size:12px");

    tip_ = new QLabel(statusBar());
    tip_->setMinimumSize(tip_->sizeHint());
    tip_->setAlignment(Qt::AlignHCenter);
    statusBar()->addPermanentWidget(tip_);
    statusBar()->setSizeGripEnabled(true);
}

void MainWindow::ChangeTabPage(const QString &page_name)
{
    if (page_name == main_tab_window_->currentTabName())
    {
        return;
    }
    main_tab_window_->removeTab(0);
    main_tab_window_->addTab(tab_page_map_[page_name], page_name);
    main_tab_window_->changeTab(page_name);
    //tip_->setText(page_name);
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    int width = size().width();
    int height = size().height();
    QList<int> left_size;
    left_size.append(height * 5/9);
    left_size.append(height * 4/9);
    left_splitter_->setSizes(left_size);

    QList<int> main_size;
    main_size.append(width * 1/5);
    main_size.append(width * 4/5);
    main_splitter_->setSizes(main_size);

    //tip_->setText(main_tab_window_->currentTabName());
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}
