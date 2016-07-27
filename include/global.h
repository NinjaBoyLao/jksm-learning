#ifndef GLOBAL_H
#define GLOBAL_H

#include <sf2d.h>
#include <sftd.h>

#include <string>

#include "titledata.h"

//this is for backup/restore modes
#define MODE_SAVE 0
#define MODE_EXTDATA 1
#define MODE_SYSSAVE 2
#define MODE_BOSS 3
#define MODE_SHARED 4

extern int state, prevState;
extern titleData *curTitle;

enum states
{
    STATE_MAINMENU,
    STATE_CARTMENU,
    STATE_CIAMENU,
    STATE_BACKUPMENU,
    STATE_SAVEMENU,
    STATE_EXTMENU,
    STATE_NANDSELECT,
    STATE_NANDBACKUP,
    STATE_EXTRAS,
    STATE_SHARED,
    STATE_SHAREDBACKUP
};

void handleState();

//buffer size
extern unsigned buff_size;

//This is the font used to print everything
extern sftd_font *font;

//This is the bar you see at the top
extern sf2d_texture *bar;

//SDMC Archive. Seems to write faster using this. Might just be me though
extern FS_Archive sdArch;

//This draws the bar you see at the top
void drawTopBar(const std::u32string nfo);

extern bool devMode, hbl, kill, centered;

//Allows app to be killed by hitting start
void killApp(u32 up);

//got tired of black, gray, and green all the time
extern u8 clearColor[3];
extern u8 selColor[3];
extern u8 unSelColor[3];

void mainMenu();

#endif
