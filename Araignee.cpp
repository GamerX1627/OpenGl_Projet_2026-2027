#include "Araignee.h"
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

Araignee::Araignee() {
    Araignee::angleGenou = -70.0f;
    Araignee::angleHanche = -35.0f;
}

void Araignee::dessinerPatte(float angleBase, float decalagePhase){
    //cuisse de l'araignée
    glPushMatrix();
    glColor3f(0.529f, 0.529f, 0.529f);
    glutSolidCylinder(0.12f, 1.8f, 32, 1);
}
void Araignee::dessiner(){
    Araignee::dessinerPatte(0.0f, 0.0f) ;
}
