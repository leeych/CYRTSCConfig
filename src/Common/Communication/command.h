#ifndef COMMAND_H
#define COMMAND_H

#include <string>

class Command
{
public:
    static std::string TSCtime;
    static std::string Network;
    static std::string DetectorInfo;
    static std::string Version; // 1
    static std::string BeginMonitor;
    static std::string EndMonitor;

private:
    Command();
};

#endif // COMMAND_H
