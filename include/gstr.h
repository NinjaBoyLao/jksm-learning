#ifndef GSTR_H
#define GSTR_H

#include <string>

std::string GetString(const char *hint);
int getInt(const char *hint, unsigned init, int maxValue);

#endif // GSTR_H
