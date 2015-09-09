#include "lidarviewer.h"
#include <QOpenGLFunctions>

lidarViewer::lidarViewer(QWidget *parent) : QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    this->setFormat(format); // must be called before the widget or its parent window gets shown
}

void lidarViewer::initializeGL()
{
    // Set up the rendering context, load shaders and other resources, etc.:
    /*QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    ...*/
}


void lidarViewer::resizeGL(int w, int h)
{
    // Update projection matrix and other size related settings:
    /*m_projection.setToIdentity();
    m_projection.perspective(45.0f, w / float(h), 0.01f, 100.0f);
    ...*/
}

void lidarViewer::paintGL()
{
    // Draw the scene:
    /*QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT);
    ...*/
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);


}
