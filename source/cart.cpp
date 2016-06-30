#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>
#include <stdio.h>
#include <string>

#include "archive.h"
#include "cart.h"
#include "menu.h"
#include "backup.h"
#include "restore.h"
#include "ui.h"
#include "util.h"
#include "titledata.h"
#include "global.h"
#include "savemenu.h"
#include "extmenu.h"

enum
{
    _saveOpts,
    _extDataOpts,
    _back
};

//We can try two different ways with carts
bool openCart(FS_Archive *out, const titleData dat)
{
    //if first way works
    if(openCartArch(out))
        return true;
    else if(openSaveArch(out, dat, true))
        return true;

    return false;
}

bool gatewayCopy(const titleData dat)
{
    char path[32];
    sprintf(path,"/%016llX.sav", dat.id);

    FILE *save = fopen(path, "rb");
    if(save==NULL)
        return false;

    fseek(save, 0, SEEK_END);
    unsigned size = ftell(save);
    fseek(save, 0, SEEK_SET);

    u8 *buff = new u8[size];

    fread(buff, 1, size, save);
    fclose(save);

    save = fopen("/0004000002C23200.sav", "wb");
    fwrite(buff, 1, size, save);
    fclose(save);

    delete[] buff;

    return true;
}

void gatewayCopyBack(const titleData dat)
{
    char path[32];
    sprintf(path, "/%016llX.sav", dat.id);

    FILE *save = fopen("/0004000002C23200.sav", "rb");

    fseek(save, 0, SEEK_END);
    unsigned size = ftell(save);
    fseek(save, 0, SEEK_SET);

    u8 *buff = new u8[size];

    fread(buff, 1, size, save);
    fclose(save);

    save = fopen(path, "wb");
    fwrite(buff, 1, size, save);
    fclose(save);

    delete[] buff;
}


extern std::string extDataConfirm;

void cartManager()
{
    //Check if there is a cart
    bool cartInserted;
    FSUSER_CardSlotIsInserted(&cartInserted);
    if(!cartInserted)
    {
        showMessage("No cartridge inserted!");
        return;
    }

    //Make sure it's a 3ds game
    FS_CardType type;
    FSUSER_GetCardType(&type);
    if(type!=CARD_CTR)
    {
        showMessage("No DS support yet.");
        return;
    }

    //get cart's id
    u64 cartID;
    AM_GetTitleList(NULL, MEDIATYPE_GAME_CARD, 1, &cartID);

    //use titledata to take care of everything
    titleData cartData;
    if(!cartData.init(cartID, MEDIATYPE_GAME_CARD))
    {
        showMessage("Error getting info for cart!");
        return;
    }

    if(gatewayMode)
    {
        bool copied = gatewayCopy(cartData);
        if(!copied)
        {
            showMessage("Couldn't find Gateway sav file!");
            return;
        }
    }

    //top bar info
    //wchar_t for 3ds is 32bit.
    std::u32string info = tou32(cartData.name);
    info += U" : Cart";

    //menu
    menu cartMenu(136, 96, false);
    cartMenu.addItem("Save Data Options");
    cartMenu.addItem("ExtData Options");
    cartMenu.addItem("Back");

    bool loop = true, importSave = false;

    while(loop && !kill)
    {
        hidScanInput();

        u32 up = hidKeysUp();

        cartMenu.handleInput(up);

        if(up & KEY_A)
        {
            FS_Archive archive;
            switch(cartMenu.getSelected())
            {
                case _saveOpts:
                    if(openCart(&archive, cartData))
                        startSaveMenu(archive, cartData);
                    break;
                case _extDataOpts:
                    if(openExtdata(&archive, cartData, true))
                        startExtMenu(archive, cartData);
                    else
                    {
                        if(confirm("Would you like to try to create extra data for this title?"))
                            createExtData(cartData);
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
            cartMenu.draw();
            drawTopBar(info);
        sf2d_end_frame();

        sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
            cartData.printInfo();
        sf2d_end_frame();

        sf2d_swapbuffers();
    }

    //Copy gateway save back and clean up
    if(gatewayMode && importSave)
    {
        gatewayCopyBack(cartData);
        remove("/0004000002C23200.sav");
    }
}
