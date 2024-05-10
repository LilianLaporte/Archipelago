//noeud.h 
//Laporte Lilian
//El Hamid Ousmane
//Architecture b1
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include "tools.h"
#include "constantes.h"
#include "error.h"

#ifndef NOEUD_HEADER_H
#define NOEUD_HEADER_H

enum NodeType{LOGEMENT, TRANSPORT, PRODUCTION};

 
class Noeud 
{ 
protected :
	unsigned int numero;
	cercle quartier;
	double nbp;
	std::vector<Noeud*> link;
	bool in;
	double access;
	unsigned int parent;

public : 
	Noeud (unsigned int uid, double x, double y , double capacite);
	virtual ~Noeud();
	virtual NodeType get_type() const=0;
	unsigned int get_uid() const;
	double get_nbp() const;
	std::vector<Noeud*>* get_link_pointeur();
	std::vector<Noeud*> get_link() const;
	cercle get_quartier() const;
	points get_centre() const;
	void set_centre(double x,double y);
	double get_rayon() const;
	void set_nbp(double new_nbp);
	void set_rayon(double new_rayon);
	bool get_in() const;
	void set_in(bool new_in);
	double get_access() const;
	void set_access(double new_access);
	unsigned int get_parent() const;
	void set_parent(unsigned int new_parent);   
	bool test_reserved_uid();
	bool test_max_link();	
	bool test_max_capacity() ;
	bool test_min_capacity();		
	std::string test_noeud();	
	void affichage_noeud(std::ofstream& fichier);	
	virtual void draw_node(Color couleur)=0;
	void draw_link_node(Color couleur);
};

class Noeud_logement: public Noeud
{
private:

public:
	Noeud_logement(unsigned int uid, double x, double y , double capacite);
	~Noeud_logement();
	NodeType get_type() const;
	void draw_node(Color couleur);
};

class Noeud_transport: public Noeud
{
private:

public:
	Noeud_transport(unsigned int uid, double x, double y , double capacite);
	~Noeud_transport();
	NodeType get_type() const;
	void draw_node(Color couleur);
};

class Noeud_production: public Noeud
{
private:

public:
	Noeud_production(unsigned int uid, double x, double y , double capacite);
	~Noeud_production();
	NodeType get_type() const;	
	void draw_node(Color couleur);
};


std::string test_superposition(const std::vector<Noeud*> city,double distance_ecart);
std::string test_multiple_same_link(const std::vector<unsigned int> tab_lien, 
							 const unsigned int noeud_1, const unsigned int noeud_2);
							 
std::string test_link_vacuum(const std::vector<Noeud*> city,const unsigned int noeud_1,
					  const unsigned int noeud_2);
					  
std::string boucle_max_link(const std::vector<unsigned int> tab_lien,
							const std::vector<Noeud*> city, int nb_node_l);
std::string test_one_node_max_link(Noeud* noeud);							
std::string test_self_link_node (const unsigned int& noeud_1, 
								 const unsigned int& noeud_2);
std::string test_identical_uid(const double uid, const std::vector<Noeud*> city);	
std::string test_inter_cercle(const std::vector<Noeud*> city,Noeud* ville_noeud,
					   size_t indice, double distance_ecart);					   
std::string test_inter_cercle_seg(const std::vector<Noeud*> city,Noeud* ville_noeud,
						   double distance_ecart);
//~ void gestion_erreur(int type_erreur);

#endif

