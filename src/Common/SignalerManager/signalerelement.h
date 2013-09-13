#ifndef SIGNALERELEMENT_H
#define SIGNALERELEMENT_H

#include <QString>

class SignalerElement
{
public:
    static QString Name;
    static QString IP;
    static QString Port;
    static QString Location;
    static QString Branch;
    static QString Mode;
    static QString Version;
    static QString Remark;
    static QString Status;

    static QString SignalerList;
    static QString Signaler;

private:
    SignalerElement();
    ~SignalerElement();
};

#endif // SIGNALERELEMENT_H
