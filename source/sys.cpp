#include <3ds.h>
#include <stdio.h>
#include <sf2d.h>
#include <sftd.h>

#include "yashi_ttf.h"
#include "img.h"
#include "global.h"
#include "textbox.h"
#include "ui.h"
#include "menu.h"
#include "util.h"

//Gets the handle of what it's running under and tries to use it
//I haven't found a title that works yet. Maybe I'm doing this wrong?
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

void loadImgs()
{
    bar = sf2d_create_texture_mem_RGBA8(TopBar.pixel_data, TopBar.width, TopBar.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    textboxInit();
    progressBarInit();
    loadArrow();
}

void freeImgs()
{
    sf2d_free_texture(bar);
    textboxExit();
    progressBarExit();
    freeArrow();
}

void loadCol()
{
    FILE *colBin = fopen("/JKSV/colBin", "rb");

    for(int i = 0; i < 3; i++)
        clearColor[i] = fgetc(colBin);
    for(int i = 0; i < 3; i++)
        selColor[i] = fgetc(colBin);
    for(int i = 0; i < 3; i++)
        unSelColor[i] = fgetc(colBin);

}

//I just use this so I don't have to type so much. I'm lazy
void createDir(const char *path)
{
    FSUSER_CreateDirectory(sdArch, fsMakePath(PATH_ASCII, path), 0);
}

void sysInit()
{
    if(fexists("/JKSV/colBin"))
    {
        loadCol();
    }
    //Start sf2d
    sf2d_init();
    //Set clear to black
    sf2d_set_clear_color(RGBA8(clearColor[0], clearColor[1], clearColor[2], 255));
    //Wait for vsync
    sf2d_set_vblank_wait(1);

    //Start sftd
    sftd_init();
    //Load yashi
    yashi = sftd_load_font_mem(yashi_ttf, yashi_ttf_size);

    //Load graphics needed
    loadImgs();

    //Start 3ds services
    amInit();
    aptInit();
    srvInit();
    hidInit();

    //Open SDMC archive
    Result Res = FSUSER_OpenArchive(&sdArch, ARCHIVE_SDMC, fsMakePath(PATH_EMPTY, ""));
    if(Res)
    {
        showMessage("Error opening SDMC archive!");
        logWriteError("SDMC Open", Res);
    }

    //Create output directories
    createDir("/JKSV");

    //These are just in case they used an earlier build. Just moves the folders inside /JKSV
    FSUSER_RenameDirectory(sdArch, fsMakePath(PATH_ASCII, "/Saves/"), sdArch, fsMakePath(PATH_ASCII, "/JKSV/Saves/"));
    FSUSER_RenameDirectory(sdArch, fsMakePath(PATH_ASCII, "/ExtData/"), sdArch, fsMakePath(PATH_ASCII, "/JKSV/ExtData/"));

    createDir("/JKSV/Saves");
    createDir("/JKSV/ExtData");
    createDir("/JKSV/SysSave");
    createDir("/JKSV/Boss");
    createDir("/JKSV/Shared");

    //I decided to leave this in this time around
    //It detects if it's running by itself(CIA) or under something(3DSX)
    //I haven't found a title yet that gives it the right permissions
    //It's also possible that I'm doing it wrong.
    if(runningUnder())
        fsStart();
}

void sysExit()
{
    if(runningUnder())
        fsEnd();
    //Close SDMC
    FSUSER_CloseArchive(sdArch);

    //exit services
    amExit();
    aptExit();
    srvExit();
    hidExit();

    freeImgs();

    sftd_free_font(yashi);
    sftd_fini();
    sf2d_fini();
}
