#ifndef MACROSTRING_H
#define MACROSTRING_H

#include <QObject>

#define STRING_TIP                      QObject::tr("Tip")
#define STRING_YES                      QObject::tr("Yes")
#define STRING_NO                       QObject::tr("No")
#define STRING_CANCEL                   QObject::tr("Cancel")
#define STRING_OK                       QObject::tr("Ok")

#define STRING_UI_ADD                   QObject::tr("Add")
#define STRING_UI_EDIT                  QObject::tr("Edit")
#define STRING_UI_DELETE                QObject::tr("Delete")
#define STRING_UI_SAVE                  QObject::tr("Save")
#define STRING_UI_RESET                 QObject::tr("Reset")
#define STRING_UI_SETTING               QObject::tr("Setting")

#define STRING_UI_LOGIN_USERNAME        QObject::tr("Username")
#define STRING_UI_LOGIN_PASSWD          QObject::tr("Password")
#define STRING_UI_LOGIN_LOGIN           QObject::tr("Log in")

#define STRING_UI_MONDAY                QObject::tr("Monday")
#define STRING_UI_THUESDAY              QObject::tr("Thuesday")
#define STRING_UI_WEDNESDAY             QObject::tr("Wednesday")
#define STRING_UI_THURSDAY              QObject::tr("Thursday")
#define STRING_UI_FRIDAY                QObject::tr("Friday")
#define STRING_UI_SATURDAY              QObject::tr("Saturday")
#define STRING_UI_SUNDAY                QObject::tr("Sunday")

#define STRING_UI_OPEN_FILE             QObject::tr("Open file")
#define STRING_UI_NEW_FILE              QObject::tr("New file")
#define STRING_UI_DELETE_FILE           QObject::tr("Delete file from disk failed.")

#define STRING_SYMBOL_ARROW             QObject::tr("->")

#define STRING_MAIN_TITLE				QObject::tr("ChaoYuan centralize coordinate traffic siganler setting")
#define STRING_MAIN_COMMUNICATION       QObject::tr("Communication")
#define STRING_MAIN_CONFIGURATION       QObject::tr("Configuration")
#define STRING_MAIN_HELP                QObject::tr("Help")
#define STRING_MAIN_STATUS              QObject::tr("Status")
#define STRING_MAIN_READY               QObject::tr("Ready")

#define STRING_UI_SIGNALER_STATUS       QObject::tr("Status")
#define STRING_UI_SIGNALER_NAME         QObject::tr("Signaler name")
#define STRING_UI_SIGNALER_IP           QObject::tr("IP")
#define STRING_UI_SIGNALER_PORT         QObject::tr("Port")
#define STRING_UI_SIGNALER_POSITION     QObject::tr("Position")
#define STRING_UI_SIGNALER_BRANCH       QObject::tr("Branch")
#define STRING_UI_SIGNALER_MODE         QObject::tr("Signaler mode")
#define STRING_UI_SIGNALER_VERSION      QObject::tr("Software version")
#define STRING_UI_SIGNALER_MARKUP       QObject::tr("Mark up")

#define STRING_UI_SIGNALER_ID           QObject::tr("Signaler id")
#define STRING_UI_SIGNALER_PARAM_SET    QObject::tr("Signaler param setting")

#define STRING_UI_FILELIST_ADD          QObject::tr("Add file to list")
#define STRING_UI_FILELIST_CREATE       QObject::tr("Create a new file")
#define STRING_UI_FILELIST_REMOVE       QObject::tr("Remove file")
#define STRING_UI_FILELIST_DELETE       QObject::tr("Delete file")
#define STRING_UI_FILELIST_CLEAR        QObject::tr("Clear list")

#define STRING_UI_SCHEDULE_PLAN         QObject::tr("Schedule Plan Table")
#define STRING_UI_TIME_SECTION          QObject::tr("Time Section Table")
#define STRING_UI_TIMING_PLAN           QObject::tr("Timing Plan Table")
#define STRING_UI_PHASE_TIMING          QObject::tr("Phase Timing Table")
#define STRING_UI_PHASE_TABLE           QObject::tr("Phase Table")
#define STRING_UI_PHASE_CONFLICT        QObject::tr("Phase Conflict Table")
#define STRING_UI_CHANNEL               QObject::tr("Channel Table")
#define STRING_UI_DETECTOR              QObject::tr("Detector Table")
#define STRING_UI_UNIT_TABLE            QObject::tr("Unit Table")
#define STRING_UI_SIGNALER              QObject::tr("Signaler setting")

