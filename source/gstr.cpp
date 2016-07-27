#include <string>

#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <sf2d.h>

#include "gstr.h"
#include "global.h"
#include "date.h"
#include "textbox.h"

std::string GetString(const char *hint)
{
    SwkbdState keyState;
    char input[64];

    swkbdInit(&keyState, SWKBD_TYPE_NORMAL, 2, 64);
    swkbdSetHintText(&keyState, hint);
    swkbdSetFeatures(&keyState, SWKBD_PREDICTIVE_INPUT);
    SwkbdDictWord dates[2];
    swkbdSetDictWord(&dates[0], "2016", GetDate(FORMAT_YDM));
    swkbdSetDictWord(&dates[1], "2016", GetDate(FORMAT_YMD));
    swkbdSetInitialText(&keyState, GetDate(FORMAT_YMD));
    swkbdSetDictionary(&keyState, dates, 2);

    swkbdInputText(&keyState, input, 64);

    return std::string(input);
}

int getInt(const char *hint, int maxValue)
{
    SwkbdState keyState;
    char input[4];

    swkbdInit(&keyState, SWKBD_TYPE_NUMPAD, 2, 4);
    swkbdSetHintText(&keyState, hint);

    SwkbdButton pressed = swkbdInputText(&keyState, input, 4);
    int ret;
    //Cancel
    if(pressed == SWKBD_BUTTON_LEFT)
        ret = -1;
    else
    {
        ret = strtol(input, NULL, 10);
        if(ret > maxValue)
            ret = maxValue;
    }
    return ret;
}
