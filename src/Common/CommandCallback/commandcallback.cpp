#include "commandcallback.h"


CommandCallback::CommandCallback()
{
}

CommandCallback::~CommandCallback()
{
}

CommandCallback *CommandCallback::GetInstance()
{
    if (instance_ == NULL)
    {
        instance_ = new CommandCallback;
    }
    return instance_;
}

void CommandCallback::ReadTscTime()
{
}

void CommandCallback::OnCmdParam()
{
}

void CommandCallback::OnReadTscTimeSlot()
{
}
