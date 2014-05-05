#ifndef FONTSTASHRENDERER_H
#define FONTSTASHRENDERER_H

#include "fontrenderer.h"

#include <stdio.h>
#include <string.h>
#define FONTSTASH_IMPLEMENTATION    // Expands implementation
#include "fontstash/fontstash.h"

#include <QtOpenGL>
#define GLFONTSTASH_IMPLEMENTATION    // Expands implementation
#include "fontstash/glfontstash.h"

class FontStashRenderer : public FontRenderer
{
    public:
        FontStashRenderer();
        void renderText(const double x, const double y, const double angle, const QString &string, OGLWidget *widget);
        QString name();
        void init();
        void setHeight(double height);

    private:
        static bool mRegistered;
        static bool setup();

        struct FONScontext *mFont;
        int mFontID;
        unsigned int mColor;
};

#endif // FONTSTASHRENDERER_H
