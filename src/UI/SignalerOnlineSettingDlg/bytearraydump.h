#ifndef BYTEARRAYDUMP_H
#define BYTEARRAYDUMP_H

#include <QFile>
#include <QByteArray>

class ByteArrayDump
{
public:
    ByteArrayDump();
    bool dumpByteArray(const QString &file_name, const QByteArray &array);

private:
    QFile file_;
    QString file_name_;
};

#endif // BYTEARRAYDUMP_H
