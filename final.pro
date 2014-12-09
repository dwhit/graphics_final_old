QT += core gui opengl

TARGET = final
TEMPLATE = app

# If you add your own folders, add them to INCLUDEPATH and DEPENDPATH, e.g.
INCLUDEPATH += glm lib objects
DEPENDPATH += glm lib objects

SOURCES += main.cpp \
    mainwindow.cpp \
    view.cpp \
    lib/ResourceLoader.cpp \
    objects/terrain.cpp \
    lib/transforms.cpp

HEADERS += mainwindow.h \
    view.h \
    lib/ResourceLoader.h \
    lib/transforms.h \
    objects/terrain.h \
    camera.h

FORMS += mainwindow.ui

LIBS += -L/course/cs123/lib/glew/glew-1.10.0/include -lGLEW
INCLUDEPATH += /course/cs123/lib/glew/glew-1.10.0/include
DEPENDPATH += /course/cs123/lib/glew/glew-1.10.0/include

RESOURCES += \
    shaders/terrain.qrc

OTHER_FILES += \
    shaders/terrain.frag \
    shaders/terrain.vert
