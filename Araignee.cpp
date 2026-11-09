#include "Araignee.h"
#include "eclairage.h"
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <cmath>
#include <cstdio> // avant jpeglib.h
#include <iostream>
#include <jpeglib.h>
using namespace std;

const float PI = 3.14159265358979f;
const float RAYON_TRAJET = 1.2f;
const float VITESSE_MARCHE = 30.0f; // degrés par seconde
const float FLEXION = 8.0f;         // degrés
const float PERIODE_FLEXION = 3.0f; // secondes
const float TRANSITION_IDLE = 0.5f; // secondes

// charge un jpeg dans un tableau RGBA (lignes à l'envers pour OpenGL)
static bool chargerJPEG(const char *nom, std::vector<GLubyte> &px, int &w,
                        int &h) {
  FILE *f = fopen(nom, "rb");
  if (!f)
    return false;
  jpeg_decompress_struct cinfo;
  jpeg_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, f);
  jpeg_read_header(&cinfo, TRUE);
  cinfo.out_color_space = JCS_RGB;
  jpeg_start_decompress(&cinfo);
  w = cinfo.output_width;
  h = cinfo.output_height;
  px.resize(w * h * 4);
  std::vector<JSAMPLE> ligne(w * 3);
  while (cinfo.output_scanline < cinfo.output_height) {
    int y = cinfo.output_scanline;
    JSAMPROW r = ligne.data();
    jpeg_read_scanlines(&cinfo, &r, 1);
    for (int x = 0; x < w; x++) {
      int d = ((h - 1 - y) * w + x) * 4;
      px[d] = ligne[3 * x];
      px[d + 1] = ligne[3 * x + 1];
      px[d + 2] = ligne[3 * x + 2];
      px[d + 3] = 255;
    }
  }
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(f);
  return true;
}

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

  // dimensions Abdomen
  longueurAbdomen = 0.8f;
  largeurAbdomen = 0.7f;
  hauteurAbdomen = 0.4f;

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

  temps = 0.0f;
  tempsIdle = 0.0f;
  facteurIdle = 1.0f;
  marche = false;
  angleMarche = 0.0f;
  texAbdomen = 0;
  construireAbdomen(32, 32);
}

void Araignee::mettreAJour(float dt) {
  tempsIdle += dt;
  // la flexion disparaît progressivement pendant la marche
  float cible = marche ? 0.0f : 1.0f;
  float pas = dt / TRANSITION_IDLE;
  if (facteurIdle < cible) {
    facteurIdle += pas;
    if (facteurIdle > cible)
      facteurIdle = cible;
  } else {
    facteurIdle -= pas;
    if (facteurIdle < cible)
      facteurIdle = cible;
  }
  if (!marche)
    return;
  temps += dt;
  angleMarche += VITESSE_MARCHE * dt;
  if (angleMarche >= 360.0f)
    angleMarche -= 360.0f;
}

void Araignee::basculerMarche() { marche = !marche; }

float Araignee::flexion() const {
  return FLEXION * facteurIdle * sinf(2.0f * PI / PERIODE_FLEXION * tempsIdle);
}

// hauteur du pied d'une patte par rapport au corps
float Araignee::hauteurPied(float hanche, float genou) const {
  const float r = PI / 180.0f;
  return hautCephalo / 2 - (longueurCuisse + rayonRotule / 2) * sinf(hanche * r) -
         (longueurJambe + rayonRotule) * sinf((hanche + genou) * r) -
         longueurPied * sinf((hanche + genou + angleCheville) * r);
}

// à appeler après glutCreateWindow
void Araignee::chargerTextures() {
  std::vector<GLubyte> px;
  int w, h;
  if (!chargerJPEG("abdomen.jpg", px, w, h)) {
    std::cerr << "abdomen.jpg introuvable" << std::endl;
    return;
  }
  glGenTextures(1, &texAbdomen);
  glBindTexture(GL_TEXTURE_2D, texAbdomen);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, w, h, GL_RGBA, GL_UNSIGNED_BYTE,
                    px.data());
}

// methode dessinerCephalothorax

void Araignee::dessinerCephalothorax() {
  glPushMatrix();
  Eclairage::materiau(0.0f, 0.0f, 1.0f);
  glScalef(1.0f, 0.5f,
           1.1); // pour avoir le scale de 1,1 en z dans les dimensions du
                 // cephalo, valEtire vaudra 1/1.1 et le scale en y vaut rayon
                 // cephalo/2;
  glutSolidSphere(rayonCephalothorax, 16, 16);
  glPopMatrix();
}

