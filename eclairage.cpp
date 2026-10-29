#include "eclairage.h"

Eclairage::Eclairage()
    : actif(true), eloigneeOn(true), projecteurOn(true)
{
}

void Eclairage::initialiser()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);   // normales unitaires malgré les glScalef

    // Lumière ambiante globale
    GLfloat globale_ambiante[] = {0.1, 0.1, 0.1, 1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globale_ambiante);

    // GL_LIGHT0 : source éloignée, blanche
    GLfloat amb_0[]  = {0.1, 0.1, 0.1, 1.0};
    GLfloat dif_0[]  = {0.8, 0.8, 0.8, 1.0};
    GLfloat spec_0[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT,  amb_0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  dif_0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec_0);
    glEnable(GL_LIGHT0);

    // GL_LIGHT1 : projecteur, jaune
    GLfloat amb_1[]  = {0.0, 0.0, 0.0, 1.0};
    GLfloat dif_1[]  = {1.0, 1.0, 0.0, 1.0};
    GLfloat spec_1[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT1, GL_AMBIENT,  amb_1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  dif_1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, spec_1);
    // ouverture du cône de 30°
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
    // atténuation de l'éclairage au bord du cône
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 4.0);
    // atténuation avec la distance : 1 / (a + bd + cd²)
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.05);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0);
    glEnable(GL_LIGHT1);
}

// Positions données après gluLookAt : les lumières restent fixes dans la scène
void Eclairage::placer() const
{
    // w = 0 : direction, source à distance infinie
    GLfloat direction_source0[] = {1.0, 2.0, 3.0, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, direction_source0);

    // w = 1 : position, projecteur au-dessus de l'araignée
    GLfloat position_source1[] = {0.0, 4.0, 0.0, 1.0};
    GLfloat dir[] = {0.0, -1.0, 0.0};
    glLightfv(GL_LIGHT1, GL_POSITION, position_source1);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir);
}

void Eclairage::basculer()
{
    actif = !actif;
    if (actif) glEnable(GL_LIGHTING);
    else       glDisable(GL_LIGHTING);
}

void Eclairage::basculerEloignee()
{
    eloigneeOn = !eloigneeOn;
    if (eloigneeOn) glEnable(GL_LIGHT0);
    else            glDisable(GL_LIGHT0);
}

void Eclairage::basculerProjecteur()
{
    projecteurOn = !projecteurOn;
    if (projecteurOn) glEnable(GL_LIGHT1);
    else              glDisable(GL_LIGHT1);
}

void Eclairage::materiau(GLfloat r, GLfloat v, GLfloat b, GLfloat brillance)
{
    GLfloat couleur[]    = {r, v, b, 1.0};
    GLfloat speculaire[] = {0.5, 0.5, 0.5, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   couleur);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   couleur);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  speculaire);
    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, brillance);
    glColor3f(r, v, b);   // couleur utilisée quand l'éclairage est coupé
}
