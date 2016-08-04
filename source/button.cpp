#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>
#include <string>

#include "global.h"
#include "button.h"
#include "img.h"
#include "util.h"

button::button(const char * sText, int sX, int sY, int sWidth, int sHeight)
{
    X = sX;
    Y = sY;
    width = sWidth;
    height = sHeight;
    text = sText;

    textX =  X + 8;
    textY = Y + 8;

    Pressed = false;
}

void button::draw()
{
    sf2d_draw_rectangle(X - 1, Y - 1, width + 2, height + 2, RGBA8(64, 64, 64, 255));
    if(Pressed)
        sf2d_draw_rectangle(X, Y, width, height, RGBA8(200, 200, 200, 255));
    else
        sf2d_draw_rectangle(X, Y, width, height, RGBA8(244, 244, 244, 255));

    sftd_draw_text(font, textX, textY, RGBA8(0,0,0,255), 12, text.c_str());
}

//This checks whether the stylus is inside the button
bool button::isOver(touchPosition p)
{
   return (p.px > X && p.px < X + width) && (p.py > Y && p.py < Y + height);
}

//Checks if the stylus was removed.
bool button::released(touchPosition p)
{
    Prev = p;
    if(isOver(p))
    {
        Pressed = true;
    }
    else
    {
        //This should now only return true if
        //the stylus is lifted directly off.
        //should return false now if moved away to different part
        if(Pressed && !touchPressed(p))
        {
            Pressed = false;
            return true;
        }
        else
        {
            Pressed = false;
        }
    }
    return false;
}
