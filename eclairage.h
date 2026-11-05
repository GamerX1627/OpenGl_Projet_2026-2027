#ifndef ECLAIRAGE_H
#define ECLAIRAGE_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

class Eclairage
{
private:
    bool actif;         // GL_LIGHTING
    bool eloigneeOn;    // GL_LIGHT0 : source éloignée
    bool projecteurOn;  // GL_LIGHT1 : projecteur

public:
    Eclairage();

    void initialiser();   // à appeler une fois après glutCreateWindow
    void placer() const;  // à appeler dans affichage() après camera.placer()

    void basculer();            // touche l
    void basculerEloignee();    // touche 1
    void basculerProjecteur();  // touche 2

    bool estActif() const { return actif; }

    // couleur d'un matériau : ambiante = diffuse (cours), reflet blanc
    static void materiau(GLfloat r, GLfloat v, GLfloat b, GLfloat brillance = 50.0f);
};

#endif
