#ifndef LIDARVIEWER_H
#define LIDARVIEWER_H

#include <QWidget>
#include <QOpenGLWidget>

class lidarViewer : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit lidarViewer(QWidget *parent = 0);

signals:

public slots:

private slots:

private:

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

};

#endif // LIDARVIEWER_H
