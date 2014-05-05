#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QGLWidget>
#include <QTimer>
#include "fontrenderer.h"
class FontRenderer;

class OGLWidget : public QGLWidget
{
        Q_OBJECT
    public:
        explicit OGLWidget(QWidget *parent = 0);

        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();

        void setAutoBufferSwap(bool on) { QGLWidget::setAutoBufferSwap(on); }
        bool autoBufferSwap() const { return QGLWidget::autoBufferSwap(); }

        void setAutoFillBackground(bool enabled) { QGLWidget::setAutoFillBackground(enabled); }
        bool autoFillBackground() const { return QGLWidget::autoFillBackground(); }

    signals:
        void signalFPS(QString fps);

    public slots:
        void setFontRenderer(QString renderer);
        void updateStringCount(int count);
        void varyHeight(bool vary);
        void rotate(bool rotate);
        void varyColor(bool vary);

    private slots:
        void updateFPS();

    private:
        QTimer mRedrawTimer;
        QTimer mFPSTimer;
        int mFPS;
        int mFrames;
        int mStringCount;
        FontRenderer *mRenderer;
        bool mFontInitialized;
        bool mVaryHeight;
        bool mRotate;
        bool mVaryColor;
};

#endif // TESTWINDOW_H