void Araignee::dessinerPatte(float angleBase, float decalagePhase) {
  // 1 cycle par seconde
  const float w = 2.0f * PI;
  float f = flexion();
  float hanche = angleHanche - f + 20.0f * sinf(w * temps + decalagePhase);
  float genou = angleGenou + f + 15.0f * sinf(w * temps + decalagePhase + 1.0f);
  glPushMatrix(); // pour faire la rotation en fonction de l'angle donné autour
                  // de y
  glRotatef(angleBase, 0, 1, 0);
  glPushMatrix();
  // cuisse de l'araignée
  Eclairage::materiau(0.529f, 0.529f, 0.529f);
  glTranslatef(0, hautCephalo / 2, hautCephalo);
  glRotatef(hanche, 1, 0, 0);
  glutSolidCylinder(0.024f, 0.5f, 32, 1);
  // genou de l'araignée
  Eclairage::materiau(1.0f, 0.0f, 0.0f);
  glTranslatef(0.0f, 0.0f, longueurCuisse);
  glutSolidSphere(0.034f, 16, 16);
  // jambe de l'araignée
  Eclairage::materiau(0.529f, 0.529f, 0.529f);
  glTranslatef(0.0f, 0.0f,
               rayonRotule -
                   (50.0 / 100) *
                       rayonRotule); // pourquoi le -50% c'est pour que le
                                     // cylindre ne soit pas visible
  glRotatef(genou, 1, 0, 0);
  glutSolidCylinder(rayonJambe, longueurJambe, 32, 1);
  // cheville de l'araignée;
  Eclairage::materiau(1.0f, 0.0f, 0.0f);
  glTranslatef(0, 0, longueurJambe);
  glutSolidSphere(rayonRotule, 16, 16);
  // pied de l'araignée;
  Eclairage::materiau(0.529f, 0.529f, 0.529f);
  glTranslatef(0, 0, rayonRotule);
  glRotatef(angleCheville, 1, 0, 0);
  glutSolidCylinder(rayonPied, longueurPied, 16, 1);
  glPopMatrix();
  glPopMatrix();
}
void Araignee::dessinerTete() {
  glPushMatrix();
  Eclairage::materiau(0.0f, 1.0f, 0.0f);
  glTranslatef(0.0, 0.0,
               rayonCephalothorax + rayonTete -
                   (20 * (rayonCephalothorax + rayonTete) / 100));
  glutSolidSphere(rayonTete, 16, 16);
  glPopMatrix();
}

// remplit les sommets, normales, coordonnées de texture et faces
void Araignee::construireAbdomen(int slices, int stacks) {
  // demi-axes
  const float a = largeurAbdomen / 2.0f;
  const float b = hauteurAbdomen / 2.0f;
  const float c = longueurAbdomen / 2.0f;

  pAbdomen.clear();
  nAbdomen.clear();
  tAbdomen.clear();
  fAbdomen.clear();

  // sommets P(i,j), indice à plat k = j * (slices + 1) + i
  for (int j = 0; j <= stacks; j++) {
    float phi = -PI / 2.0f + j * PI / stacks;
    for (int i = 0; i <= slices; i++) {
      float theta = i * 2.0f * PI / slices;
      pAbdomen.push_back(Point(a * cosf(phi) * cosf(theta),
                               b * sinf(phi),
                               c * cosf(phi) * sinf(theta)));

      // normale = (x/a², y/b², z/c²) normalisée
      float nx = cosf(phi) * cosf(theta) / a;
      float ny = sinf(phi) / b;
      float nz = cosf(phi) * sinf(theta) / c;
      float norme = sqrtf(nx * nx + ny * ny + nz * nz);
      nAbdomen.push_back(Point(nx / norme, ny / norme, nz / norme));

      // texture : s = i/slices, t = j/stacks
      tAbdomen.push_back(Point((float)i / slices, (float)j / stacks, 0.0f));
    }
  }

  // faces : P(i,j), P(i,j+1), P(i+1,j+1), P(i+1,j)
  for (int j = 0; j < stacks; j++) {
    for (int i = 0; i < slices; i++) {
      int k = j * (slices + 1) + i;
      std::array<int, 4> face = {k, k + (slices + 1), k + (slices + 1) + 1,
                                 k + 1};
      fAbdomen.push_back(face);
    }
  }
}

void Araignee::dessinerAbdomen() {
  glPushMatrix();
  bool texture = (texAbdomen != 0);
  if (texture)
    Eclairage::materiau(1.0f, 1.0f, 1.0f); // blanc pour garder la texture
  else
    Eclairage::materiau(0.6f, 0.3f, 0.0f);
  // derrière le cephalo avec 20% de recouvrement comme pour la tête
  glTranslatef(0.0f, 0.0f,
               -(rayonCephalothorax * 1.1f + longueurAbdomen / 2.0f) * 0.8f);
  if (texture) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texAbdomen);
  }
  for (size_t f = 0; f < fAbdomen.size(); f++) {
    glBegin(GL_POLYGON);
    for (int k = 0; k < 4; k++) {
      int idx = fAbdomen[f][k];
      const Point &p = pAbdomen[idx];
      const Point &n = nAbdomen[idx];
      const Point &t = tAbdomen[idx];
      glTexCoord2f(t.x, t.y);
      glNormal3f(n.x, n.y, n.z);
      glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
  }
  if (texture)
    glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

void Araignee::dessiner() {
  glPushMatrix();
  // trajectoire circulaire autour du point (RAYON_TRAJET, 0, 0)
  glTranslatef(RAYON_TRAJET, 0.0f, 0.0f);
  glRotatef(angleMarche, 0.0f, 1.0f, 0.0f);
  glTranslatef(-RAYON_TRAJET, 0.0f, 0.0f);
  // le corps descend quand les pattes fléchissent, les pieds restent au même niveau
  float f = flexion();
  glTranslatef(0.0f,
               hauteurPied(angleHanche, angleGenou) -
                   hauteurPied(angleHanche - f, angleGenou + f),
               0.0f);
  // pattes en damier (décalage 0 ou PI)
  Araignee::dessinerPatte(45.0f, 0.0f);
  Araignee::dessinerPatte(-45.0f, PI);
  Araignee::dessinerPatte(75.0f, PI);
  Araignee::dessinerPatte(-75.0f, 0.0f);
  Araignee::dessinerPatte(105.0f, 0.0f);
  Araignee::dessinerPatte(-105.0f, PI);
  Araignee::dessinerPatte(135.0f, PI);
  Araignee::dessinerPatte(-135.0f, 0.0f);
  Araignee::dessinerTete();
  Araignee::dessinerAbdomen();
  Araignee::dessinerCephalothorax();
  glPopMatrix();
}