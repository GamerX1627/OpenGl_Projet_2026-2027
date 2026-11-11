#ifndef ARAIGNEE_H
#define ARAIGNEE_H
#include <GL/freeglut.h>
// includes nécessaires à la déclaration
#include <array>
#include <vector>
#include "Point.h"
class Araignee {
public:
  Araignee(); // constructeur par défaut
  void mettreAJour(float dt);
  void basculerMarche();
  void dessiner(); // qui va faire appel à mes méthodes privées pour
                         // dessiner explicitement chaque partie de l'araignee
  void chargerTextures(); // à appeler après glutCreateWindow
  // charge un jpeg dans un objet de texture
  static GLuint chargerTexture(const char *nom, GLint wrapS, GLint wrapT);
private:
  float angleHanche;
  float angleGenou;
  float angleCheville;
  float temps;
  float tempsIdle;
  float facteurIdle;
  bool marche;
  float angleMarche; // en degrés
  // dimensions Tête;
  float rayonTete;
  //dimensions Cephalothorax
  float rayonCephalothorax;
  float valEtire; //vaut rayonCephalo/1.1 dans notre cas pour avoir des dimensions qui sont potables
  float hautCephalo; // vaudra rayonCephalo/2
  //dimensions Abdomen
  float longueurAbdomen;
  float largeurAbdomen;
  float hauteurAbdomen;
  // tableaux de l'abdomen
  std::vector<Point> pAbdomen;               // sommets
  std::vector<Point> nAbdomen;               // normales
  std::vector<Point> tAbdomen;               // coordonnées de texture (x = s, y = t)
  std::vector<std::array<int, 4> > fAbdomen; // faces (4 indices)
  GLuint texAbdomen;
  /*dimensions patte;*/
  //cuisse
  float longueurCuisse;
  float rayonCuisse;
  //jambe
  float longueurJambe;
  float rayonJambe;
  //pied
  float longueurPied;
  float rayonPied;
  //dimensions rotule (articulation)
  float rayonRotule;

  float rayonAbdomen; //pour le moment je ne sais pas quoi faire de cette valeur mais je la garde au cas où
  /**
  @param angleBase permet de déterminer l'orientation de la patte
  @param decalagePhase permet de déterminer le décalage de phase pour les
  animations comme ça les pattes ne bougent pas toutes en même temps
  **/
  void dessinerPatte(float angleBase, float decalagePhase);
  void dessinerTete(); // methode qui dessine la tete de notre araignee avec les
                       // yeux.....
  void dessinerCephalothorax(); // dans notre cas c'est la partie sur laquelle
                                // les pattes sont reliées
  /**
  remplit les tableaux de l'abdomen
  @param slices nombre de subdivisions de notre abdomen
  @param stacks nombre de piles de notre abdomen
  **/
  void construireAbdomen(int slices, int stacks);
  void dessinerAbdomen(); // parcourt les tableaux de l'abdomen
  float flexion() const;
  float hauteurPied(float hanche, float genou) const;
};

#endif