#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>
#include <stdio.h>
#include <string>

#include "archive.h"
#include "cart.h"
#include "ui.h"
#include "titledata.h"
#include "global.h"
#include "backupmenu.h"

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

bool cartCheck()
{
    //Check if there is a cart
    bool cartInserted;
    FSUSER_CardSlotIsInserted(&cartInserted);
    if(!cartInserted)
    {
        showMessage("No cartridge inserted!");
        return false;
    }

    //Make sure it's a 3ds game
    FS_CardType type;
    FSUSER_GetCardType(&type);
    if(type!=CARD_CTR)
    {
        showMessage("No DS support.");
        return false;
    }

    return true;
}

void cartManager()
{
    if(!cartCheck())
    {
        state = states::STATE_MAINMENU;
        return;
    }
    //get cart's id
    u64 cartID;
    AM_GetTitleList(NULL, MEDIATYPE_GAME_CARD, 1, &cartID);

    //use titledata to take care of everything
    static titleData cartData;
    if(cartData.id!=cartID || !cartData.initd)
    {
        cartData.init(cartID, MEDIATYPE_GAME_CARD);
    }

    curTitle = &cartData;
    prevState = states::STATE_MAINMENU;

    backupMenu();
}
