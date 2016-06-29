#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include "hbfilter.h"
#include "util.h"

//These are the homebrew IDs from the gbatemp thread - 3Dsurfer.
//3DSurfer and Picross E3 use the same ID

std::vector<u32> filterID;

void loadFilterList()
{
    if(fexists("/homebrew/3ds/JKSV/filter.txt"))
    {
        FILE *load = fopen("/homebrew/3ds/JKSV/filter.txt", "r");

        char id[16];

        while(fgets(id, 16, load))
        {
            u32 newID = strtoul(id, NULL, 16);
            filterID.push_back(newID);
        }

        fclose(load);
    }
}

bool hbFilter(u64 id)
{
    u32 low = (u32)id;
    //it's easier to filter retroarch this way
    char tmp[16];
    AM_GetTitleProductCode(MEDIATYPE_SD, id, tmp);
    std::string test;
    test.assign(tmp, 0, 5);
    if(test.compare("RARCH")==0)
        return true;

    for(unsigned i = 0; i < filterID.size(); i++)
    {
        if(low==filterID[i])
            return true;
    }
    return false;
}