#define STRING_UI_SCHEDULE_ID           QObject::tr("Schedule ID")
#define STRING_UI_SCHEDULE_MONTH        QObject::tr("Schedule Month(X Month)")
#define STRING_UI_SCHEDULE_WEEKDAY      QObject::tr("Schedule Weekday(Week X)")
#define STRING_UI_SCHEDULE_DAY          QObject::tr("Schedule Day(X Day)")
#define STRING_UI_SCHEDULE_TIME         QObject::tr("Time Section ID")

#define STRING_UI_SCHEDULE_TOP_LIMITED  QObject::tr("Up to the limited schedule count ")

#define STRING_UI_SCHE_DLG_AVAILABLE    QObject::tr("Available schedule id")
#define STRING_UI_SCHE_DLG_MONTH        QObject::tr("Schedule month(12 months in a year)")
#define STRING_UI_SCHE_DLG_WEEKDAY      QObject::tr("Schedule weekday(7 days in a week)")
#define STRING_UI_SCHE_DLG_DAY          QObject::tr("Schedule day(31 days in a month)")
#define STRING_UI_SCHE_DLG_TIMESECTION  QObject::tr("Time section id(0 presents for invalid)")

#define STRING_UI_SCHE_DLG_ALL          QObject::tr("All selected")
#define STRING_UI_SCHE_DLG_NONE         QObject::tr("None selected")

#define STRING_UI_TIMING_ID             QObject::tr("Timing ID")
#define STRING_UI_TIMING_CYCLE          QObject::tr("Cycle(s)")
#define STRING_UI_TIMING_PHASE          QObject::tr("Phase(s)")
#define STRING_UI_TIMING_COOR_PHASE     QObject::tr("Coordinate Phase(Phase ID)")

#define STRING_UI_TIMING_TOP_LIMITED	QObject::tr("Already up to the timing table cout ")

#define STRING_UI_PHASE_TIMING_ID       QObject::tr("Phase Timing ID")
#define STRING_UI_PHASE_TIMING_SET      QObject::tr("Phase settings")
#define STRING_UI_PHASE_TIMING_OPT      QObject::tr("Phase optional param")
#define STRING_UI_PHASE_TIMING_INDUCTION    QObject::tr("Induction section")
#define STRING_UI_PHASE_TIMING_RELEASE  QObject::tr("Release phase id")
#define STRING_UI_PHASE_TIMING_GREEN    QObject::tr("Phase green time")
#define STRING_UI_PHASE_TIMING_YELLOW   QObject::tr("Phase yellow time")
#define STRING_UI_PHASE_TIMING_RED      QObject::tr("Phase red time")
#define STRING_UI_PHASE_TIMING_DELAY    QObject::tr("Start delay time")

#define STRING_UI_PHASE_TIMING_STAGE    QObject::tr("Stage ID")
#define STRING_UI_PHASE_TIMING_TOP_LIMITED	QObject::tr("Already up to the phasetiming top limited num.")

#define STRING_UI_PHASE_ID              QObject::tr("Phase ID")
#define STRING_UI_PHASE_MAN_GREEN       QObject::tr("Man green time")
#define STRING_UI_PHASE_MAN_CLEAR       QObject::tr("Man clear time")
#define STRING_UI_PHASE_MIN_GREEN       QObject::tr("Man min time")
#define STRING_UI_PHASE_UNIT_GREEN      QObject::tr("Unit green delay time")
#define STRING_UI_PHASE_MAX_GREEN1      QObject::tr("Max green 1")
#define STRING_UI_PHASE_MAX_GREEN2      QObject::tr("Max green 2")
#define STRING_UI_PHASE_FIX_GREEN       QObject::tr("Fixed green time")
#define STRING_UI_PHASE_GREEN_FLASH     QObject::tr("Green flash time")
#define STRING_UI_PHASE_TYPE            QObject::tr("Phase type")
#define STRING_UI_PHASE_OPTION_FUNC     QObject::tr("Phase option functions(function id)")
#define STRING_UI_PHASE_EXTENSION       QObject::tr("Extension field")
#define STRING_UI_PHASE_CHANNELS        QObject::tr("Channels(channel id)")

