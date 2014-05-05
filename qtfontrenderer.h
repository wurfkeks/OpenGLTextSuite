#ifndef QTFONTRENDERER_H
#define QTFONTRENDERER_H

#include "fontrenderer.h"

class QtFontRenderer : public FontRenderer
{
    public:
        QtFontRenderer();
        void renderText(const double x, const double y, const double angle, const QString &string, OGLWidget *widget);
        QString name();
        void init();
        void setHeight(double height);

    private:
        static bool mRegistered;
        static bool setup();

        QFont mFont;
        QPainter mPainter;
};

#endif // QTFONTRENDERER_H
