#include "camera.h"
#include <cmath>

#define PI 3.14159265358979

const GLfloat Camera::DIST_MIN = 2.0f;
const GLfloat Camera::DIST_MAX = 30.0f;
const GLfloat Camera::PHI_MAX  = 89.0f;   // à 90° gluLookAt ne marche plus

Camera::Camera(GLfloat theta, GLfloat phi, GLfloat distance)
    : theta(theta), phi(phi), distance(distance)
{
}

void Camera::placer() const
{
    GLfloat th = theta * PI / 180.0f;
    GLfloat ph = phi   * PI / 180.0f;

    GLfloat x = distance * cos(ph) * sin(th);
    GLfloat y = distance * sin(ph);
    GLfloat z = distance * cos(ph) * cos(th);

    gluLookAt(x, y, z,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);
}

void Camera::tourner(GLfloat dTheta, GLfloat dPhi)
{
    theta += dTheta;
    if (theta >= 360.0f) theta -= 360.0f;
    if (theta <    0.0f) theta += 360.0f;

    phi += dPhi;
    if (phi >  PHI_MAX) phi =  PHI_MAX;
    if (phi < -PHI_MAX) phi = -PHI_MAX;
}

void Camera::zoomer(GLfloat pas)
{
    distance += pas;
    if (distance < DIST_MIN) distance = DIST_MIN;
    if (distance > DIST_MAX) distance = DIST_MAX;
}
