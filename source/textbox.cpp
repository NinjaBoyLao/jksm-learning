#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>
#include <sfil.h>
#include <stdio.h>
#include <string.h>

#include "global.h"
#include "textbox.h"
#include "util.h"

static sf2d_texture *tbox;

void textboxInit()
{
    tbox = sfil_load_PNG_file("romfs:/img/textBox.png", SF2D_PLACE_RAM);
}

void textboxExit()
{
    sf2d_free_texture(tbox);
}

textbox::textbox(unsigned x, unsigned y, unsigned width, unsigned height, const char *text, const char *_head)
{
    Text = text;
    evenString(&Text);
    head = _head;

    X = x;
    Y = y;
    Width = width;
    Height = height;

    headX =  X + ((width / 2) - (sftd_get_text_width(font, 12, head.c_str()) / 2));

    if(Width > 32)
        xScale = (float)((Width - 32) / 16);
    if(Height > 32)
        yScale = (float)((Height - 32) / 16);
}

void textbox::draw()
{
    //Top
    sf2d_draw_texture_part(tbox, X, Y, 0, 0, 16, 24);
    sf2d_draw_texture_part_scale(tbox, X + 16, Y, 16, 0, 16, 24, xScale, 1);
    sf2d_draw_texture_part(tbox, (X + 16) + (16 * xScale), Y, 32, 0, 16, 24);

    //Mid
    sf2d_draw_texture_part_scale(tbox, X, Y + 24, 0, 24, 16, 16, 1, yScale);
    sf2d_draw_texture_part_scale(tbox, X + 16, Y + 24, 16, 24, 16, 16, xScale, yScale);
    sf2d_draw_texture_part_scale(tbox, (X + 16) + (16 * xScale), Y + 24, 32, 24, 16, 16, 1, yScale);

    //Bottom
    sf2d_draw_texture_part(tbox, X, (Y + 16) + (16 * yScale), 0, 40, 16, 16);
    sf2d_draw_texture_part_scale(tbox, X + 16, (Y + 16) + (16 * yScale), 16, 40, 16, 16, xScale, 1);
    sf2d_draw_texture_part(tbox, (X + 16) + (16 * xScale), (Y + 16) + (16 * yScale), 32, 40, 16, 16);

    //Text
    sftd_draw_text(font, headX, Y + 4, RGBA8(0, 0, 0, 255), 12, head.c_str());
    sftd_draw_text_wrap(font, X + 8, Y + 26, RGBA8(0, 0, 0, 255), 12, (X + Width) - 8, Text.c_str());
}
