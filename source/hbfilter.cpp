#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include "hbfilter.h"
#include "util.h"
#include "ui.h"

std::vector<u32> filterID;

bool downloadFilter()
{
    u32 wifi;
    ACU_GetWifiStatus(&wifi);
    if(wifi==0)
        return false;

    httpcContext filter;

    Result res = httpcOpenContext(&filter, HTTPC_METHOD_GET, "https://raw.githubusercontent.com/J-D-K/JKSM/master/filter.txt", 1);
    if(res)
    {
        showMessage("Open context.");
        return false;
    }

    res = httpcSetSSLOpt(&filter, SSLCOPT_DisableVerify);
    if(res)
    {
        showMessage("Set SSL Opt.");
    }

    res = httpcBeginRequest(&filter);
    if(res)
    {
        showMessage("Begin request.");
        return false;
    }

    u32 code;
    res = httpcGetResponseStatusCode(&filter, &code, 0);
    if(res || code!=200)
    {
        showMessage("Not found?");
        return false;
    }

    u32 fSize;
    res = httpcGetDownloadSizeState(&filter, NULL, &fSize);
    if(res)
    {
        showMessage("Download size.");
        return false;
    }

    u8 *buff = new u8[fSize];
    res = httpcDownloadData(&filter, buff, fSize, NULL);
    if(res)
    {
        showMessage("Download data.");
        delete[] buff;
        return false;
    }

    FILE *filterTxt = fopen("filter.txt", "wb");
    fwrite(buff, 1, fSize, filterTxt);
    fclose(filterTxt);

    delete[] buff;

    showMessage("Filter downloaded.");

    return true;
}

void loadFilterList()
{
    filterID.clear();
    if(fexists("filter.txt"))
    {
        FILE *load = fopen("filter.txt", "r");

        char id[16];

        while(fgets(id, 16, load))
        {
            u32 newID = strtoul(id, NULL, 16);
            filterID.push_back(newID);
        }

        fclose(load);
    }
    else
    {
        if(downloadFilter())
            loadFilterList();
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
