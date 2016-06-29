#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "titles.h"
#include "menu.h"
#include "global.h"
#include "util.h"
#include "archive.h"
#include "backup.h"
#include "restore.h"
#include "ui.h"
#include "button.h"
#include "textbox.h"
#include "auto.h"
#include "slot.h"
#include "savemenu.h"
#include "extmenu.h"

void sdBackupMenu(const titleData dat);

void sdStartSelect()
{
    if(sdTitle.size() < 1)
        return;

    menu titleMenu(88, 20, true);
    for(unsigned i = 0; i < sdTitle.size(); i++)
        titleMenu.addItem(sdTitle[i].name);

    bool loop = true;

    //Info draw on top of bar
    std::u32string info = U"Select a title";

    //help text for help button
    std::string helpText = "Press L to select multiple. Press R to select all. Press Y to backup selected. Press X to restore selected.";

    //Help button
    button help("Help", 224, 208);

    while(loop && !kill)
    {
        hidScanInput();

        u32 up = hidKeysUp();

        titleMenu.handleInput(up);

        touchPosition pos;
        hidTouchRead(&pos);

        if(up & KEY_A)
        {
            unsigned selected = titleMenu.getSelected();
            sdBackupMenu(sdTitle[selected]);
        }
        else if(help.released(pos))
        {
            showMessage(helpText.c_str());
        }
        else if(up & KEY_Y)
        {
            autoBackup(titleMenu);
        }
        else if(up & KEY_X)
        {
            autoRestore(titleMenu);
        }
        else if(up & KEY_B)
            break;

        killApp(up);

        sf2d_start_frame(GFX_TOP, GFX_LEFT);
            drawTopBar(info);
            titleMenu.draw();
        sf2d_end_frame();

        sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
            sdTitle[titleMenu.getSelected()].printInfo();
            help.draw();
        sf2d_end_frame();

        sf2d_swapbuffers();
    }
}

enum
{
    _saveOpts,
    _extDataOpts,
    _back
};

void sdBackupMenu(const titleData dat)
{
    menu backupMenu(136, 96, false);
    backupMenu.addItem("Save Data Options");
    backupMenu.addItem("ExtData Options");
    backupMenu.addItem("Back");

    bool loop = true;

    std::u32string info = tou32(dat.name);
    info += U" : SD/CIA";
    while(loop && !kill)
    {
        hidScanInput();

        u32 up = hidKeysUp();

        backupMenu.handleInput(up);

        if(up & KEY_A)
        {
            FS_Archive archive;
            switch(backupMenu.getSelected())
            {
                case _saveOpts:
                    if(openSaveArch(&archive, dat, true))
                    {
                        startSaveMenu(archive, dat);
                        deleteSV(dat);
                    }
                    break;
                case _extDataOpts:
                    if(openExtdata(&archive, dat, true))
                    {
                        startExtMenu(archive, dat);
                    }
                    else
                    {
                        if(confirm("Would you like to try to create extra data for this title?"))
                            createExtData(dat);
                    }
                    break;
                case _back:
                    loop = false;
                    break;
            }
            FSUSER_CloseArchive(archive);
        }
        else if(up & KEY_B)
            break;

        killApp(up);

        sf2d_start_frame(GFX_TOP, GFX_LEFT);
            drawTopBar(info);
            backupMenu.draw();
        sf2d_end_frame();

        sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
        sf2d_end_frame();

        sf2d_swapbuffers();
    }
}
