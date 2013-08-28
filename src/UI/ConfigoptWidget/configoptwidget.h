#ifndef CONFIGOPTWIDGET_H
#define CONFIGOPTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFrame>

class ConfigoptWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigoptWidget(QWidget *parent = 0);
    
signals:
    void scheduleplanSignal(const QString& page_name);
    void timesectionSignal(const QString& page_name);
    void timingplanSignal(const QString& page_name);
    void phasetimingSignal(const QString& page_name);
    void phasetableSignal(const QString& page_name);
    void phaseconflictSignal(const QString& page_name);
    void channelSignal(const QString& page_name);
    void detectorSignal(const QString& page_name);
    void unitparamSignal(const QString& page_name);

public slots:
    void OnScheduleplanButtonClicked();
    void OnTimesectionButtonClicked();
    void OnTimingplanButtonClicked();
    void OnPhasetimingButtonClicked();
    void OnPhasetableButtonClicked();
    void OnPhaseconflictButtonClicked();
    void OnChannelButtonClicked();
    void OnDetectorButtonClicked();
    void OnUnitparamButtonClicked();

private:
    void InitPage();
    void InitSignalSlots();

private:
    QFrame *frame_;
    QPushButton* schedule_plan_button_;
    QPushButton* time_table_button_;
    QPushButton* timing_plan_button_;
    QPushButton* phase_timing_button_;
    QPushButton* phase_table_button_;
    QPushButton* phase_conflict_button_;
    QPushButton* channel_button_;
    QPushButton* detector_button_;
    QPushButton* unit_param_button_;
};

#endif // CONFIGOPTWIDGET_H
