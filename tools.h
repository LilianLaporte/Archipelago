//OUSMANE EL HAMID / LILIAN LAPORTE / tools.h  
//Architecture b1
#include <cmath>
#ifndef TOOLS_HEADER_H
#define TOOLS_HEADER_H

struct points 
{
	double x;
	double y;
};

struct cercle 
{
	double  rayon;
	points centre;
};

struct vecteur 
{ 
	points translation;
};

struct segment 
{
	points origine;
	vecteur v;
};

double length (const vecteur ab);
double distance_orthogonale(const vecteur& v, const vecteur& w);
vecteur liaison_cercle (const cercle& C, const cercle C2);
bool inter_cercle_seg (const cercle& C, const segment& A_B, double distance_ecart);
bool inter_cercle (const cercle& C, const cercle& C2, double distance_ecart);
double distance(const points& A, const points& B);
double longueur_vect(const vecteur& v1);
void draw_logement(const cercle& C);
void draw_transport(const cercle& C);
void draw_production(const cercle& C);
void draw_link(const cercle& C1, const cercle& C2);

#endif
