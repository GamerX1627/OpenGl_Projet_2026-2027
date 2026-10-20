#include "Araignee.h"
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <iostream>
using namespace std;
Araignee::Araignee() {
  angleHanche = -30.0f;
  angleGenou = 50.0f;
  angleCheville=45.0f;
  rayonTete=0.15f;
  //dimensions Cephalothorax
  rayonCephalothorax = 0.25f;
  valEtire = rayonCephalothorax / 1.1f; //vaut rayonCephalo/1.1 dans notre cas pour avoir des dimensions qui sont potables
  hautCephalo = rayonCephalothorax / 2.0f; // vaudra rayonCephalo/2

  /*dimensions patte*/
  //cuisse
  longueurCuisse = rayonCephalothorax * 2.0f; // 0.5
  rayonCuisse = 0.024f;
  //jambe
  longueurJambe = rayonCephalothorax * 2.2f; // 0.55
  rayonJambe = 0.020f;
  //cheville
  longueurPied = rayonCephalothorax * 1.1f;
  rayonPied = 0.014f;
  //dimensions rotule (articulation)
  rayonRotule = 0.034f;
}

// methode dessinerCephalothorax

void Araignee::dessinerCephalothorax() {
  glPushMatrix();
  glColor3f(0.0f, 0.0f, 1.0f);
  glScalef(1.0f, 0.5f,
           1.1); // pour avoir le scale de 1,1 en z dans les dimensions du
                 // cephalo, valEtire vaudra 1/1.1 et le scale en y vaut rayon
                 // cephalo/2;
  glutSolidSphere(rayonCephalothorax, 16, 16);
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
  glutSolidSphere(0.034f, 16, 16);
  //jambe de l'araignée
  glColor3f(0.529f, 0.529f, 0.529f);
  glTranslatef(0.0f, 0.0f, rayonRotule-(50.0/100)*rayonRotule); // pourquoi le -50% c'est pour que le cylindre ne soit pas visible
  glRotatef(angleGenou, 1, 0, 0);
  glutSolidCylinder(rayonJambe, longueurJambe, 32, 1);
  //cheville de l'araignée;
  glColor3f(1.0, 0, 0);
  glTranslatef(0, 0, longueurJambe);
  glutSolidSphere(rayonRotule, 16, 16);
  //pied de l'araignée;
  glColor3f(0.529f, 0.529f, 0.529f);
  glTranslatef(0, 0, rayonRotule);
  glRotatef(angleCheville, 1, 0, 0);
  glutSolidCylinder(rayonPied, longueurPied, 16, 1);
  glPopMatrix();
  glPopMatrix();
}
void Araignee::dessinerTete(){
  glPushMatrix();
  glColor3f(0.0f, 1.0f, 0.0f);
  glTranslatef(0.0, 0.0, rayonCephalothorax+rayonTete-(20*(rayonCephalothorax+rayonTete)/100));
  glutSolidSphere(rayonTete, 16, 16);
  glPopMatrix();
}
void Araignee::dessiner() {
  Araignee::dessinerPatte(45.0f, 0.0f);
  Araignee::dessinerPatte(-45.0f, 0.0f);
  Araignee::dessinerPatte(75.0f, 0.0f);
  Araignee::dessinerPatte(-75.0f, 0.0f);
  Araignee::dessinerPatte(105.0f, 0.0f);
  Araignee::dessinerPatte(-105.0f, 0.0f);
  Araignee::dessinerPatte(135.0f, 0.0f);
  Araignee::dessinerPatte(-135.0f, 0.0f);
  Araignee::dessinerTete();
  Araignee::dessinerCephalothorax();
}
