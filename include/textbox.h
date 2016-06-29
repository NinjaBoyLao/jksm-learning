#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <string>

class textbox
{
    public:
        textbox(unsigned x, unsigned y, unsigned width, unsigned height, const char *text);
        void draw(bool Pressed);

        unsigned X, Y;
    protected:
        unsigned Width, Height;
        std::string Text;
};

void textboxInit();
void textboxExit();

#endif // TEXTBOX_H
