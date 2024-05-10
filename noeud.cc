//noeud.cc 
//Laporte Lilian
//El Hamid Ousmane
//Architecture b1
#include "noeud.h"

using namespace std;

Noeud::Noeud(unsigned int uid, double x, double y , double capacite) 
{
	numero = uid;
	quartier.centre.x = x;
	quartier.centre.y = y;
	nbp = capacite;
	quartier.rayon = sqrt(nbp);
}

Noeud::~Noeud()
{
	link.clear();
}

unsigned int Noeud::get_uid() const
{
	return numero;
}

double Noeud::get_nbp() const
{ 
	return nbp;
}

vector<Noeud*>* Noeud::get_link_pointeur() 
{
	return &link;
}

vector<Noeud*> Noeud::get_link() const
{
	return link;
}

cercle Noeud::get_quartier() const
{
	return quartier;
}

points Noeud::get_centre() const
{
	return quartier.centre;
}

void Noeud::set_centre(double x,double y)
{
	quartier.centre.x = x;
	quartier.centre.y = y;
}

double Noeud::get_rayon() const
{
	return quartier.rayon;
}

void Noeud::set_nbp(double new_nbp)
{
	nbp = new_nbp;
}

void Noeud::set_rayon(double new_rayon)
{
	quartier.rayon = new_rayon;
}

bool Noeud::get_in() const
{
	return in;
}

void Noeud::set_in(bool new_in)
{
	in = new_in;
}

double Noeud::get_access() const
{
	return access;
}

void Noeud::set_access(double new_access)
{
	access = new_access;
}

unsigned int Noeud::get_parent() const
{
	return parent;
}

void Noeud::set_parent(unsigned int new_parent)
{
	parent = new_parent;
}

bool Noeud::test_reserved_uid()
{
	if(numero == no_link) return true;
	return false;
}

bool Noeud::test_max_link() 
{
	if (link.size() > max_link)	return true; 
	return false;
}

bool Noeud::test_max_capacity() 
{
	if(nbp > max_capacity) return true; 
	return false;
}

bool Noeud::test_min_capacity() 
{
	if (nbp < min_capacity) return true;
	return false;
}

string Noeud::test_noeud()
{
	if(test_reserved_uid())
	{
		return error::reserved_uid() ;
	}
	
	if(test_max_capacity())
	{
		return error::too_much_capacity(nbp);
	}
	
	if(test_min_capacity())
	{
		return error::too_little_capacity(nbp);
	}
	
	return "";
}

void Noeud::affichage_noeud(ofstream& fichier)
{
	fichier << "	" << numero << " " << quartier.centre.x << " " << quartier.centre.y 
			<< " " << nbp << endl;
}

void Noeud::draw_link_node(Color couleur)
{
	for(auto lien : link)
	{
		draw_link(quartier,lien->get_quartier(),couleur);
	}
}

Noeud_logement::Noeud_logement(unsigned int uid, double x, double y , double capacite): 
Noeud(uid,x,y,capacite)
{
}

Noeud_logement::~Noeud_logement()
{
}

NodeType Noeud_logement::get_type() const
{
	return NodeType::LOGEMENT;
}

void Noeud_logement::draw_node(Color couleur)
{
	draw_logement(quartier, couleur);
}

Noeud_transport::Noeud_transport(unsigned int uid, double x, double y , 
								 double capacite) : Noeud(uid,x,y,capacite)
{
}

Noeud_transport::~Noeud_transport()
{
}

NodeType Noeud_transport::get_type() const
{
	return TRANSPORT;
}

void Noeud_transport::draw_node(Color couleur)
{
	draw_transport(quartier, couleur);
}

Noeud_production::Noeud_production(unsigned int uid, double x, double y , 
								   double capacite) : Noeud(uid,x,y,capacite)
{
}

Noeud_production::~Noeud_production()
{
}

NodeType Noeud_production::get_type() const
{
	return PRODUCTION;
}

void Noeud_production::draw_node(Color couleur)
{
	draw_production(quartier, couleur);
}

