#include <3ds.h>
#include <stdio.h>
#include <sf2d.h>
#include <sftd.h>
#include <string>
#include <cstdlib>
#include <fstream>

#include "sys.h"
#include "global.h"
#include "util.h"
#include "3dsx.h"
#include "titles.h"

int main(int argc, const char * argv[])
{
    sysInit();

    hidScanInput();

    u32 held = hidKeysHeld();
    if((held & KEY_R) && (held & KEY_L))
        devMode = true;
    //This is for making sure I didn't butcher the font
    else if(held & KEY_R)
        sysLanguage = CFG_LANGUAGE_JP;

    if(runningUnder() && !devMode)
    {
        hbl = true;
        start3dsxMode();
    }
    else
    {
        sdTitlesInit();
        for(unsigned i = 0; i < sdTitle.size(); i++)
            renameDir(sdTitle[i]);
        nandTitlesInit();
        for(unsigned i = 0; i < nandTitle.size(); i++)
            renameDir(nandTitle[i]);

        while(aptMainLoop() && !kill)
        {
            handleState();
        }
    }

    sysExit();
    return 0;
}
