#ifndef FILEREADERWRITER_H
#define FILEREADERWRITER_H

#include "tscparam.h"
#include "mdatabase.h"
#include <stdio.h>
#include <fstream>
#include <string>


class FileReaderWriter
{
    
public:
    FileReaderWriter();
    ~FileReaderWriter();

    bool ReadFile(const char* file_path);
    bool WriteFile(const char* file_path);

private:
    MDatabase *db_;
    TSCParam tsc_param_;

    std::string open_file_path_;
    std::string new_file_path_;
    std::string write_file_path_;
};

#endif // FILEREADERWRITER_H