#define STRING_UI_PHASE_OPTIONAL        QObject::tr("Phase optional func")
#define STRING_UI_PHASE_CROSS_STREET    QObject::tr("Man cross street or not")
#define STRING_UI_PHASE_TO_BE_DETERMINED QObject::tr("To be determined")
#define STRING_UI_PHASE_MAN_MOTOR_TOGETHER  QObject::tr("Man motor together")
#define STRING_UI_PHASE_TOGETHER        QObject::tr("Together")
#define STRING_UI_PHASE_DEMOTE          QObject::tr("Demote to timer control")
#define STRING_UI_PHASE_DETECTOR_NUM    QObject::tr("Max detector num")

#define STRING_UI_PHASE_ENABLE          QObject::tr("Enable this phase or not")

#define STRING_UI_PHASE_FIX             QObject::tr("Fixed phase")
#define STRING_UI_PHASE_DETERMINED      QObject::tr("Determined phase")
#define STRING_UI_PHASE_ELASTICITY      QObject::tr("Elasticity phase")
#define STRING_UI_PHASE_CRUTIAL         QObject::tr("Crutial phase")

#define STRING_UI_PHASE_CONFLICT_ID     QObject::tr("ID")
#define STRING_UI_PHASE_CONFLICT_CONID  QObject::tr("conflict ID")
#define STRING_UI_PHASE_CURRENT_ID      QObject::tr("Current phase")

#define STRING_UI_PHASE_TOP_LIMITED		QObject::tr("Already up to the phase table limited count ")

#define STRING_UI_CHANNEL_ID            QObject::tr("Channel ID")
#define STRING_UI_CHANNEL_STATUS        QObject::tr("Channel status(Automatic flash control mode)")
#define STRING_UI_CHANNEL_CONTROL_MODE  QObject::tr("Channel control mode")
#define STRING_UI_CHANNEL_DIRECTION     QObject::tr("Channel direction")
#define STRING_UI_CHANNEL_LANE          QObject::tr("Channel lane")
#define STRING_UI_CHANNEL_LANE_DIRECTION    QObject::tr("Channel lane direction")

#define STRING_UI_CHANNEL_YELLOW_FLASH  QObject::tr("Yellow flash")
#define STRING_UI_CHANNEL_RED_FLASH     QObject::tr("Red flash")
#define STRING_UI_CHANNEL_ALTER         QObject::tr("Alter flash")

#define STRING_UI_CHANNEL_MOTOR         QObject::tr("Motor")
#define STRING_UI_CHANNEL_MAN           QObject::tr("Man")
#define STRING_UI_CHANNEL_F_PHASE       QObject::tr("Phase follow")
#define STRING_UI_CHANNEL_OTHER         QObject::tr("Other")

#define STRING_UI_CHANNEL_EAST          QObject::tr("East")
#define STRING_UI_CHANNEL_WEST          QObject::tr("West")
#define STRING_UI_CHANNEL_NORTH         QObject::tr("North")
#define STRING_UI_CHANNEL_SOUTH         QObject::tr("South")
#define STRING_UI_CHANNEL_NORTH_EAST    QObject::tr("North-east")
#define STRING_UI_CHANNEL_SOUTH_EAST    QObject::tr("South-east")
#define STRING_UI_CHANNEL_NORTH_WEST    QObject::tr("North-west")
#define STRING_UI_CHANNEL_SOUTH_WEST    QObject::tr("South-west")
#define STRING_UI_CHANNEL_LINE          QObject::tr("Line")
#define STRING_UI_CHANNEL_TURN_LEFT     QObject::tr("Turn left")
#define STRING_UI_CHANNEL_TURN_RIGHT    QObject::tr("Turn right")
#define STRING_UI_CHANNEL_SIDEWALK      QObject::tr("Sidewalk")

