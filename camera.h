#ifndef CAMERA_H
#define CAMERA_H

#include "glm/vec3.hpp"

// "near" and "far" used to be pointer modifiers in 16-bit DOS, and
// apparently are still #defined in Windows headers somewhere.
// #undef-ing them allows the lab to compile in Windows. Neat!
#undef near
#undef far
struct Camera
{
    glm::vec3 eye, center, up;
    float angle, near, far;
};

#endif // CAMERA_H
