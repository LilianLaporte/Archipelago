//ville.h 
//Laporte Lilian
//El Hamid Ousmane
//Architecture b1
#ifndef VILLE_HEADER_H
#define VILLE_HEADER_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream> 
#include "noeud.h"
#include "error.h"
#include "tools.h"
#include "constantes.h"


class Ville
{
private :
	std::vector<Noeud*> city;
	double enj;
	double ci;
	double mta;

public :
	Ville();
	~Ville();
	std::vector<Noeud*> get_city() const;
	double get_enj() const;
	void set_enj(double new_enj); 
	double get_ci() const;
	void set_ci(double new_ci);
	double get_mta() const;
	void set_mta(double new_mta);
	std::string lecture(std::string nomfichier);
	std::string decodage_ligne(std::string line,std::vector<Noeud*>& city);
	bool existence_lien(Noeud* noeud1, Noeud* noeud2);
	std::string creation_lien_interactif(Noeud* noeud_1, Noeud* noeud_2);
	void add_link(Noeud* noeud_1, Noeud* noeud_2);
	void supp_all_link_node(Noeud* noeud);
	void supp_link(Noeud* noeud1, Noeud* noeud2);
	std::string creation_noeud(NodeType type, double x, double y);
	void choix_uid(unsigned int &uid);
	std::string add_node_l(unsigned int uid,double nbp,double x,double y);
	std::string add_node_t(unsigned int uid,double nbp,double x,double y);
	std::string add_node_p(unsigned int uid,double nbp,double x,double y);
	void creation_lien(unsigned int noeud_1, unsigned int noeud_2);
	void delete_noeud_courant(Noeud* noeud_courant);
	void supp_noeud(size_t indice);	
	void supp_all();
	void critere_enj();
	void critere_ci();
	void critere_mta();
	double mta_transport();
	double mta_production();
	void initialisation_mta(std::vector<unsigned int>& ta, size_t indice);
	void tri_insertion(std::vector<unsigned int>& ta);
	void set_production_false();	
	unsigned int find_transport(std::vector<unsigned int>& ta);
	unsigned int find_production(std::vector<unsigned int>& ta);
	void dijkstra(std::vector<unsigned int>& ta,unsigned int indice,
				  bool noeud_production);
	unsigned int find_min_access(const std::vector<unsigned int>& ta,
								 int indice_non_production);
	void initialisation_ta(std::vector<unsigned int>& ta);	
	void enregistrement_etat(std::string filename);
	void affichage_ville(std::ofstream& fichier);
	void affichage_type_noeud(std::ofstream& fichier, std::string type_node, int type);
	void affichage_lien(std::ofstream& fichier);	
	int nombre_type_noeud(int type);
	void draw_all();
	void draw_active_node(Noeud* noeud_courant);
	Noeud* noeud_selectionne(double clic_x, double clic_y);
	unsigned int find_indice(Noeud* noeud);
	std::string changement_rayon(double rayon_init, double rayon_fin, Noeud* noeud);
	std::string changement_place(Noeud* noeud, double x, double y);
	void shortest_path_transport(unsigned int indice);
	void shortest_path_production(unsigned int indice);
	void show_all_shortest_path(unsigned int indice_depart, 
								unsigned int indice_special);
	void show_way(unsigned int& indice_parent, unsigned int& indice_temp, 
				  unsigned int& previous_indice);
};

Ville* export_lausanne();




#endif
