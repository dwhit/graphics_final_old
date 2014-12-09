#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

#include <QHash>
#include "GL/glew.h"

class ResourceLoader
{
public:
    ResourceLoader();
    static GLuint loadShaders(const char * vertex_file_path,const char * fragment_file_path);

private:
    QHash<QString, GLuint> m_shaderPrograms; // hash table for easy access to shaders
};

#endif // RESOURCELOADER_H
