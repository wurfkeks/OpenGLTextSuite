#include "qtfontrenderer.h"

extern QList<FontRenderer*> gFontRenderer;
bool QtFontRenderer::mRegistered = QtFontRenderer::setup();
bool QtFontRenderer::setup() {
    gFontRenderer.append(new QtFontRenderer());
    return true;
}

QtFontRenderer::QtFontRenderer()
    : mFont(FONT_NAME)
{
}

void QtFontRenderer::init() {
}

void QtFontRenderer::setHeight(double height) {
    mFont.setPointSizeF(height);
}

QString QtFontRenderer::name() {
    return "QtFontRenderer";
}

void QtFontRenderer::renderText(const double x, const double y, const double angle, const QString &string, OGLWidget *widget) {
    // rotated text not supported directly;
    if (angle != 0) {
        bool afb = widget->autoFillBackground();
        bool abs = widget->autoBufferSwap();
        widget->setAutoBufferSwap(false);
        widget->setAutoFillBackground(false);

        GLfloat colors[4];
        glGetFloatv(GL_CURRENT_COLOR, colors);

        mPainter.begin(widget);
        if (mPainter.isActive()) {
            QColor color;
            color.setRgbF(colors[0], colors[1], colors[2], colors[3]);
            mPainter.setPen(color);
            mPainter.setFont(mFont);
            mPainter.translate(x,y);
            mPainter.rotate(angle);
            mPainter.drawText(0, 0, string);
            mPainter.end();
        }

        widget->setAutoBufferSwap(abs);
        widget->setAutoFillBackground(afb);
    } else {
        widget->renderText(x, y, 0.0, string, mFont);
    }
}
