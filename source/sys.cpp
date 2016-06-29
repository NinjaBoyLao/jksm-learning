#include <3ds.h>
#include <stdio.h>
#include <sf2d.h>
#include <sftd.h>
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

extern std::vector<u32> filterID;

//Gets the handle of what it's running under and tries to use it
//I haven't found a title that works yet. Maybe I'm doing this wrong?

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
    FILE *colBin = fopen("/homebrew/3ds/JKSV/colBin", "rb");

    for(int i = 0; i < 3; i++)
        clearColor[i] = fgetc(colBin);
    for(int i = 0; i < 3; i++)
        selColor[i] = fgetc(colBin);
    for(int i = 0; i < 3; i++)
        unSelColor[i] = fgetc(colBin);

    fclose(colBin);
}

void changeBuffSize()
{
    FILE *bSize = fopen("/homebrew/3ds/JKSV/buff_size", "rb");

    buff_size = 0;
    for(int i = 0; i < 4; i++)
        buff_size += (fgetc(bSize) << (8 * i));

    fclose(bSize);
}

//I just use this so I don't have to type so much. I'm lazy
void createDir(const char *path)
{
    FSUSER_CreateDirectory(sdArch, fsMakePath(PATH_ASCII, path), 0);
}

void sysInit()
{
    if(fexists("/homebrew/3ds/JKSV/colBin"))
        loadCol();
    if(fexists("/homebrew/3ds/JKSV/buff_size"))
        changeBuffSize();

    loadFilterList();

    //Start sf2d
    sf2d_init();
    //Set clear to black
    sf2d_set_clear_color(RGBA8(clearColor[0], clearColor[1], clearColor[2], 255));

    sf2d_set_3D(0);

    //Load graphics needed
    loadImgs();

    //Start sftd
    sftd_init();
    //Load font
    if(fexists("/homebrew/3ds/JKSV/font.ttf"))
        font = sftd_load_font_file("/homebrew/3ds/JKSV/font.ttf");
    else
        font = sftd_load_font_mem(font_ttf, font_ttf_size);

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
    }

    //Create output directories
    createDir("/homebrew");
    createDir("/homebrew/3ds");
    createDir("/homebrew/3ds/JKSV");

    //These are just in case they used an earlier build. Just moves the folders inside /JKSV
    /*FSUSER_RenameDirectory(sdArch, fsMakePath(PATH_ASCII, "/Saves/"), sdArch, fsMakePath(PATH_ASCII, "/JKSV/Saves/"));
    FSUSER_RenameDirectory(sdArch, fsMakePath(PATH_ASCII, "/ExtData/"), sdArch, fsMakePath(PATH_ASCII, "/JKSV/ExtData/"));*/

    createDir("/homebrew/3ds/JKSV/Saves");
    createDir("/homebrew/3ds/JKSV/ExtData");
    createDir("/homebrew/3ds/JKSV/SysSave");
    createDir("/homebrew/3ds/JKSV/Boss");
    createDir("/homebrew/3ds/JKSV/Shared");
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

    freeImgs();

    sdTitle.clear();
    nandTitle.clear();
    filterID.clear();

    sftd_free_font(font);
    sftd_fini();
    sf2d_fini();
}
