#include "Point.h"

// constructeur par défaut : le point est à l'origine
Point::Point() {
  x = 0.0f;
  y = 0.0f;
  z = 0.0f;
}

// constructeur avec les coordonnées données
Point::Point(float px, float py, float pz) {
  x = px;
  y = py;
  z = pz;
}