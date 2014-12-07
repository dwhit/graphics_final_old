/**
 * @file   CamtransCamera.cpp
 *
 * This is the perspective camera class you will need to fill in for the Camtrans lab.  See the
 * lab handout for more details.
 */

#include "CamtransCamera.h"
#include <Settings.h>

CamtransCamera::CamtransCamera()
{
    // @TODO: [CAMTRANS] Fill this in...
    m_near = 1;
    m_far = 30;
    m_h_angle = glm::radians(60.f);
    m_w_angle = -1;
    m_aspect = 1;
    m_eye = glm::vec4(2, 2, 2, 0); //in world
    m_look = glm::vec4(-1, -1, -1, 0); //in world
    m_up = glm::vec4(0, 1, 0, 0); //in world

    orientLook(m_eye, m_look, m_up); //sets u,v, and w



}

void CamtransCamera::setAspectRatio(float a) //done
{
    // @TODO: [CAMTRANS] Fill this in...
    m_aspect = a;

}

glm::mat4x4 CamtransCamera::getViewMatrix() const
{
    // @TODO: [CAMTRANS] Fill this in...

    glm::mat4 rotate = glm::transpose(glm::mat4(m_u.x, m_u.y, m_u.z, 0,
                                                m_v.x, m_v.y, m_v.z, 0,
                                                m_w.x, m_w.y, m_w.z, 0,
                                                0,     0,     0,     1));
    glm::mat4 translate = glm::transpose(glm::mat4(1, 0, 0, -m_eye.x,
                                                   0, 1, 0, -m_eye.y,
                                                   0, 0, 1, -m_eye.z,
                                                   0, 0, 0,     1    ));

    glm::mat4 both = glm::transpose(glm::mat4(m_u.x, m_u.y, m_u.z, -m_eye.x,
                                              m_v.x, m_v.y, m_v.z, -m_eye.y,
                                              m_w.x, m_w.y, m_w.z, -m_eye.z,
                                              0,     0,     0,     1));

    return rotate*translate;
    //return both;
}

glm::mat4x4 CamtransCamera::getProjectionMatrix() const
{
    // @TODO: [CAMTRANS] Fill this in...


    return getPerspectiveMatrix()*getScaleMatrix();
    //return getScaleMatrix()*getPerspectiveMatrix();
}

glm::mat4x4 CamtransCamera::getScaleMatrix() const
{
    // @TODO: [CAMTRANS] Fill this in...
    float scale_comp = m_far * tan(glm::radians(getHeightAngle())/2);

    return glm::transpose(glm::mat4( 1/(getAspectRatio() * scale_comp), 0, 0, 0,
                                     0, 1/scale_comp, 0, 0,
                                     0, 0, 1/m_far, 0,
                                     0, 0, 0, 1));
}

glm::mat4x4 CamtransCamera::getPerspectiveMatrix() const
{
    // @TODO: [CAMTRANS] Fill this in...
    float c = -m_near/m_far;

    return glm::transpose(glm::mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, -1/(1+c), c/(1+c),
                                  0, 0, -1, 0));
}

glm::vec4 CamtransCamera::getPosition() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return m_eye;
}

glm::vec4 CamtransCamera::getLook() const //gets -w
{
    // @TODO: [CAMTRANS] Fill this in...
    return -1.f * m_w;
}

glm::vec4 CamtransCamera::getUp() const //get v
{
    // @TODO: [CAMTRANS] Fill this in...
    return m_v;
}

float CamtransCamera::getAspectRatio() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return m_aspect;
}

float CamtransCamera::getHeightAngle() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return m_h_angle;
}

void CamtransCamera::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up)
{
    // @TODO: [CAMTRANS] Fill this in...
    m_eye = eye;
    m_look = look;
    m_up = up;

    m_w = -m_look/glm::length(m_look);
    glm::vec4 v_comp = m_up - glm::dot(m_up, m_w) * m_w;
    m_v = v_comp / glm::length(v_comp);
    glm::vec3 u_comp = glm::cross(glm::vec3(m_v),glm::vec3(m_w));
    m_u = glm::vec4(u_comp, 0);

}

void CamtransCamera::setHeightAngle(float h)
{
    // @TODO: [CAMTRANS] Fill this in...
    m_h_angle = h;

}

void CamtransCamera::translate(const glm::vec4 &v)
{
    // @TODO: [CAMTRANS] Fill this in...
    m_eye.x += v.x;
    m_eye.y += v.y;
    m_eye.z += v.z;

}

void CamtransCamera::rotateU(float degrees)
{
    // @TODO: [CAMTRANS] Fill this in...
    float theta = glm::radians(degrees);
    glm::vec4 tmp_v = m_v*((float) cos(theta)) + m_w*((float) sin(theta));
    glm::vec4 tmp_w = -m_v*((float) sin(theta)) + m_w*((float) cos(theta));

    m_v = tmp_v;
    m_w = tmp_w;

}

void CamtransCamera::rotateV(float degrees)
{
    // @TODO: [CAMTRANS] Fill this in...
    float theta = glm::radians(degrees);
    glm::vec4 tmp_u = m_u*((float) cos(theta)) - m_w*((float) sin(theta));
    glm::vec4 tmp_w = m_u*((float) sin(theta)) + m_w*((float) cos(theta));

    m_u = tmp_u;
    m_w = tmp_w;
}

void CamtransCamera::rotateW(float degrees)
{
    // @TODO: [CAMTRANS] Fill this in...
    float theta = glm::radians(degrees);
    glm::vec4 tmp_u = m_v*((float) sin(theta)) + m_u*((float) cos(theta));
    glm::vec4 tmp_v = m_v*((float) cos(theta)) - m_u*((float) sin(theta));

    m_u = tmp_u;
    m_v = tmp_v;
}

void CamtransCamera::setClip(float nearPlane, float farPlane) //done
{
    // @TODO: [CAMTRANS] Fill this in...
    m_near = nearPlane;
    m_far = farPlane;

}

