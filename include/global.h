#ifndef GLOBAL_H
#define GLOBAL_H

#include <sf2d.h>
#include <sftd.h>

#include <string>

//this is for backup/restore modes
#define MODE_SAVE 0
#define MODE_EXTDATA 1
#define MODE_SYSSAVE 2
#define MODE_BOSS 3
#define MODE_SHARED 4

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

//Settings
extern bool useTouch, _date;

//Turns off filters and other stuff
extern bool devMode;
extern bool gatewayMode;

//handles killing the app
//this breaks every loop and allows
//sysExit() to run at the end.
extern bool kill;
void killApp(u32 up);

//got tired of black, gray, and green all the time
extern u8 clearColor[3];
extern u8 selColor[3];
extern u8 unSelColor[3];
#endif
