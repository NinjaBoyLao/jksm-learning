#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>
#include <string>

#include "savemenu.h"
#include "backup.h"
#include "restore.h"
#include "menu.h"
#include "titledata.h"
#include "global.h"
#include "ui.h"
#include "util.h"

enum
{
    _expSav,
    _impSav,
    _browseSav,
    _delSav,
    _back
};

void startSaveMenu(FS_Archive saveArch, const titleData dat)
{
    createTitleDir(dat, MODE_SAVE);

    menu saveMenu(128, 80, false);
    saveMenu.addItem("Export Save");
    saveMenu.addItem("Import Save");
    saveMenu.addItem("Browse SD for Data");
    saveMenu.addItem("Delete Save Data");
    saveMenu.addItem("Back");

    std::u32string info = tou32(dat.name) + U" : Save Data";

    bool loop = true;

    while(loop && !kill)
    {
        hidScanInput();

        u32 up = hidKeysUp();

        touchPosition p;
        hidTouchRead(&p);

        saveMenu.handleInput(up);

        killApp(up);

        if(up & KEY_A)
        {
            switch(saveMenu.getSelected())
            {
                case _expSav:
                    backupData(dat, saveArch, MODE_SAVE, false);
                    break;
                case _impSav:
                    restoreData(dat, saveArch, MODE_SAVE);
                    break;
                case _browseSav:
                    restoreDataSDPath(dat, saveArch, MODE_SAVE);
                    break;
                case _delSav:
                    if(confirm("Are you sure you want to delete this title's current save data?"))
                    {
                        FSUSER_DeleteDirectoryRecursively(saveArch, fsMakePath(PATH_ASCII, "/"));
                        FSUSER_ControlArchive(saveArch, ARCHIVE_ACTION_COMMIT_SAVE_DATA, NULL, 0, NULL, 0);
                    }
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
            saveMenu.draw();
        sf2d_end_frame();

        sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
        sf2d_end_frame();

        sf2d_swapbuffers();
    }
}
