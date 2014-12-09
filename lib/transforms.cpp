#include "transforms.h"

Transforms::Transforms()
{
}

glm::mat4 Transforms::getTransform() const{
    return projection * view * model;
}