string test_superposition(const vector<Noeud*> city, double distance_ecart)
{
	string erreur("");
	
	for(size_t i(0); i<city.size(); ++i)
	{
		erreur = test_inter_cercle(city,city[i],i,distance_ecart);	
		if(not erreur.empty())
			return erreur;
			
		erreur = test_inter_cercle_seg(city,city[i],distance_ecart);
		if(not erreur.empty())
			return erreur;
	}

	return erreur;
}

string test_multiple_same_link(const vector<unsigned int> tab_lien, 
							 const unsigned int noeud_1,const unsigned int noeud_2)
{
	for(size_t i(0); i<tab_lien.size()-2; i+=2)
	{
		if((tab_lien[i] == noeud_1 and tab_lien[i+1] == noeud_2) or 
		   (tab_lien[i] == noeud_2 and tab_lien[i+1] == noeud_1))
		{
			return error::multiple_same_link(noeud_1, noeud_2);
		}
	}
	
	return "";
}
string test_link_vacuum(const vector<Noeud*> city,const unsigned int noeud_1,
					    const unsigned int noeud_2)
{
	bool existence_lien_1(false),existence_lien_2(false);
	
	for(size_t i(0); i<city.size();i+=1)
	{
		if (noeud_1 == city[i]->get_uid())
		{
			existence_lien_1 = true;
		}
		if (noeud_2 == city[i]->get_uid())
		{
			existence_lien_2 = true;
		}
	}
	
	if(!existence_lien_1)
	{
		return error::link_vacuum(noeud_1);
	}
	if(!existence_lien_2)
	{
		return error::link_vacuum(noeud_2);
	}
	
	return "";
}

string boucle_max_link(const vector<unsigned int> tab_lien, const vector<Noeud*> city,
					   int nb_node_l)
{
	string erreur("");
	
	for (int i(0); i<nb_node_l; ++i)
	{
		erreur = test_one_node_max_link(city[i]);
		if(not erreur.empty())
			return erreur;
	}  
	
	return ""; 
}

string test_one_node_max_link(Noeud* noeud)
{
	if(noeud->test_max_link()) 
	{
		return error::max_link(noeud->get_uid());
	}
	return "";
}

string test_identical_uid(const double uid, const vector<Noeud*> city)
{
	for(size_t i(0); i<city.size();i+=1)
	{
		if (uid == city[i]->get_uid())
		{
			return error::identical_uid(uid);
		}
	}
	return "";
}
string test_self_link_node (const unsigned int& noeud_1, const unsigned int& noeud_2)
{
	if (noeud_1 == noeud_2) 
	{
		return error::self_link_node(noeud_1);
	}
	return "";
}

string test_inter_cercle(const vector<Noeud*> city,Noeud* ville_noeud,size_t indice,
					   double distance_ecart)
{	
	for(size_t j=indice+1; j<city.size(); j++)
	{
		if(inter_cercle(ville_noeud->get_quartier(),city[j]->get_quartier(),
		   distance_ecart))
		{
			return error::node_node_superposition(ville_noeud->get_uid(),
												  city[j]->get_uid());								  
		}
	}
	
	return "";
}

string test_inter_cercle_seg(const vector<Noeud*> city,Noeud* ville_noeud, 
						   double distance_ecart)
{
	for(size_t k(0); k<ville_noeud->get_link().size(); ++k)
	{
		segment lien;			 //initialisation du lien entre un noeud i et 
								//le noeud k auquel il est lié
		lien.origine = ville_noeud->get_quartier().centre;
		lien.v.translation.x = (ville_noeud->get_link()[k])->get_quartier().centre.x 
							   - ville_noeud->get_quartier().centre.x;
								
		lien.v.translation.y = (ville_noeud->get_link()[k])->get_quartier().centre.y 
								- ville_noeud->get_quartier().centre.y;
			
		for(size_t l(0); l<city.size(); ++l)
		{		
			if ((city[l]->get_uid() != ville_noeud->get_uid()) and 
				(city[l]->get_uid() != (ville_noeud->get_link()[k])->get_uid()) and
				(inter_cercle_seg(city[l]->get_quartier(),lien,distance_ecart)))
			{
				return error::node_link_superposition(city[l]->get_uid());
			}
		}
	}
	
	return "";
}