#define STRING_UI_CHANNEL_TOP_LIMITED	QObject::tr("Already up the channel limited count ")

#define STRING_UI_DETECTOR_ID           QObject::tr("Detector ID")
#define STRING_UI_DETECTOR_PHASE        QObject::tr("Request phase(phase id)")
#define STRING_UI_DETECTOR_MODE         QObject::tr("Detector mode")
#define STRING_UI_DETECTOR_DIRECTION    QObject::tr("Detector direction")
#define STRING_UI_DETECTOR_EFF_TIME     QObject::tr("Request effective time")
#define STRING_UI_DETECTOR_UNEFF_TIME   QObject::tr("Request failure time")
#define STRING_UI_DETECTOR_OPTION       QObject::tr("Optional param(option id)")
#define STRING_UI_DETECTOR_FLOW         QObject::tr("Saturation flow")
#define STRING_UI_DETECTOR_SHARE        QObject::tr("Saturation share")

#define STRING_UI_DETECTOR_REQUEST      QObject::tr("Request detector")
#define STRING_UI_DETECTOR_INDUCTION    QObject::tr("Induction detector")
#define STRING_UI_DETECTOR_TACTICS      QObject::tr("Tactics detector")
#define STRING_UI_DETECTOR_STRATEGY     QObject::tr("Strategy detector")
#define STRING_UI_DETECTOR_MAN          QObject::tr("Pedestrian key")
#define STRING_UI_DETECTOR_BIKE         QObject::tr("Bike detector")
#define STRING_UI_DETECTOR_BUS          QObject::tr("Bus detector")
#define STRING_UI_DETECTOR_MOTOR        QObject::tr("Motor detector")

#define STRING_UI_DETECTOR_KEY_LANE_FLOW    QObject::tr("Saturation flow towards key lane")
#define STRING_UI_DETECTOR_MODEL        QObject::tr("Distinguish model or not")
#define STRING_UI_DETECTOR_KEY_LANE     QObject::tr("Whether buried in the critical lane")

#define STRING_UI_DETECTOR_TOP_LIMITED	QObject::tr("Already up the top limited count ")

#define STRING_UI_TIMESECTION_ID        QObject::tr("Time section ID")
#define STRING_UI_TIMESECTION_EVENT_ID  QObject::tr("Event ID")
#define STRING_UI_TIMESECTION_TIME      QObject::tr("Start time")

#define STRING_UI_TIMESECTION_CTRL_MODE    QObject::tr("Control mode")
#define STRING_UI_TIMESECTION_TIMING_ID    QObject::tr("Timing ID")
#define STRING_UI_TIMESECTION_FUNC_ID      QObject::tr("Accessibility(id)")
#define STRING_UI_TIMESECTION_SPECIAL_ID   QObject::tr("Special function(id)")

#define STRING_UI_TIMESECTION_BRIGHT       QObject::tr("Brightness control")
#define STRING_UI_TIMESECTION_SPECIAL_FUNC QObject::tr("Special function")
#define STRING_UI_TIMESECTION_FUNCTION     QObject::tr("Function")
#define STRING_UI_TIMESECTION_BUS          QObject::tr("Bus first")
#define STRING_UI_TIMESECTION_ACCESSIBILITY QObject::tr("Accessibility")
#define STRING_UI_TIMESECTION_EVENT        QObject::tr("Time section events")

#define STRING_UI_TIMESECTION_TOP_LIMITED  QObject::tr("Up to the limited count ")

#define STRING_UI_PHASETIMING_ID           QObject::tr("Phase timing id")
#define STRING_UI_PHASETIMING_RELEASE_ID   QObject::tr("Release phase id(phase id)")
#define STRING_UI_PHASETIMING_GREEN_TIME   QObject::tr("Phase green time(s)")
#define STRING_UI_PHASETIMING_YELLOW_TIME  QObject::tr("Phase yellow time(s)")
#define STRING_UI_PHASETIMING_RED_TIME     QObject::tr("Phase red time(s)")
#define STRING_UI_PHASETIMING_DELAY_TIME   QObject::tr("Start delay time(s)")
#define STRING_UI_PHASETIMING_OPTION       QObject::tr("Phase option param(id)")

