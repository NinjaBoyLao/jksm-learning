#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <string>

class textbox
{
    public:
        textbox(unsigned x, unsigned y, unsigned width, unsigned height, const char *text);
        void changeText(const char *nText);
        void draw(bool Pressed);

        unsigned X, Y;
    protected:
        unsigned Width, Height;
        float xScale = 0, yScale = 0;
        std::string Text;
};

void textboxInit();
void textboxExit();

#endif // TEXTBOX_H
