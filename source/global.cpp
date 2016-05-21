#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>
#include <string>

#include "global.h"
#include "date.h"
#include "sys.h"

sftd_font *yashi;

sf2d_texture *bar;

FS_Archive sdArch;

bool useTouch, _date;

bool devMode = false;

bool kill = false;

//default colors
u8 clearColor[3] = {0, 0, 0};
u8 selColor[3] = {0, 255, 0};
u8 unSelColor[3] = {128, 128, 128};

//draws the bar shown up top
void drawTopBar(const std::u32string nfo)
{
    //the bar
    sf2d_draw_texture_scale(bar, 0, 0, 25, 1);

    //nfo is wide text, UTF32
    sftd_draw_wtext(yashi, 0, 0, RGBA8(0, 0, 0, 255), 12, (wchar_t *)nfo.data());

    //time
    sftd_draw_text(yashi, 360, 0, RGBA8(0, 0, 0, 255), 12, RetTime().c_str());
}

void killApp(u32 up)
{
    if(up & KEY_START)
        kill = true;
}
