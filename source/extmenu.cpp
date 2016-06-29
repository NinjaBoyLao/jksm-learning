#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>
#include <string>

#include "extmenu.h"
#include "titledata.h"
#include "backup.h"
#include "restore.h"
#include "global.h"
#include "ui.h"
#include "util.h"
#include "menu.h"

enum
{
    _expExt,
    _impExt,
    _browseExt,
    _back
};

void startExtMenu(FS_Archive arch, const titleData dat)
{
    createTitleDir(dat, MODE_EXTDATA);

    menu extMenu(128, 80, false);
    extMenu.addItem("Export ExtData");
    extMenu.addItem("Import ExtData");
    extMenu.addItem("Browse for ExtData");
    //I need to get this working right again.
    //extMenu.addItem("Delete ExtData");
    extMenu.addItem("Back");

    std::u32string info = tou32(dat.name) + U" : Extra Data";

    bool loop = true;
    while(loop && !kill)
    {
        hidScanInput();

        u32 up = hidKeysUp();

        touchPosition p;
        hidTouchRead(&p);

        extMenu.handleInput(up);

        killApp(up);

        if(up & KEY_A)
        {
            switch(extMenu.getSelected())
            {
                case _expExt:
                    backupData(dat, arch, MODE_EXTDATA, false);
                    break;
                case _impExt:
                    restoreData(dat, arch, MODE_EXTDATA);
                    break;
                case _browseExt:
                    restoreDataSDPath(dat, arch, MODE_EXTDATA);
                    break;
                case _back:
                    loop = false;
                    break;
            }
        }
        else if(up & KEY_B)
            break;

        sf2d_start_frame(GFX_TOP, GFX_LEFT);
            drawTopBar(info);
            extMenu.draw();
        sf2d_end_frame();

        sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
        sf2d_end_frame();

        sf2d_swapbuffers();
    }
}
