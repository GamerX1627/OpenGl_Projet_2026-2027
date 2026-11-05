#include "Araignee.h"
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <cmath> // pour cosf et sinf (représentation paramétrique)
#include <iostream>
using namespace std;

const float PI = 3.14159265358979f; // pour les angles theta et phi de l'abdomen

Araignee::Araignee() {
  angleHanche = -30.0f;
  angleGenou = 50.0f;
  angleCheville = 45.0f;
  rayonTete = 0.15f;
  // dimensions Cephalothorax
  rayonCephalothorax = 0.25f;
  valEtire =
      rayonCephalothorax / 1.1f; // vaut rayonCephalo/1.1 dans notre cas pour
                                 // avoir des dimensions qui sont potables
  hautCephalo = rayonCephalothorax / 2.0f; // vaudra rayonCephalo/2

  // dimensions Abdomen (un peu plus gros que le cephalo)
  longueurAbdomen = 0.8f; // selon z, demi-axe c = 0.25
  largeurAbdomen = 0.7f;  // selon x, demi-axe a = 0.2
  hauteurAbdomen = 0.4f;  // selon y, demi-axe b = 0.2

  /*dimensions patte*/
  // cuisse
  longueurCuisse = rayonCephalothorax * 2.0f; // 0.5
  rayonCuisse = 0.024f;
  // jambe
  longueurJambe = rayonCephalothorax * 2.2f; // 0.55
  rayonJambe = 0.020f;
  // cheville
  longueurPied = rayonCephalothorax * 1.1f;
  rayonPied = 0.014f;
  // dimensions rotule (articulation)
  rayonRotule = 0.034f;

  // remplissage des tableaux de l'abdomen (une seule fois)
  // les dimensions doivent deja etre initialisées avant cet appel
  construireAbdomen(32, 32);
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
  // jambe de l'araignée
  glColor3f(0.529f, 0.529f, 0.529f);
  glTranslatef(0.0f, 0.0f,
               rayonRotule -
                   (50.0 / 100) *
                       rayonRotule); // pourquoi le -50% c'est pour que le
                                     // cylindre ne soit pas visible
  glRotatef(angleGenou, 1, 0, 0);
  glutSolidCylinder(rayonJambe, longueurJambe, 32, 1);
  // cheville de l'araignée;
  glColor3f(1.0, 0, 0);
  glTranslatef(0, 0, longueurJambe);
  glutSolidSphere(rayonRotule, 16, 16);
  // pied de l'araignée;
  glColor3f(0.529f, 0.529f, 0.529f);
  glTranslatef(0, 0, rayonRotule);
  glRotatef(angleCheville, 1, 0, 0);
  glutSolidCylinder(rayonPied, longueurPied, 16, 1);
  glPopMatrix();
  glPopMatrix();
}
void Araignee::dessinerTete() {
  glPushMatrix();
  glColor3f(0.0f, 1.0f, 0.0f);
  glTranslatef(0.0, 0.0,
               rayonCephalothorax + rayonTete -
                   (20 * (rayonCephalothorax + rayonTete) / 100));
  glutSolidSphere(rayonTete, 16, 16);
  glPopMatrix();
}

// methode construireAbdomen : remplit pAbdomen (sommets) et fAbdomen (faces)
void Araignee::construireAbdomen(int slices, int stacks) {
  // demi-axes de l'ellipsoide (on divise les dimensions totales par 2)
  const float a = largeurAbdomen/2.0f ;  // selon x
  const float b = hauteurAbdomen/2.0f ;  // selon y (axe des poles)
  const float c = longueurAbdomen/2.0f; // selon z

  // on vide les tableaux au cas où on rappelle la méthode
  pAbdomen.clear();
  fAbdomen.clear();

  // tableau des sommets P(i,j) : j parcourt les stacks (phi) et i les slices
  // (theta), indice à plat : k = j * (slices + 1) + i
  for (int j = 0; j <= stacks; j++) {
    float phi = -PI / 2.0f + j * PI / stacks; // latitude, de -pi/2 à pi/2
    for (int i = 0; i <= slices; i++) {
      float theta = i * 2.0f * PI / slices; // longitude, de 0 à 2pi
      pAbdomen.push_back(Point(a * cosf(phi) * cosf(theta), // x
                               b * sinf(phi),               // y
                               c * cosf(phi) * sinf(theta))); // z
    }
  }

  // tableau des faces : chaque case (i,j) est délimitée par 4 sommets
  // dans l'ordre P(i,j), P(i,j+1), P(i+1,j+1), P(i+1,j)
  // (sens trigonométrique vu de l'extérieur)
  for (int j = 0; j < stacks; j++) {
    for (int i = 0; i < slices; i++) {
      int k = j * (slices + 1) + i;
      std::array<int, 4> face = {k,                    // P(i,j)
                                 k + (slices + 1),     // P(i,j+1)
                                 k + (slices + 1) + 1, // P(i+1,j+1)
                                 k + 1};               // P(i+1,j)
      fAbdomen.push_back(face);
    }
  }
}

// methode dessinerAbdomen : on parcourt les faces et on envoie les sommets
void Araignee::dessinerAbdomen() {
  glPushMatrix();
  glColor3f(0.6f, 0.3f, 0.0f);
  // l'abdomen est derrière le cephalo (vers -z) avec 20% de recouvrement
  // comme pour la tête
  glTranslatef(0.0f, 0.0f,
               -(rayonCephalothorax * 1.1f + longueurAbdomen / 2.0f) * 0.8f);
  for (size_t f = 0; f < fAbdomen.size(); f++) {
    glBegin(GL_POLYGON);
    for (int k = 0; k < 4; k++) {
      const Point &p = pAbdomen[fAbdomen[f][k]];
      glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
  }
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
  Araignee::dessinerAbdomen();
  Araignee::dessinerCephalothorax();
}