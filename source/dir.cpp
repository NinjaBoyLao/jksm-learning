#include <3ds.h>
#include <string>
#include <vector>
#include <algorithm>

#include "dir.h"

struct
{
    bool inline operator()(const FS_DirectoryEntry& a, const FS_DirectoryEntry& b)
    {
        for(uint32_t i = 0; i < 0x106; i++)
        {
            int charA = tolower(a.name[i]), charB = tolower(b.name[i]);
            if(charA != charB)
                return charA < charB;
        }

        return false;
    }
} sortDirs;

dirList::dirList(FS_Archive arch, const std::u16string p)
{
    //keep archive data
    a = arch;

    //save path
    path = p;

    //open path given by p
    FSUSER_OpenDirectory(&d, a, fsMakePath(PATH_UTF16, p.data()));

    //loop until we stop reading anymore entries
    u32 read = 0;
    do
    {
        FS_DirectoryEntry getEnt;
        FSDIR_Read(d, &read, 1, &getEnt);
        entry.push_back(getEnt);
    }
    while(read > 0);

    FSDIR_Close(d);

    std::sort(entry.begin(), entry.end() - 1, sortDirs);
}

//clears vector used
dirList::~dirList()
{
    entry.clear();
}

//same thing as contructor. can be used to reinit dirList too
void dirList::reassign(const std::u16string p)
{
    entry.clear();

    FSUSER_OpenDirectory(&d, a, fsMakePath(PATH_UTF16, p.data()));

    u32 read = 0;
    do
    {
        FS_DirectoryEntry getEnt;
        FSDIR_Read(d, &read, 1, &getEnt);
        entry.push_back(getEnt);
    }
    while(read > 0);

    FSDIR_Close(d);

    std::sort(entry.begin(), entry.end() - 1, sortDirs);
}

//Always has an extra, so we subtract 1
unsigned dirList::count()
{
    return entry.size() - 1;
}

//returns true if item is a directory
bool dirList::isDir(int i)
{
    return entry[i].attributes == FS_ATTRIBUTE_DIRECTORY;
}

//returns entry's name as u16string
std::u16string dirList::retItem(int i)
{
    return std::u16string((char16_t *)entry[i].name);
}
