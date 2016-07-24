#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "extra.h"
#include "global.h"
#include "menu.h"
#include "archive.h"
#include "ui.h"
#include "gstr.h"

/*These are extra things I added.
I wasn't sure if there was a cia coin setter
besides Nintendo's official one.
*/

void setPlayCoins()
{
    FS_Archive shared;
    if(openSharedExt(&shared, 0xf000000b))
    {
        int coinAmount = getInt("Enter a number between 0 and 300", 300);
        if(coinAmount != -1)
        {
            Handle coin;
            FSUSER_OpenFile(&coin, shared, fsMakePath(PATH_ASCII, "/gamecoin.dat"), FS_OPEN_READ | FS_OPEN_WRITE, 0);

            u64 size;
            FSFILE_GetSize(coin, &size);

            u8 * buff = new u8[size];

            //Read file to buff
            FSFILE_Read(coin, NULL, 0, buff, size);

            buff[0x4] = coinAmount;
            buff[0x5] = coinAmount >> 8;

            //write it back
            FSFILE_Write(coin, NULL, 0, buff, size, FS_WRITE_FLUSH);

            //close gamecoin.dat
            FSFILE_Close(coin);

            //free memory used by buff
            delete[] buff;

            char message[64];
            sprintf(message, "Play coins set to %u!", coinAmount);
            showMessage(message);
            FSUSER_CloseArchive(shared);
        }
    }
}

void saveColBin()
{
    FILE *colBin = fopen("colBin", "wb");

    fwrite(clearColor, 1, 3, colBin);
    fwrite(selColor, 1, 3, colBin);
    fwrite(unSelColor, 1, 3, colBin);

    fclose(colBin);
}

void setBGColor()
{
    showMessage("Enter RGB info for the color wanted.");

    int RGB[3];

    if( (RGB[0] = getInt("Background Red", 255)) != -1 && (RGB[1] = getInt("Background Green", 255)) != -1 && (RGB[2] = getInt("Background Blue", 255)) != -1)
    {
        for(int i = 0; i < 3; i++)
            clearColor[i] = RGB[i];

        sf2d_set_clear_color(RGBA8(clearColor[0], clearColor[1], clearColor[2], 255));

        saveColBin();
    }
}

void setSelColor()
{
    showMessage("Enter RGB info for the color wanted.");

    int RGB[3];

    if( (RGB[0] = getInt("Selected Red", 255)) != -1 && (RGB[1] = getInt("Selected Green", 255)) != -1 && (RGB[2] = getInt("Selected Blue", 255)) != -1)
    {
        for(int i = 0; i < 3; i++)
            selColor[i] = RGB[i];

        saveColBin();
    }
}

void setUnselColor()
{
    showMessage("Enter RGB info for the color wanted.");

    int RGB[3];
    if( (RGB[0] = getInt("Unselected Red", 255)) != -1 && (RGB[1] = getInt("Unselected Green", 255)) != -1 && (RGB[2] = getInt("Unselected Blue", 255)) != -1)
    {
        for(int i = 0; i < 3; i++)
            unSelColor[i] = RGB[i];

        saveColBin();
    }
}

enum extraOpts
{
    setPlay,
    bgColor,
    slColor,
    unslColor,
    back
};

void extrasMenu()
{
    static menu extra(136, 80, false, true);
    if(extra.getSize()==0)
    {
        extra.addItem("Set Play Coins");
        extra.addItem("Set Background Color");
        extra.addItem("Set Selected Item Color");
        extra.addItem("Set Unselected Item Color");
        extra.addItem("Back");
    }


    hidScanInput();

    u32 down = hidKeysDown();

    extra.handleInput(down, 0);

    if(down & KEY_A)
    {
        switch(extra.getSelected())
        {
            case extraOpts::setPlay:
                setPlayCoins();
                break;
            case extraOpts::bgColor:
                setBGColor();
                break;
            case extraOpts::slColor:
                setSelColor();
                break;
            case extraOpts::unslColor:
                setUnselColor();
                break;
            case extraOpts::back:
                state = STATE_MAINMENU;
                break;
        }
    }
    else if(down & KEY_B)
        state = STATE_MAINMENU;

    killApp(down);

    sf2d_start_frame(GFX_TOP, GFX_LEFT);
        drawTopBar(U"Extras");
        extra.draw();
    sf2d_end_frame();

    sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
    sf2d_end_frame();

    sf2d_swapbuffers();
}
