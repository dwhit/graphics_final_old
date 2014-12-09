#include "view.h"
#include <QApplication>
#include <QKeyEvent>


//=============Updated============
#include <ResourceLoader.h>
#include <transforms.h>
#include <labcamera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
//================================


View::View(QWidget *parent) : QGLWidget(parent)
{
    // View needs all mouse move events, not just mouse drag events
    setMouseTracking(true);

    // Hide the cursor since this is a fullscreen app
    setCursor(Qt::BlankCursor);

    // View needs keyboard focus
    setFocusPolicy(Qt::StrongFocus);

    // The game loop is implemented using a timer
    connect(&timer, SIGNAL(timeout()), this, SLOT(tick()));


    m_camera = new Camera();
    m_camera->eye.x = 0.0f, m_camera->eye.y = 8.f, m_camera->eye.z = 0.0f;
    m_camera->center.x = 0.0f, m_camera->center.y = 0.0f, m_camera->center.z = 0.0f;
    m_camera->up.x = 0.0f, m_camera->up.y = 1.0f, m_camera->up.z = 0.0f;
    m_camera->angle = 90.0f, m_camera->near = 1.0f, m_camera->far = 1000.0f;

    QObject::connect(this, SIGNAL(_mouseDrag(QMouseEvent*)),this, SLOT(mouseDragEvent(QMouseEvent*)));
    setAutoBufferSwap(false);
    setFocusPolicy(Qt::StrongFocus);
}

View::~View()
{
}

void View::initializeGL()
{
    // All OpenGL initialization *MUST* be done during or after this
    // method. Before this method is called, there is no active OpenGL
    // context and all OpenGL calls have no effect.

    // Start a timer that will try to get 60 frames per second (the actual
    // frame rate depends on the operating system and other running programs)
    time.start();
    connect(&timer, SIGNAL(timeout()), this, SLOT(redraw()));
    timer.start(1000 / 60);


    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));




    // =========== TEMP ===========
    // init VBO/VAO in some shape class?

    glClearColor(0, 0, 0, 0); // Always reset the screen to black before drawing anything
    glEnable(GL_DEPTH_TEST);  // When drawing a triangle, only keep pixels closer to the camera
                              // than what's already been drawn.

    // Make things pretty.
    glEnable(GL_MULTISAMPLE);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    // Cull triangles that are facing away from the camera
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    m_shader = ResourceLoader::loadShaders("/home/dl57/cs123Final/final-template/shaders/terrain.vert", "/home/dl57/cs123Final/final-template/shaders/terrain.frag");
    m_terrain.initGL(glGetAttribLocation(m_shader, "position"),glGetAttribLocation(m_shader, "normal"), glGetAttribLocation(m_shader, "texCoord"));

    // Load the texture. Feel free to substitue with your own image.
    m_textureId = loadTexture("/home/dl57/cs123Final/final-template/textures/snow1.jpg");
    if (m_textureId == -1)


    // ============================



    // Center the mouse, which is explained more in mouseMoveEvent() below.
    // This needs to be done here because the mouse may be initially outside
    // the fullscreen window and will not automatically receive mouse move
    // events. This occurs if there are two monitors and the mouse is on the
    // secondary monitor.
    QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
}

void View::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //Comment these two lines out to use the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    glUseProgram(m_shader);
    glUniform3f(glGetUniformLocation(m_shader, "color"), 1, 1, 1);
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "mvp"), 1, GL_FALSE, &m_transforms.getTransform()[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "m"), 1, GL_FALSE, &m_transforms.model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "v"), 1, GL_FALSE, &m_transforms.view[0][0]);
    glUniform3f(glGetUniformLocation(m_shader, "lightPosition_cameraSpace"),  0.f, 10.f, 1.0f );
    glUniform1i(glGetUniformLocation(m_shader, "textureSampler"), 0);
    m_terrain.draw();
    glUseProgram(0);

    // Force OpenGL to perform all pending operations -- usually a good idea to call this
    glFlush();
    // Swap the buffers to show what we have just drawn onto the screen
    swapBuffers();
}


void View::updateCamera()
{
    float w = width(), h = height();
    float ratio = 1.0f * w / h;

    m_transforms.projection = glm::perspective(m_camera->angle, ratio, m_camera->near, m_camera->far);
    m_transforms.view = glm::lookAt(m_camera->eye, m_camera->center, m_camera->up);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(&m_transforms.projection[0][0]);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(&m_transforms.view[0][0]);
}


/**
 * Generates a new OpenGL texture and loads the image data from the file at the given path into
 * the texture. Makes the new texture active and returns its unique texture ID.
 */
int View::loadTexture(const QString &filename)
{
    // Make sure the image file exists
    QFile file(filename);
    if (!file.exists())
        return -1;

    // Load the file into memory
    QImage image;
    image.load(file.fileName());
    image = image.mirrored(false, true);
    QImage texture = QGLWidget::convertToGLFormat(image);

    // Generate a new OpenGL texture ID to put our image into
    GLuint id = 0;
    glGenTextures(1, &id);

    // Make the texture we just created the new active texture
    glBindTexture(GL_TEXTURE_2D, id);

    // Copy the image data into the OpenGL texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width(), texture.height(), GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());

    // Set filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set coordinate wrapping options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    return id;
}



void View::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void View::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton)
        m_lastMousePos.x = event->x(); m_lastMousePos.y = event->y();
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    // This starter code implements mouse capture, which gives the change in
    // mouse position since the last mouse movement. The mouse needs to be
    // recentered after every movement because it might otherwise run into
    // the edge of the screen, which would stop the user from moving further
    // in that direction. Note that it is important to check that deltaX and
    // deltaY are not zero before recentering the mouse, otherwise there will
    // be an infinite loop of mouse move events.
    int deltaX = event->x() - width() / 2;
    int deltaY = event->y() - height() / 2;
    if (!deltaX && !deltaY) return;
    QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));

    // TODO: Handle mouse movements here

    if (event->buttons() & Qt::LeftButton || event->buttons() & Qt::RightButton)
    {
        emit _mouseDrag(event);
        m_lastMousePos.x = event->x(); m_lastMousePos.y = event->y();
    }
}

void View::mouseDragEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::RightButton || event->buttons() & Qt::LeftButton)
    {
        float dx = event->x() - m_lastMousePos.x,
              dy = event->y() - m_lastMousePos.y;

        double x = m_camera->eye.x, y = m_camera->eye.y, z = m_camera->eye.z,
               r = sqrt(m_camera->eye.x * m_camera->eye.x +
                        m_camera->eye.y * m_camera->eye.y +
                        m_camera->eye.z * m_camera->eye.z),
               theta = acos(y / r) - dy * 0.01f,
               phi = atan2(z, x) + dx * 0.01f;

        if (theta > M_PI-.1) theta = M_PI - .1;
        if (theta < .1) theta = .1;

        m_camera->eye.x = r * sin(theta) * cos(phi);
        m_camera->eye.y = r * cos(theta);
        m_camera->eye.z = r * sin(theta) * sin(phi);

        updateCamera();
    }
}

void View::mouseReleaseEvent(QMouseEvent *event)
{

}

void View::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) QApplication::quit();

    // TODO: Handle keyboard presses here
}

void View::keyReleaseEvent(QKeyEvent *event)
{
}

void View::tick()
{
    // Get the number of seconds since the last tick (variable update rate)
    float seconds = time.restart() * 0.001f;

    // TODO: Implement the demo update here

    // Flag this view for repainting (Qt will call paintGL() soon after)
    update();
}
