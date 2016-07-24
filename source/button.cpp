#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>
#include <string>

#include "global.h"
#include "button.h"
#include "img.h"
#include "util.h"

using namespace std;

button::button(const char * sText, int sX, int sY, int sWidth, int sHeight)
{
    Box = new textbox(sX, sY, sWidth, sHeight, sText);

    X = sX;
    Y = sY;
    width = sWidth;
    height = sHeight;

    Pressed = false;
}

button::~button()
{
    delete Box;
}

void button::draw()
{
    Box->draw(Pressed);
}

//This checks whether the stylus is inside the button
bool button::isOver(touchPosition p)
{
    if( (p.px > X && p.px < X + width) && (p.py > Y && p.py < Y + height) )
        return true;

    return false;
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
