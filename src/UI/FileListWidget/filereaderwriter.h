#ifndef FILEREADERWRITER_H
#define FILEREADERWRITER_H

#include "tscparam.h"
#include "mdatabase.h"
#include <QString>
#include <stdio.h>
#include <fstream>
#include <string>


class FileReaderWriter
{
    
public:
    FileReaderWriter();
    ~FileReaderWriter();

    void InitDatabase(MDatabase *db_ptr);
    MDatabase *defaultDatabase();

    bool ReadFile(MDatabase *db, const QString &file_path);
    bool ReadFile(const QString &file_path, TSCParam &param);
    bool WriteFile(const QString &file_path);
    bool WriteFile(const TSCParam &param, const QString &file_path);
    void SetTSCParam(const TSCParam &param);

private:
	void ResetParam();
    void setTscParam(const QString &filename, TSCParam &param);

private:
    MDatabase *db_;
    TSCParam tsc_param_;

    QString open_file_path_;
    QString new_file_path_;
    QString write_file_path_;
};

#endif // FILEREADERWRITER_H
