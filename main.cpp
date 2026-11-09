#include <iostream>
#ifdef __APPLE__
#include <GLUT/glut.h> /* Pour Mac OS X */
#else
#include <GL/freeglut.h> /* Pour les autres systemes */
#endif
#include <cstdlib>
#include "Araignee.h"
#include "camera.h"
#include "eclairage.h"

Camera camera;
Eclairage eclairage;
Araignee mkb;

/* gestion de la souris */
char presse;
int xold, yold;

const GLfloat PAS_ZOOM = 0.5f;     // déplacement de la caméra pour z / Z
const GLfloat PAS_ROTATION = 5.0f; // rotation de la caméra pour les flèches

/* animation */
int dernierTemps = 0;

// prototypes de mes fonctions
void affichage();
void clavier(unsigned char touche, int x, int y);
void clavierSpecial(int touche, int x, int y);
void reshape(int x, int y);
void mouse(int bouton, int etat, int x, int y);
void mousemotion(int x, int y);
void animation(int valeur);

// fonction main qui affiche la fenêtre et qui permet de visualiser tout ce que
// j'ai fait jusque là
//  ça va être dur ce projet mais lessgo

int main(int argc, char **argv) {
  // initialisation de GLUT et création de la fenêtre
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(200, 200);
  glutInitWindowSize(1920, 1080);
  glutCreateWindow("Araignee");

  /* Initialisation d'OpenGL */
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glColor3f(1.0, 1.0, 1.0);
  glPointSize(2.0);
  glEnable(GL_DEPTH_TEST);
  eclairage.initialiser();
  mkb.chargerTextures(); // après glutCreateWindow

  /* enregistrement des fonctions de rappel */
  glutDisplayFunc(affichage);
  glutKeyboardFunc(clavier);
  glutSpecialFunc(clavierSpecial); // flèches
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(mousemotion);
  dernierTemps = glutGet(GLUT_ELAPSED_TIME);
  glutTimerFunc(16, animation, 0);

  /* Entree dans la boucle principale glut */
  glutMainLoop();
  return 0;
}

void reshape(int w, int h) {
  if (h == 0)
    h = 1;
  glViewport(0, 0, w, h); // taille du cadre d'affichage dans la fenêtre

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (float)w / (float)h, 0.5, 50.0);
  glMatrixMode(GL_MODELVIEW);
}

void affichage() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glShadeModel(GL_SMOOTH);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  camera.placer();
  eclairage.placer(); // après la caméra : lumières fixes dans la scène
  mkb.dessiner();

  // Repère (sans éclairage pour garder les couleurs pures)
  glDisable(GL_LIGHTING);
  // axe x en rouge
  glBegin(GL_LINES);
  glColor3f(1.0, 0.0, 0.0);
  glVertex3f(0, 0, 0.0);
  glVertex3f(1, 0, 0.0);
  glEnd();
  // axe des y en vert
  glBegin(GL_LINES);
  glColor3f(0.0, 1.0, 0.0);
  glVertex3f(0, 0, 0.0);
  glVertex3f(0, 1, 0.0);
  glEnd();
  // axe des z en bleu
  glBegin(GL_LINES);
  glColor3f(0.0, 0.0, 1.0);
  glVertex3f(0, 0, 0.0);
  glVertex3f(0, 0, 1.0);
  glEnd();
  if (eclairage.estActif())
    glEnable(GL_LIGHTING);

  glFlush();

  // On echange les buffers
  glutSwapBuffers();
}

void clavier(unsigned char touche, int x, int y) {
  switch (touche) {
  case 'p': /* affichage du carre plein */
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glutPostRedisplay();
    break;
  case 'f': /* affichage en mode fil de fer */
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glutPostRedisplay();
    break;
  case 's': /* Affichage en mode sommets seuls */
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    glutPostRedisplay();
    break;
  case 'd':
    glEnable(GL_DEPTH_TEST);
    glutPostRedisplay();
    break;
  case 'D':
    glDisable(GL_DEPTH_TEST);
    glutPostRedisplay();
    break;
  case 'a': // Les faces à l'envers s'affichent en fil de fer
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);
    glutPostRedisplay();
    break;
  case 'z': // la caméra se rapproche
    camera.zoomer(-PAS_ZOOM);
    glutPostRedisplay();
    break;
  case 'Z': // la caméra s'éloigne
    camera.zoomer(PAS_ZOOM);
    glutPostRedisplay();
    break;
  case 'l': // allume / éteint l'éclairage
    eclairage.basculer();
    glutPostRedisplay();
    break;
  case '1': // source éloignée
    eclairage.basculerEloignee();
    glutPostRedisplay();
    break;
  case '2': // projecteur
    eclairage.basculerProjecteur();
    glutPostRedisplay();
    break;
  case 'm': // marche
    mkb.basculerMarche();
    break;
  case 'q': /* q, Q ou Échap quittent le programme */
  case 'Q':
  case 27:
    exit(0);
  }
}

// flèches : la caméra tourne autour de l'araignée
void clavierSpecial(int touche, int x, int y) {
  switch (touche) {
  case GLUT_KEY_LEFT: // par la droite
    camera.tourner(PAS_ROTATION, 0.0f);
    break;
  case GLUT_KEY_RIGHT: // par la gauche
    camera.tourner(-PAS_ROTATION, 0.0f);
    break;
  case GLUT_KEY_UP: // par le bas
    camera.tourner(0.0f, -PAS_ROTATION);
    break;
  case GLUT_KEY_DOWN: // par le haut
    camera.tourner(0.0f, PAS_ROTATION);
    break;
  default:
    return;
  }
  glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
  /* si on appuie sur le bouton gauche */
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    presse = 1; /* le booleen presse passe a 1 (vrai) */
    xold = x;   /* on sauvegarde la position de la souris */
    yold = y;
  }
  /* si on relache le bouton gauche */
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    presse = 0; /* le booleen presse passe a 0 (faux) */
}

void mousemotion(int x, int y) {
  if (presse) /* si le bouton gauche est presse */
  {
    /* la caméra tourne autour de l'araignée, même logique que les flèches */
    camera.tourner(-(x - xold) * 0.5f, (y - yold) * 0.5f);
    glutPostRedisplay(); /* on demande un rafraichissement de l'affichage */
  }

  xold = x; /* sauvegarde des valeurs courante de le position de la souris */
  yold = y;
}

// appelée toutes les 16 ms
void animation(int valeur) {
  int maintenant = glutGet(GLUT_ELAPSED_TIME);
  float dt = (maintenant - dernierTemps) / 1000.0f;
  dernierTemps = maintenant;
  mkb.mettreAJour(dt);
  glutPostRedisplay();
  glutTimerFunc(16, animation, 0);
}