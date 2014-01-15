#ifndef LOGTRACERINFO_H
#define LOGTRACERINFO_H

#include <QList>
#include <QString>

class LogTracerInfo
{
public:
    LogTracerInfo();

    bool get_debug_enable() const { return debug_enable_; }
    bool get_debug_file_enable() const { return debug_file_enable_; }
    bool get_debug_terminal_enable() const { return debug_terminal_enable_; }
    bool get_debug_timestamp_enable() const { return debug_timestamp_enable_; }
    bool get_debug_module_name_enable() const { return debug_module_name_enable_; }
    bool get_ui_display() const { return ui_display_; }

    void set_debug_enable(bool enable) { debug_enable_ = enable; }
    void set_debug_file_enable(bool enable) { debug_file_enable_ = enable; }
    void set_debug_terminal_enable(bool enable) { debug_terminal_enable_ = enable; }
    void set_debug_timestamp_enable(bool enable) { debug_timestamp_enable_ = enable; }
    void set_debug_module_name_enable(bool enable) { debug_module_name_enable_ = enable; }
    void set_ui_display(bool enable) { ui_display_ = enable; }

    void addLevel(const int level) { level_list_.append(level); }
    void addModule(const QString &name) { module_name_list_.append(name); }

    QList<QString> get_module_name_list() const { return module_name_list_; }
    QList<int> get_level_list() const { return level_list_; }

private:
    bool debug_enable_;
    bool debug_file_enable_;
    bool debug_terminal_enable_;
    bool debug_timestamp_enable_;
    bool debug_module_name_enable_;
    bool ui_display_;

    QList<QString> module_name_list_;
    QList<int> level_list_;
};

#endif // LOGTRACERINFO_H