#define STRING_UI_UNIT_SIGNAL_IP           QObject::tr("Signaler IP")
#define STRING_UI_UNIT_SERVER_IP           QObject::tr("Server IP")
#define STRING_UI_UNIT_SERVER_PORT         QObject::tr("Server port")
#define STRING_UI_UNIT_FLASH_TIME          QObject::tr("Start flashing time(s)")
#define STRING_UI_UNIT_ALL_RED             QObject::tr("Start all-red time(s)")
#define STRING_UI_UNIT_SIGNALER_STATUS     QObject::tr("Current signaler ctrl state")

#define STRING_UI_UNIT_MAN_SIGNALER        QObject::tr("Pedestrian-demand signaler")
#define STRING_UI_UNIT_MAN_SCHEDULE        QObject::tr("Man->multi-period->yellow flash->shutdown")
#define STRING_UI_UNIT_MULTI_SIGNALER      QObject::tr("Multi-period timer signaler")
#define STRING_UI_UNIT_MULTI_SCHEDULE      QObject::tr("Coordinate->multi-period->yellow flash->shutdown")
#define STRING_UI_UNIT_INDUCTION_SIGNALER  QObject::tr("Induction controlling signaler")
#define STRING_UI_UNIT_INDUCTION_SCHEDULE  QObject::tr("Induction->multi-period->yellow flash->shutdown")
#define STRING_UI_UNIT_OTHER_SIGNALER      QObject::tr("Other")
#define STRING_UI_UNIT_CENTRALIZE_SIGNALER QObject::tr("Centralize coordinate signaler")

#define STRING_UI_UNIT_SETTING_GROUP       QObject::tr("Signaler settings")
#define STRING_UI_UNIT_FAULT_GROUP         QObject::tr("Signaler fault demotion order")

#define STRING_UI_UNIT_INVALID              QObject::tr("Invalid")
#define STRING_UI_UNIT_AUTO_CONTORL         QObject::tr("Autonomous control")
#define STRING_UI_UNIT_CLOSE_LIGHTS         QObject::tr("Close lights")
#define STRING_UI_UNIT_FLASH                QObject::tr("Flash")
#define STRING_UI_UNIT_ALLRED               QObject::tr("All red")
#define STRING_UI_UNIT_COORDINATE           QObject::tr("Coordinate")
#define STRING_UI_UNIT_INDUCTION            QObject::tr("Induction")
#define STRING_UI_UNIT_MAIN_INDUCTION       QObject::tr("Main road half induction")
#define STRING_UI_UNIT_SECOND_INDUCTION     QObject::tr("Second road half induction")
#define STRING_UI_UNIT_SINGLE_OPTIONAL      QObject::tr("Single-point optimization")
#define STRING_UI_UNIT_CROSS_STREET         QObject::tr("Pedestrian crossing")
#define STRING_UI_UNIT_COORDINATE_INDUCTION QObject::tr("Coordinate induction")
#define STRING_UI_UNIT_MASTER_SLAVE         QObject::tr("Master-slave remote control")
#define STRING_UI_UNIT_SYSTEM_OPTIONAL      QObject::tr("System Optimization")
#define STRING_UI_UNIT_INTERVENTION         QObject::tr("Intervention wire")

#define STRING_UI_FILEMANAGER_NEW_FILE_FAILED      QObject::tr("Create new file failed.")
#define STRING_UI_FILEMANAGER_ADD_FILE_FAILED      QObject::tr("Add file failed.")
#define STRING_UI_FILEMANAGER_REMOVE_FILE_FAILED   QObject::tr("Remove file failed.")
#define STRING_UI_FILEMANAGER_DELETE_FILE_FAILED   QObject::tr("Delete file failed.")
#define STRING_UI_FILEMANAGER_CLEAR_FILE_FAILED    QObject::tr("Clear file list failed.")


#endif // MACROSTRING_H
