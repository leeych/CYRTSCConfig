#include "filereaderwriter.h"
#include <memory.h>


FileReaderWriter::FileReaderWriter()
{
    db_ = MDatabase::GetInstance();
}

FileReaderWriter::~FileReaderWriter()
{
}

bool FileReaderWriter::ReadFile(const char* file_path)
{
	open_file_path_ = std::string(file_path);
	FILE *fp = fopen(file_path, "rb+");
	if (fp == NULL)
	{
		return false;
	}
	fseek(fp, 0, SEEK_SET);
	fread(&tsc_param_.tsc_header_, sizeof(tsc_param_.tsc_header_), 1, fp);
	fread(&tsc_param_.unit_param_, sizeof(tsc_param_.unit_param_), 1, fp);
	fread(&tsc_param_.sched_table_, sizeof(tsc_param_.sched_table_), 1, fp);
	fread(&tsc_param_.time_section_table_, sizeof(tsc_param_.time_section_table_), 1, fp);
	fread(&tsc_param_.timing_plan_table_, sizeof(tsc_param_.timing_plan_table_), 1, fp);
	fread(&tsc_param_.stage_timing_table_, sizeof(tsc_param_.stage_timing_table_), 1, fp);
	fread(&tsc_param_.phase_table_, sizeof(tsc_param_.phase_table_), 1, fp);
	fread(&tsc_param_.phase_conflict_table_, sizeof(tsc_param_.phase_conflict_table_), 1, fp);
	fread(&tsc_param_.channel_table_, sizeof(tsc_param_.channel_table_), 1, fp);
	fread(&tsc_param_.channel_hint_table_, sizeof(tsc_param_.channel_hint_table_), 1, fp);
	fread(&tsc_param_.detector_table_, sizeof(tsc_param_.detector_table_), 1, fp);
	db_->set_tsc_header(tsc_param_.tsc_header_);
	db_->set_unit_table(tsc_param_.unit_param_);
	db_->set_schedule_table(tsc_param_.sched_table_);
	db_->set_timesection_table(tsc_param_.time_section_table_);
	db_->set_timing_table(tsc_param_.timing_plan_table_);
	db_->set_phasetiming_table(tsc_param_.stage_timing_table_);
	db_->set_phase_table(tsc_param_.phase_table_);
	db_->set_phase_conflict_table(tsc_param_.phase_conflict_table_);
	db_->set_channel_table(tsc_param_.channel_table_);
	db_->set_channel_hint_table(tsc_param_.channel_hint_table_);
	db_->set_detector_table(tsc_param_.detector_table_);

	fclose(fp);
	return true;
}

bool FileReaderWriter::WriteFile(const char* file_path)
{
    FILE *fp = fopen(file_path, "wb+");
    fseek(fp, 0, SEEK_SET);
    memcpy(&tsc_param_.tsc_header_, &db_->get_tsc_header(), sizeof(tsc_param_.tsc_header_));
    memcpy(&tsc_param_.unit_param_, &db_->get_unit_table(), sizeof(tsc_param_.unit_param_));
    memcpy(&tsc_param_.sched_table_, &db_->get_schedule(), sizeof(tsc_param_.sched_table_));
    memcpy(&tsc_param_.time_section_table_, &db_->get_timesection(), sizeof(tsc_param_.time_section_table_));
    memcpy(&tsc_param_.timing_plan_table_, &db_->get_timingplan(), sizeof(tsc_param_.timing_plan_table_));
    memcpy(&tsc_param_.stage_timing_table_, &db_->get_timeconfig(), sizeof(tsc_param_.stage_timing_table_));
    memcpy(&tsc_param_.phase_table_, &db_->get_phase(), sizeof(tsc_param_.phase_table_));
    memcpy(&tsc_param_.phase_conflict_table_, &db_->get_phase_err(), sizeof(tsc_param_.phase_conflict_table_));
    memcpy(&tsc_param_.channel_table_, &db_->get_channel(), sizeof(tsc_param_.channel_table_));
    memcpy(&tsc_param_.channel_hint_table_, &db_->get_channel_hint(), sizeof(tsc_param_.channel_hint_table_));
    memcpy(&tsc_param_.detector_table_, &db_->get_detector(), sizeof(tsc_param_.detector_table_));

    fwrite(&tsc_param_.tsc_header_, sizeof(tsc_param_.tsc_header_), 1, fp);
    fwrite(&tsc_param_.unit_param_, sizeof(tsc_param_.unit_param_), 1, fp);
    fwrite(&tsc_param_.sched_table_, sizeof(tsc_param_.sched_table_), 1, fp);
    fwrite(&tsc_param_.time_section_table_, sizeof(tsc_param_.time_section_table_), 1, fp);
    fwrite(&tsc_param_.timing_plan_table_, sizeof(tsc_param_.timing_plan_table_), 1, fp);
    fwrite(&tsc_param_.stage_timing_table_, sizeof(tsc_param_.stage_timing_table_), 1, fp);
    fwrite(&tsc_param_.phase_table_, sizeof(tsc_param_.phase_table_), 1, fp);
    fwrite(&tsc_param_.phase_conflict_table_, sizeof(tsc_param_.phase_conflict_table_), 1, fp);
    fwrite(&tsc_param_.channel_table_, sizeof(tsc_param_.channel_table_), 1, fp);
    fwrite(&tsc_param_.channel_hint_table_, sizeof(tsc_param_.channel_hint_table_), 1, fp);
    fwrite(&tsc_param_.detector_table_, sizeof(tsc_param_.detector_table_), 1, fp);

    fclose(fp);
    return true;
}
