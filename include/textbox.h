#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <string>

class textbox
{
    public:
        textbox(unsigned x, unsigned y, unsigned width, unsigned height, const char *text, const char *_head);
        void changeText(const char *nText);
        void draw();

        unsigned X, Y;
    protected:
        unsigned Width, Height;
        unsigned headX;
        float xScale = 0, yScale = 0;
        std::string Text, head;
};

void textboxInit();
void textboxExit();

#endif // TEXTBOX_H
