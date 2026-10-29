#ifndef ARAIGNEE_H
#define ARAIGNEE_H
#include <GL/freeglut.h>
// includes nécessaires à la déclaration
#include <array>  // pour stocker les 4 indices de chaque face de l'abdomen
#include <vector> // pour stocker les sommets et les faces de l'abdomen
#include "Point.h" // classe Point utilisée pour les sommets de l'abdomen
class Araignee {
public:
  Araignee(); // constructeur par défaut
  void mettreAJour(/*à voir après en fonction de quel paramètres je pourrai le mettre à jour*/);
  void dessiner(); // qui va faire appel à mes méthodes privées pour
                         // dessiner explicitement chaque partie de l'araignee
private:
  float angleHanche;
  float angleGenou;
  float angleCheville;
  // dimensions Tête;
  float rayonTete;
  //dimensions Cephalothorax
  float rayonCephalothorax;
  float valEtire; //vaut rayonCephalo/1.1 dans notre cas pour avoir des dimensions qui sont potables
  float hautCephalo; // vaudra rayonCephalo/2
  //dimensions Abdomen
  float longueurAbdomen; // selon l'axe z
  float largeurAbdomen;  // selon l'axe x
  float hauteurAbdomen;  // selon l'axe y
  // tableaux de l'abdomen (remplis une seule fois dans le constructeur)
  std::vector<Point> pAbdomen;               // sommets P(i,j) de l'ellipsoide
  std::vector<std::array<int, 4> > fAbdomen; // indices des 4 sommets de chaque face
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
  methode qui remplit pAbdomen et fAbdomen (calcul des sommets et des faces
  de l'ellipsoide avec sa représentation paramétrique)
  @param slices nombre de subdivisions de notre abdomen (méridiens)
  @param stacks nombre de piles de notre abdomen (parallèles)
  **/
  void construireAbdomen(int slices, int stacks);
  /**
  methode qui dessine l'abdomen en parcourant pAbdomen et fAbdomen
  les dimensions sont celles des attributs longueurAbdomen (axe z),
  largeurAbdomen (axe x) et hauteurAbdomen (axe y)
  **/
  void dessinerAbdomen();
};

#endif