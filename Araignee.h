#ifndef ARAIGNEE_H
#define ARAIGNEE_H
#include <GL/freeglut.h>
// includes nécessaires à la déclaration
class Araignee {
public:
  Araignee(); // constructeur par défaut
  void mettreAJour(/*à voir après en fonction de quel paramètres je pourrai le mettre à jour*/);
  void dessiner(); // qui va faire appel à mes méthodes privées pour
                         // dessiner explicitement chaque partie de l'araignee
private:
  float angleHanche;
  float angleGenou;

  //dimensions Cephalothorax
  float rayonCephalothorax=0.25f;
  float valEtire=rayonCephalothorax/1.1; //vaut rayonCephalo/1.1 dans notre cas pour avoir des dimensions qui sont potables
  float hautCephalo=rayonCephalothorax/2.0f; // vaudra rayonCephalo/2

  //dimensions patte;
  float longueurCuisse=0.5f;
  float rayonCuisse=0.024f;
  
  //dimensions rotule (articulation)
  float rayonRotule=0.034;

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
  void dessinerAbdomen();
};

#endif