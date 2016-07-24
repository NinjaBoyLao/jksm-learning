#ifndef TITLES_H
#define TITLES_H

#include <vector>
#include "titledata.h"

void sdTitlesInit();
void nandTitlesInit();

extern std::vector<titleData> sdTitle;
extern std::vector<titleData> nandTitle;

extern bool refreshed;

#endif // TITLES_H
