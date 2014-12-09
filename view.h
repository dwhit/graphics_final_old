#ifndef VIEW_H
#define VIEW_H

// ===========UPDATED===========
#include "GL/glew.h"
#include <QGLWidget>
#include <QTimer>
#ifdef Q_OS_MAC
   #include <glu.h>
#else
    #include <GL/glu.h>
#endif
// ==============================

#include <camera.h>
#include "terrain.h"
#include <transforms.h>
#include <qgl.h>
#include <QTime>
#include <QTimer>



class View : public QGLWidget
{
    Q_OBJECT

public:
    View(QWidget *parent);
    ~View();

private:

    QTime time;
    QTimer timer;

    void initializeGL();
    void paintGL();
    void updateCamera();
    int loadTexture(const QString &filename);
    void resizeGL(int w, int h);
    void rotateCamera(float deltaX, float deltaY);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private slots:
    void tick();

protected:
    Camera *m_camera;
    glm::mat4 m_projectionMatrix;
    glm::mat4 m_viewMatrix;
    float m_xDiff, m_zDiff;
    float m_angleX, m_angleY;
    float m_originalMouseX;
    float m_originalMouseY;



    glm::vec2 m_lastMousePos;

    int m_textureId;
    Terrain m_terrain;
    GLuint m_shader;
    Transforms m_transforms;


protected slots:
     void mouseDragEvent(QMouseEvent *event);


signals:
     void _mouseDrag(QMouseEvent *);

};

#endif // VIEW_H

