#ifndef CAMERA_H
#define CAMERA_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

class Camera
{
private:
    GLfloat theta;     // angle horizontal (degrés)
    GLfloat phi;       // angle vertical (degrés)
    GLfloat distance;  // distance à la créature

    static const GLfloat DIST_MIN;
    static const GLfloat DIST_MAX;
    static const GLfloat PHI_MAX;

public:
    Camera(GLfloat theta = 0.0f, GLfloat phi = 20.0f, GLfloat distance = 6.0f);

    void placer() const;
    void tourner(GLfloat dTheta, GLfloat dPhi);
    void zoomer(GLfloat pas);
};

#endif
