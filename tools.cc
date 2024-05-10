//OUSMANE EL HAMID / LILIAN LAPORTE / tools.cc 
//Architecture b1 
#include "tools.h"
#include "graphic.h"


bool inter_cercle (const cercle& C, const cercle& C2, double distance_ecart) 
{
	if (C.rayon + C2.rayon + distance_ecart >= sqrt((pow(C.centre.x-C2.centre.x, 2))+
		pow(C.centre.y-C2.centre.y, 2))) 
	{
		return true;
	}
	return false;
}


double produit_scalaire (const vecteur& v ,const vecteur& w) 
{
	return ((v.translation.x*w.translation.x) + (v.translation.y*w.translation.y));
}

double distance_orthogonale(const vecteur& v, const vecteur& w) 
{
	vecteur v_proj ;
	vecteur v_ortho;
	double dist_ortho;
	v_proj.translation.x = (produit_scalaire(v,w) / produit_scalaire(w, w) ) 
							* w.translation.x;
	v_proj.translation.y =(produit_scalaire(v,w) / produit_scalaire(w, w) ) 
						   * w.translation.y;
	
	v_ortho.translation.x = v.translation.x - v_proj.translation.x;
	v_ortho.translation.y = v.translation.y - v_proj.translation.y;
	dist_ortho = sqrt(pow(v_ortho.translation.x,2) + pow(v_ortho.translation.y,2));
	return dist_ortho;
}
vecteur neg_vect (vecteur v)
{
	v.translation.x = -v.translation.x;
	v.translation.y = -v.translation.y;
	return v;
}

bool inter_cercle_seg (const cercle& C, const segment& A_B, double distance_ecart)
{
	vecteur liaison;              //liaison et liaison_neg permettent de lier les 
	vecteur liaison_neg;		  //2 extremites du segment au cercle 	
	
	liaison.translation.x = C.centre.x - A_B.origine.x;
	liaison.translation.y = C.centre.y - A_B.origine.y;
	liaison_neg.translation.x = C.centre.x - A_B.origine.x - A_B.v.translation.x;
	liaison_neg.translation.y = C.centre.y - A_B.origine.y - A_B.v.translation.y;

	if ((distance_orthogonale(liaison, A_B.v) <= C.rayon + distance_ecart) and
	(produit_scalaire(liaison,A_B.v) > 0) and 
	(produit_scalaire(neg_vect(A_B.v) , liaison_neg)>0)) 
	{
		return true;
	}
	return false;
}

double distance(const points& A, const points& B)
{
	return sqrt(pow(B.x - A.x,2) + pow(B.y - A.y,2));
}

double longueur_vect(const vecteur& v1)
{
	return sqrt(pow(v1.translation.x,2) + pow(v1.translation.y,2));
}

void draw_logement(const cercle& C, Color couleur)
{
	
	double centre_x = C.centre.x;
	double centre_y = C.centre.y;
	double rayon = C.rayon;
	setColor(couleur);
	graphic_draw_cercle(centre_x, centre_y, rayon);
}

void draw_transport(const cercle& C, Color couleur)
{
	double centre_x = C.centre.x;
	double centre_y = C.centre.y;
	double rayon = C.rayon;
	double x_diagonale_gauche = centre_x-rayon*(sqrt(2)/2);
	double x_diagonale_droite = centre_x+rayon*(sqrt(2)/2);
	double y_diagonale_bas = centre_y-rayon*(sqrt(2)/2);
	double y_diagonale_haut = centre_y+rayon*(sqrt(2)/2);
	
	setColor(couleur);

	graphic_draw_cercle(centre_x, centre_y, rayon);
	graphic_draw_droite(centre_x-rayon,centre_y,centre_x+rayon,centre_y);
	graphic_draw_droite(centre_x,centre_y+rayon,centre_x,centre_y-rayon);
	graphic_draw_droite(x_diagonale_gauche,y_diagonale_bas,x_diagonale_droite,
						y_diagonale_haut);
	graphic_draw_droite(x_diagonale_gauche,y_diagonale_haut,x_diagonale_droite,
						y_diagonale_bas);	
}

void draw_production(const cercle& C, Color couleur)
{
	double centre_x = C.centre.x;
	double centre_y = C.centre.y;
	double rayon = C.rayon;
	double demi_larg_rect = rayon*0.75;
	double demi_haut_rect = rayon*(1/8.);
	
	setColor(couleur);
	
	graphic_draw_cercle(centre_x, centre_y, rayon);
	graphic_draw_droite(centre_x-demi_larg_rect,centre_y-demi_haut_rect,
						centre_x+demi_larg_rect,centre_y-demi_haut_rect);
	graphic_draw_droite(centre_x-demi_larg_rect,centre_y+demi_haut_rect,
						centre_x+demi_larg_rect,centre_y+demi_haut_rect);
	graphic_draw_droite(centre_x-demi_larg_rect,centre_y-demi_haut_rect,
						centre_x-demi_larg_rect,centre_y+demi_haut_rect);
	graphic_draw_droite(centre_x+demi_larg_rect,centre_y-demi_haut_rect,
						centre_x+demi_larg_rect,centre_y+demi_haut_rect);
}

void draw_link(const cercle& C1, const cercle& C2, Color couleur)
{
	double centre_x1 = C1.centre.x;
	double centre_y1= C1.centre.y;
	double centre_x2 = C2.centre.x;
	double centre_y2 = C2.centre.y;

	setColor(couleur);

	graphic_draw_droite(centre_x1,centre_y1,centre_x2,centre_y2);
}

