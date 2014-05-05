#include "fontstashrenderer.h"
#include <QDebug>

extern QList<FontRenderer*> gFontRenderer;
bool FontStashRenderer::mRegistered = FontStashRenderer::setup();
bool FontStashRenderer::setup() {
    gFontRenderer.append(new FontStashRenderer());
    return true;
}

FontStashRenderer::FontStashRenderer()
    : mFont(NULL)
{
}

void FontStashRenderer::setHeight(double height) {
    fonsSetSize(mFont, height);
}

void FontStashRenderer::init() {
    if (mFont == NULL) {
        mFont = glfonsCreate(512,512,FONS_ZERO_TOPLEFT);
        if (mFont == NULL) {
            qDebug() << "Error: Could not create font stash";
            return;
        }

        mFontID = fonsAddFont(mFont, "sans", FONT_TTF);
        if (mFontID == FONS_INVALID) {
            qDebug() << "Error: Could not add font normal.";
        }

        mColor = glfonsRGBA(255,255,255,255);

        fonsSetFont(mFont, mFontID);
        fonsSetSize(mFont, 12.0f);
        fonsSetColor(mFont, mColor);

        qDebug() << "Generated font";
    }
}

QString FontStashRenderer::name() {
    return "FontStashRenderer";
}

void FontStashRenderer::renderText(const double x, const double y, const double angle, const QString &string, OGLWidget *) {
    glPushMatrix();
    glTranslated(x, y, 0);
    glRotated(angle, 0, 0, 1);

    GLfloat colors[4];
    glGetFloatv(GL_CURRENT_COLOR, colors);

    int color = glfonsRGBA(255*colors[0], 255*colors[1], 255*colors[2], 255*colors[3]);
    fonsSetColor(mFont, color);

    fonsDrawText(mFont, 0, 0, string.toUtf8().constData(), NULL);

    glPopMatrix();
}
