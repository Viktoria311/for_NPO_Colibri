#ifndef FUNCS_H
#define FUNCS_H

#include "encoder.h"
#include <string>

void customize(settings & s);

void modified_file_name(std::string & file_name_, int i);

bool is_open_by_user(const std::string & file_);

void search_and_encode_files(const settings & s);

void set_all(settings & s);

#endif
