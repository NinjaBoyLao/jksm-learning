#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>
#include <stdio.h>
#include <string>

#include "archive.h"
#include "savemenu.h"
#include "extmenu.h"
#include "menu.h"
#include "titledata.h"
#include "global.h"
#include "util.h"
#include "ui.h"

enum
{
    _saveOpts,
    _extDataOpts,
    _exit
};

bool openSaveArch3dsx(FS_Archive *arch)
{
    Result res = FSUSER_OpenArchive(arch, ARCHIVE_SAVEDATA, fsMakePath(PATH_EMPTY, ""));
    if(res)
    {
        showMessage("Error opening save archive!");
        return false;
    }

    return true;
}

void fsStart()
{
   Handle fs;
   srvGetServiceHandleDirect(&fs, "fs:USER");
   FSUSER_Initialize(fs);
   fsUseSession(fs);
}

void fsEnd()
{
    fsEndUseSession();
}

void start3dsxMode()
{
    menu opts(128, 96, false);
    opts.addItem("Save Data Options");
    opts.addItem("Extra Data Options");
    opts.addItem("Exit");

    u64 id;
    APT_GetProgramID(&id);

    //This doesn't work if you start the FS session.
    titleData data;
    if(!data.init(id, MEDIATYPE_GAME_CARD))
        if(!data.init(id, MEDIATYPE_SD))
            data.init(id, MEDIATYPE_NAND);

    std::u32string info = tou32(data.name) + U" : 3DSX Mode";

    fsStart();

    while(!kill)
    {
        hidScanInput();

        u32 up = hidKeysUp();

        touchPosition p;
        hidTouchRead(&p);

        opts.handleInput(up);
        if(up & KEY_A)
        {
            FS_Archive archive;
            switch(opts.getSelected())
            {
                case _saveOpts:
                    if(openSaveArch3dsx(&archive))
                        startSaveMenu(archive, data);
                    break;
                case _extDataOpts:
                    if(openExtdata(&archive, data, true))
                        startExtMenu(archive, data);
                    break;
                case _exit:
                    kill = true;
                    break;
            }
        }
        else if(up & KEY_START)
            kill = true;

        sf2d_start_frame(GFX_TOP, GFX_LEFT);
            drawTopBar(info);
            opts.draw();
        sf2d_end_frame();

        sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
            data.printInfo();
        sf2d_end_frame();

        sf2d_swapbuffers();
    }

    fsEnd();

    //This fails if used while the FS session is open.
    deleteSV(data);

}
