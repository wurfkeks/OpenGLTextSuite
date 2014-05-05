#include "oglwidget.h"

OGLWidget::OGLWidget(QWidget *parent)
    : QGLWidget(parent)
    , mFPS(0)
    , mFrames(0)
    , mStringCount(10)
    , mRenderer(NULL)
    , mFontInitialized(false)
    , mVaryHeight(false)
    , mRotate(false)
    , mVaryColor(false)
{
    connect(&mRedrawTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
    connect(&mFPSTimer, SIGNAL(timeout()), this, SLOT(updateFPS()));

    mRedrawTimer.start(0);
    mFPSTimer.start(1000);
}

void OGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, w, h, 0.0, 1.0, -1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void OGLWidget::initializeGL() {
    glColor3ub(0,0,0);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);

    int smooth_hint = GL_DONT_CARE;
    glHint(GL_POINT_SMOOTH_HINT, smooth_hint);
    glHint(GL_LINE_SMOOTH_HINT, smooth_hint);
    glHint(GL_POLYGON_SMOOTH_HINT, smooth_hint);
}

void OGLWidget::setFontRenderer(QString renderer) {
    mRenderer = FontRenderer::getRenderer(renderer);
    mFontInitialized = false;
}

void OGLWidget::updateFPS() {
    mFPS = mFrames;
    mFrames = 0;
    emit(signalFPS(QString("FPS: %1").arg(mFPS)));
}

void OGLWidget::paintGL() {
    mFrames += 1;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (mRenderer == NULL)
        return;

    if (!mFontInitialized) {
        mRenderer->init();
        mFontInitialized = true;
    }

    QString line("string %1");

    const int w = width();
    const int h = height();
    srand(mStringCount);

    glColor3ub(255,255,255);
    for (int i=1; i < mStringCount; ++i) {
        int posX = rand() % w;
        int posY = rand() % h;
        int angle = 0;

        if (mRotate)
            angle = (posX*posY) % 360;

        if (mVaryHeight)
            mRenderer->setHeight((posX*posY) % 29 + 4); // between 4 and 32

        if (mVaryColor)
            glColor3ub(posX%256, posY%256, (posX+posY)%256);

        mRenderer->renderText(posX, posY, angle, line.arg(i+1), this);
    }
}

void OGLWidget::updateStringCount(int count) {
    mStringCount = count;
}

void OGLWidget::varyHeight(bool vary) {
    mVaryHeight = vary;
    if (mVaryHeight == false && mRenderer != NULL) {
        mRenderer->setHeight(12);
    }
}

void OGLWidget::rotate(bool rotate) {
    mRotate = rotate;
}

void OGLWidget::varyColor(bool vary) {
    mVaryColor = vary;
    if (!vary)
        glColor3ub(255,255,255);
}
