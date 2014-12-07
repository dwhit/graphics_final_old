QT += core gui opengl

TARGET = final
TEMPLATE = app

# If you add your own folders, add them to INCLUDEPATH and DEPENDPATH, e.g.
INCLUDEPATH += glm lib
DEPENDPATH += glm lib

SOURCES += main.cpp \
    mainwindow.cpp \
    view.cpp \
    lib/ResourceLoader.cpp \
    ui/Canvas3D.cpp \
    ui/SupportCanvas3D.cpp \
    camera/OrbitingCamera.cpp \
    scene/OpenGLScene.cpp \
    scene/Scene.cpp

HEADERS += mainwindow.h \
    view.h \
    lib/ResourceLoader.h \
    ui/Canvas3D.h \
    ui/SupportCanvas3D.h \
    lib/CS123Common.h \
    camera/Camera.h \
    camera/OrbitingCamera.h \
    scene/OpenGLScene.h \
    scene/Scene.h \
    lib/CS123SceneData.h \
    ui/Settings.h

FORMS += mainwindow.ui

LIBS += -L/course/cs123/lib/glew/glew-1.10.0/include -lGLEW
INCLUDEPATH += /course/cs123/lib/glew/glew-1.10.0/include
DEPENDPATH += /course/cs123/lib/glew/glew-1.10.0/include

OTHER_FILES += \
    shaders/shader.frag \
    shaders/shader.vert
