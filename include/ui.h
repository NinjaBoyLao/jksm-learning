#ifndef UI_H
#define UI_H

#include <3ds.h>
#include <string>
#include "textbox.h"

bool confirm(const char *t);
void showMessage(const char *t, const char *head);
void showError(const char *t, unsigned error);

class progressBar
{
    public:
        progressBar(float _max, const char *t, const char *head);
        ~progressBar();
        void draw(float cur);

    private:
        textbox *back;
        float max;
};

void progressBarInit();
void progressBarExit();

#endif // UI_H
