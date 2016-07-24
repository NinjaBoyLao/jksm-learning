#ifndef GSTR_H
#define GSTR_H

#include <string>

//Uses my shitty keyboard to get a string.
std::string GetString(const char *hint);
int getInt(const char *hint, int maxValue);

#endif // GSTR_H
