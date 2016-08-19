#ifndef BUTTON_H
#define BUTTON_H

#include <3ds.h>
#include <sf2d.h>
#include <string>

#include "textbox.h"

/*
This is those buttons I use.
sText = what button says
sX = X position
sY = Y position
*/
class button
{
    public:
        button(const char *sText, int sX, int sY, int sWidth, int sHeight);
        void draw();
        bool isOver(touchPosition p);
        bool released(touchPosition p);

    private:
        bool Pressed;
        int X, Y, width, height;
        int textX, textY;
        std::string text;
        touchPosition Prev;
};

#endif // BUTTON_H
