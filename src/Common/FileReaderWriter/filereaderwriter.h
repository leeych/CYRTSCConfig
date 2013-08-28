#ifndef FILEREADERWRITER_H
#define FILEREADERWRITER_H


#include "tscparam.h"
#include "mdatabase.h"
#include "filereaderwriter_global.h"
#include <stdio.h>
#include <fstream>



class FILEREADERWRITERSHARED_EXPORT FileReaderWriter
{
    
public:
    FileReaderWriter();
    ~FileReaderWriter();

    bool ReadFile(const char* file_path);
    bool WriteFile(const char* file_path);

private:
    MDatabase *db_;
    TSCParam tsc_param_;
};

#endif // FILEREADERWRITER_H
