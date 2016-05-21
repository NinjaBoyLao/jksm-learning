#include <3ds.h>
#include <stdio.h>
#include <sf2d.h>
#include <sftd.h>
#include <string>
#include <cstdlib>

#include "sys.h"
#include "global.h"
#include "menu.h"
#include "titles.h"
#include "sd_cia.h"
#include "nand.h"
#include "cart.h"
#include "extra.h"
#include "shared.h"
#include "sdpath.h"

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

    sdTitlesInit();
    nandTitlesInit();
    logOpen();

    std::u32string info = U"JKSM 5/21/2016";

    menu mainMenu(136, 80, false);
    mainMenu.addItem("Cartridge");
    mainMenu.addItem("SD/CIA");
    mainMenu.addItem("System Titles");
    mainMenu.addItem("Shared ExtData");
    mainMenu.addItem("Refresh Games");
    mainMenu.addItem("Extras");
    mainMenu.addItem("Exit");
    if(devMode)
        mainMenu.addItem("Test");

    //I use this to break the loop from inside switches.
    //You could use a goto too, but I was always told goto is da devil
    bool loop = true;

    while(aptMainLoop() && loop && !kill)
    {
        hidScanInput();

        u32 up = hidKeysUp();

        mainMenu.handleInput(up);

        touchPosition p;
        hidTouchRead(&p);

        if(up & KEY_A)
        {
            switch(mainMenu.getSelected())
            {
                case _cart:
                    cartManager();
                    break;
                case _cia:
                    sdStartSelect();
                    break;
                case _sys:
                    nandStartSelect();
                    break;
                case _shared:
                    sharedExtManager();
                    break;
                case _refresh:
                    FSUSER_DeleteFile(sdArch, fsMakePath(PATH_ASCII, "/JKSV/titles"));
                    sdTitlesInit();
                    break;
                case _extra:
                    extrasMenu();
                    break;
                case _exit:
                    loop = false;
                    break;
                case _test:
                    getSDPath();
                    break;
            }
        }
        else if(up & KEY_B)
            break;

        killApp(up);

        sf2d_start_frame(GFX_TOP, GFX_LEFT);
        drawTopBar(info);
        mainMenu.draw();
        sf2d_end_frame();

        sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
        sf2d_end_frame();

        sf2d_swapbuffers();
    }

    sysExit();
    return 0;
}
