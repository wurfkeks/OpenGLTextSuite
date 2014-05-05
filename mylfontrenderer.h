#ifndef MYLFONTRENDERER_H
#define MYLFONTRENDERER_H

#include "fontrenderer.h"
#include "lfontrenderer/lfontrenderer.h"

class MyLFontRenderer : public FontRenderer
{
    public:
        MyLFontRenderer();
        virtual ~MyLFontRenderer();
        void renderText(const double x, const double y, const double angle, const QString &string, OGLWidget *widget);
        QString name();
        void init();
        void setHeight(double height);

    private:
        static bool mRegistered;
        static bool setup();

        LFontRenderer mRenderer;
};

#endif // MYLFONTRENDERER_H
