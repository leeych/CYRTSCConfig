#include "bytearraydump.h"
#include <QDir>

ByteArrayDump::ByteArrayDump()
{
}

bool ByteArrayDump::dumpByteArray(const QString &file_name, const QByteArray &array)
{
    int index = file_name.lastIndexOf("/");
    QString file_dir = file_name.left(index);
    QDir dir;
    if (!dir.exists(file_dir))
    {
        return false;
    }
    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        return false;
    }
    file.write(array);
    file.close();
    return true;
}
