#include <3ds.h>
#include <stdio.h>
#include <sf2d.h>
#include <sftd.h>
#include <string>
#include <cstdlib>
#include <fstream>

#include "sys.h"
#include "global.h"
#include "menu.h"
#include "titles.h"
#include "sd_cia.h"
#include "nand.h"
#include "cart.h"
#include "extra.h"
#include "shared.h"
#include "hbfilter.h"
#include "util.h"
#include "3dsx.h"
#include "ui.h"

enum
{
    _cart,
    _cia,
    _sys,
    _shared,
    _refresh,
    _extra,
    _exit,
    _test

};

int main(int argc, const char * argv[])
{
    sysInit();

    hidScanInput();
    u32 held = hidKeysHeld();
    if((held & KEY_R) && (held & KEY_L))
        devMode = true;

    if(runningUnder() && !devMode)
    {
        hbl = true;
        start3dsxMode();
    }
    else
    {
        sdTitlesInit();
        nandTitlesInit();

        while(aptMainLoop() && !kill)
        {
            handleState();
        }
    }

    sysExit();
    return 0;
}
