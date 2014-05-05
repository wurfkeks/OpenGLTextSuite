#ifndef FONTRENDERER_H
#define FONTRENDERER_H

#include <QMap>
#include <QList>
#include "oglwidget.h"

#define FONT_TTF "testfont.ttf"
#define FONT_FNT "testfont.fnt"
#define FONT_NAME "Airbus FMGS Screens"

class OGLWidget;

class FontRenderer
{
    public:
        explicit FontRenderer();
        virtual void renderText(const double x, const double y, const double angle, const QString& string, OGLWidget *widget) = 0;
        virtual QString name() = 0;
        virtual void init() = 0;
        virtual void setHeight(double height) = 0;

        static FontRenderer* getRenderer(const QString& renderer);
        static void registerRenderer(FontRenderer *fr);
        static QList<QString> listRenderer();

    private:
        static QMap<QString, FontRenderer*> sFontRenderer;

};

#endif // FONTRENDERER_H
