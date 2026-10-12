#include "Araignee.h"
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <cmath>
#include <iostream>
using namespace std;
Araignee::Araignee() {
  Araignee::angleGenou = 70.0f;
  Araignee::angleHanche = -35.0f;
}

// methode dessinerCephalothorax

void Araignee::dessinerCephalothorax() {
  glPushMatrix();
  glColor3f(0.529f, 0.529f, 0.529f);
  glScalef(1.0f, 0.5f,
           1.1); // pour avoir le scale de 1,1 en z dans les dimensions du
                 // cephalo, valEtire vaudra 1/1.1 et le scale en y vaut rayon
                 // cephalo/2;
  glutSolidSphere(rayonCephalothorax, 32, 32);
  glPopMatrix();
}

void Araignee::dessinerPatte(float angleBase, float decalagePhase) {
  glPushMatrix(); // pour faire la rotation en fonction de l'angle donné autour
                  // de y
  glRotatef(angleBase, 0, 1, 0);
  glPushMatrix();
  // cuisse de l'araignée
  glColor3f(0.529f, 0.529f, 0.529f);
  glTranslatef(0, hautCephalo / 2, hautCephalo);
  glRotatef(angleHanche, 1, 0, 0);
  glutSolidCylinder(0.024f, 0.5f, 32, 1);
  // genou de l'araignée
  glColor3f(1.0f, 0.0, 0.0);
  glTranslatef(0.0f, 0.0f, longueurCuisse);
  glutSolidSphere(0.034f, 32, 32);
  //jambe de l'araignée
  glColor3f(0.529f, 0.529f, 0.529f);
  glTranslatef(0.0f, 0.0f, rayonRotule-(50.0/100)*rayonRotule); // pourquoi le -50% c'est pour que le cylindre ne soit pas visible
  glRotatef(angleGenou, 1, 0, 0);
  glutSolidCylinder(rayonCuisse, longueurCuisse, 32, 1);
  glPopMatrix();
  glPopMatrix();
}
void Araignee::dessiner() {
  Araignee::dessinerPatte(45.0f, 0.0f);
  Araignee::dessinerCephalothorax();
}
