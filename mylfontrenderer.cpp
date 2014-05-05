#include "mylfontrenderer.h"

#include <string>

extern QList<FontRenderer*> gFontRenderer;
bool MyLFontRenderer::mRegistered = MyLFontRenderer::setup();
bool MyLFontRenderer::setup() {
    gFontRenderer.append(new MyLFontRenderer());
    return true;
}

MyLFontRenderer::MyLFontRenderer() {
    std::string fontName = "efis";
    mRenderer.LoadFont(fontName, FONT_FNT);
    mRenderer.SetActiveFont(fontName);
    mRenderer.SetHeight(12);
    mRenderer.SetWidthScale(1);

    GLfloat colors[4];
    glGetFloatv(GL_CURRENT_COLOR, colors);
    mRenderer.SetColor(colors[0], colors[1], colors[2]);
}

MyLFontRenderer::~MyLFontRenderer() {
}

void MyLFontRenderer::init() {

}

void MyLFontRenderer::setHeight(double height) {
    mRenderer.SetHeight(height);
}

QString MyLFontRenderer::name() {
    return "LFontRenderer";
}

void MyLFontRenderer::renderText(const double x, const double y, const double angle, const QString &string, OGLWidget *) {
    glPushMatrix();
    glTranslated(x, y, 0);
    glRotated(angle, 0, 0, 1);

    mRenderer.StringOut(0, 0, string.toUtf8().constData());
    mRenderer.Draw();

    glPopMatrix();
}
