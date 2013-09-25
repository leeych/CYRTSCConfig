#include "unitparamhandler.h"

UnitparamHandler::UnitparamHandler()
{
    db_ = MDatabase::GetInstance();
}

UnitparamHandler::~UnitparamHandler()
{
}

void UnitparamHandler::init_database(void *db_ptr)
{
    db_ = static_cast<MDatabase *>(db_ptr);
}

void UnitparamHandler::reset_database()
{
    db_ = MDatabase::GetInstance();
}

void UnitparamHandler::init()
{
    unit_param_ = db_->get_unit_table();
}

bool UnitparamHandler::get_unitparam(Unit_t &unitparam)
{
    unitparam = unit_param_;
    return true;
}

void UnitparamHandler::set_unitparam(const Unit_t &unitparam)
{
    unit_param_ = unitparam;
}

bool UnitparamHandler::save_data()
{
    db_->set_unit_table(unit_param_);
    return true;
}
