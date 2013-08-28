#ifndef UNITPARAMHANDLER_H
#define UNITPARAMHANDLER_H

#include "mdatabase.h"

class UnitparamHandler
{
public:
    UnitparamHandler();
    ~UnitparamHandler();

    void init();

    bool get_unitparam(Unit_t &unitparam);
    void set_unitparam(const Unit_t &unitparam);

    bool save_data();

private:
    MDatabase *db_;
    Unit_t unit_param_;
};

#endif // UNITPARAMHANDLER_H
