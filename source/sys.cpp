#include <3ds.h>
#include <stdio.h>
#include <sf2d.h>
#include <sftd.h>
#include <unistd.h>
#include <sys/stat.h>
#include <vector>

#include "font_ttf.h"
#include "img.h"
#include "global.h"
#include "textbox.h"
#include "ui.h"
#include "menu.h"
#include "util.h"
#include "titles.h"
#include "hbfilter.h"

#include "file.h"

void loadImgs()
{
    topBarInit();
    textboxInit();
    progressBarInit();
}

void freeImgs()
{
    topBarExit();
    textboxExit();
    progressBarExit();
}

void loadCol()
{
    FILE *colBin = fopen("colBin", "rb");

    fread(clearColor, 1, 3, colBin);
    fread(selColor, 1, 3, colBin);
    fread(unSelColor, 1, 3, colBin);

    fclose(colBin);
}

void changeBuffSize()
{
    FILE *bSize = fopen("buff_size", "rb");

    fread(&buff_size, sizeof(u32), 1, bSize);

    fclose(bSize);
}

//I just use this so I don't have to type so much. I'm lazy
void createDir(const char *path)
{
    FSUSER_CreateDirectory(sdArch, fsMakePath(PATH_ASCII, path), 0);
}

void sysInit()
{
    mkdir("/JKSV", 0777);
    chdir("/JKSV");

    if(fexists("colBin"))
        loadCol();
    if(fexists("buff_size"))
        changeBuffSize();
    if(fexists("no_center"))
        centered = false;

    //Start sf2d
    sf2d_init();
    //Set clear to black
    sf2d_set_clear_color(RGBA8(clearColor[0], clearColor[1], clearColor[2], 255));

    loadImgs();

    //Start sftd
    sftd_init();
    //Load font
    if(fexists("font.ttf"))
        font = sftd_load_font_file("font.ttf");
    else
        font = sftd_load_font_mem(font_ttf, font_ttf_size);

    //Start 3ds services
    amInit();
    aptInit();
    srvInit();
    hidInit();
    acInit();
    cfguInit();
    httpcInit(0);

    loadFilterList();

    //Open SDMC archive
    Result Res = FSUSER_OpenArchive(&sdArch, ARCHIVE_SDMC, fsMakePath(PATH_EMPTY, ""));
    if(Res)
    {
        showError("Error opening SDMC archive", (unsigned)Res);
    }

    CFGU_GetSystemLanguage(&sysLanguage);

    createDir("/JKSV/Saves");
    createDir("/JKSV/ExtData");
    createDir("/JKSV/SysSave");
    createDir("/JKSV/Boss");
    createDir("/JKSV/Shared");

    prepareMenus();
}

void sysExit()
{
    //Close SDMC
    FSUSER_CloseArchive(sdArch);

    //exit services
    amExit();
    aptExit();
    srvExit();
    hidExit();
    acExit();
    cfguExit();
    httpcExit();

    freeImgs();

    sftd_free_font(font);
    sftd_fini();
    sf2d_fini();
}
