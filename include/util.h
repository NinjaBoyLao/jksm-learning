#ifndef UTIL_H
#define UTIL_H

#include <string>

#include "titledata.h"
#include "dir.h"

std::u32string tou32(const std::u16string t);
std::u16string tou16(const char *t);
std::string toString(const std::u16string t);

std::u32string modeText(int mode);

void writeErrorToBuff(u8 *buff, size_t bSize, unsigned error);

void createTitleDir(const titleData t, int mode);
bool deleteSV(const titleData t);
std::u16string getPath(int mode);

void deleteExtdata(const titleData dat);
void createExtData(const titleData dat);

void evenString(std::string *test);

//This detects if running under something as 3dsx
bool runningUnder();

//returns if touchscreen is still being pressed anywhere
bool touchPressed(touchPosition p);

//sleeps for x seconds
void sleep(int s);

//returns if dealing with extdata
bool modeExtdata(int mode);

bool fexists(const char *path);

void fsStart();
void fsEnd();

void prepareMenus();
#endif // UTIL_H
