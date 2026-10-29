#ifndef POINT_H
#define POINT_H

// classe Point : un point 3D (sans couleur)
class Point {
public:
  // coordonnées x, y et z du point
  float x;
  float y;
  float z;

  Point();                              // constructeur par défaut (0,0,0)
  Point(float px, float py, float pz);  // constructeur avec coordonnées
};

#endif