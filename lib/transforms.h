#ifndef TRANSFORMS_H
#define TRANSFORMS_H

#include <glm/glm.hpp>

class Transforms
{
public:
    Transforms();
    glm::mat4 getTransform() const;

    glm::mat4 model, view, projection;
};

#endif // TRANSFORMS_H
