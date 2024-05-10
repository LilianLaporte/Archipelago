//ville.cc 
//Laporte Lilian
//El Hamid Ousmane
//Architecture b1
#include "ville.h"

using namespace std ;

namespace
{
	Ville lausanne = Ville(); 
	
	double calcul_temps_trajet(Noeud* noeud_1, Noeud* noeud_2);	
	string erreur_link(unsigned int noeud_1, unsigned int noeud_2, 
					   const vector<unsigned int> tab_lien, const vector<Noeud*> city);
	string erreur_finale(const vector<unsigned int> tab_lien,const vector<Noeud*> city,
						 int nb_node_l, double distance_ecart);
}


Ville::Ville()   				
{
	city=vector<Noeud*>();
}

Ville::~Ville()                  
{
}

vector<Noeud*> Ville::get_city() const
{ 
	return city;
}

double Ville::get_enj() const
{
	return enj;
}

void Ville::set_enj(double new_enj) 
{
	enj = new_enj;
}

double Ville::get_ci() const
{
	return ci;
}

void Ville::set_ci(double new_ci)
{
	ci = new_ci;
}

double Ville::get_mta() const
{
	return mta;
}

void Ville::set_mta(double new_mta)
{
	mta = new_mta;
}

string Ville::lecture(string nomfichier)
{
	string line;
	ifstream fichier(nomfichier);
	string erreur("");

	while(getline(fichier >> ws,line)) 	
	{		
		if((line.rfind('#') == 0) or (line.size() >= max_line))  
		{
			continue;
		}  
		erreur = decodage_ligne(line,city);
		if(not erreur.empty())
			break;
	}
	fichier.close();
	
	if(not erreur.empty()) 
	{
		supp_all();
	}
	
	return erreur;
}
	
string Ville::decodage_ligne(string line,vector<Noeud*>& city)
{
	istringstream data(line);
	string ligne;						 
	enum Etat_lecture{NODE_L,NODE_T,NODE_P,LINK,NBNODE_L,NBNODE_T,NBNODE_P,NBLINK,FIN};
	static int etat(NBNODE_L),total(0),compteur(0),nb_node_l(0);
	static vector<unsigned int> tab_lien; 	
	double x(0),y(0),nbp(0),distance_ecart(0);   //distance_ecart=0 car on ne prend pas 
	unsigned int uid(0),noeud_1(0),noeud_2(0);   //en compte dist_min
	string erreur("");
	while(erreur.empty())
	{
		switch(etat) 
		{
			case NBNODE_L:
				data >> total;
				compteur=0;
				nb_node_l= total;
				if (total == 0) etat = NBNODE_T; else etat=NODE_L;
				break;			
			case NODE_L: 
				data >> uid >> x >> y >> nbp;
				++compteur;		
				if (compteur == total) etat = NBNODE_T;		
				erreur = add_node_l(uid,nbp,x,y);
				break;			
			case NBNODE_T:
				data >> total;
				compteur=0;
				if (total == 0) etat = NBNODE_P; else etat = NODE_T;
				break;			
			case NODE_T: 
				data  >> uid >> x >> y >> nbp;
				++compteur;
				if(compteur == total) etat = NBNODE_P;	
				erreur = add_node_t(uid,nbp,x,y);
				break;	
			case NBNODE_P:
				data >> total;
				compteur=0;
				if(total == 0) etat = NBLINK; else etat = NODE_P;
				break;		    
			case NODE_P:
				data  >> uid >> x >> y >> nbp;
				++compteur;
				if(compteur == total) etat = NBLINK;		
				erreur = add_node_p(uid,nbp,x,y);
				break;			
			case NBLINK:
				data >> total;
				compteur=0;
				if (total == 0) etat = FIN; else etat = LINK;
				break;	
			case LINK:
				data >> noeud_1 >> noeud_2;
				++compteur;
				if (compteur == total) etat = FIN;
				tab_lien.push_back(noeud_1);
				tab_lien.push_back(noeud_2);		
				erreur = erreur_link(noeud_1, noeud_2, tab_lien, city);		
				creation_lien(noeud_1,noeud_2);
				break;
		}	
		if (etat == FIN)
		{	
			erreur = erreur_finale(tab_lien, city, nb_node_l, distance_ecart);			
			etat = NBNODE_L;
			tab_lien.clear();
			break;		
		}
		break;
	}
	if(not erreur.empty()) 
	{
		cout << "tab clear"<<endl;
		tab_lien.clear();
		etat = NBNODE_L;
	}
	return erreur;
}

bool Ville::existence_lien(Noeud* noeud1, Noeud* noeud2)
{
	for(auto noeud : noeud1->get_link())
	{
		if(noeud->get_uid() == noeud2->get_uid())
			return true;
	}
	
	return false;
}

std::string Ville::creation_lien_interactif(Noeud* noeud_1, Noeud* noeud_2)
{
	string erreur("");
	
	add_link(noeud_1, noeud_2);
	
	if(noeud_1->get_type() == LOGEMENT) 
		erreur = test_one_node_max_link(noeud_1);
	if(not erreur.empty())
	{
		supp_link(noeud_1, noeud_2);
		return erreur;
	}
		
	if(noeud_2->get_type() == LOGEMENT) 
		erreur = test_one_node_max_link(noeud_2);
	if(not erreur.empty())
	{
		supp_link(noeud_1, noeud_2);
		return erreur;
	}
	
	erreur = test_superposition(city, dist_min);
	if(not erreur.empty())
		supp_link(noeud_1, noeud_2);
	
	return erreur;
}

void Ville::add_link(Noeud* noeud_1, Noeud* noeud_2)
{
	(*(noeud_1->get_link_pointeur())).push_back(noeud_2);
	(*(noeud_2->get_link_pointeur())).push_back(noeud_1);
}

void Ville::supp_all_link_node(Noeud* noeud)
{
	for(auto noeud_lien : noeud->get_link())
	{
		supp_link(noeud, noeud_lien);
	}
}

void Ville::supp_link(Noeud* noeud1, Noeud* noeud2)
{
	for(size_t i(0); i<noeud1->get_link().size(); ++i)
	{
		if(noeud1->get_link()[i]->get_uid() == noeud2->get_uid())
		{
			swap((*(noeud1->get_link_pointeur()))[i],
				 (*(noeud1->get_link_pointeur())).back());
			
			(*(noeud1->get_link_pointeur())).pop_back();
		}
	}
	
	for(size_t j(0); j<noeud2->get_link().size(); ++j)
	{
		if(noeud2->get_link()[j]->get_uid() == noeud1->get_uid())
		{
			swap((*(noeud2->get_link_pointeur()))[j],
				 (*(noeud2->get_link_pointeur())).back());
			
			(*(noeud2->get_link_pointeur())).pop_back();
		}
	}
}

string Ville::creation_noeud(NodeType type, double x, double y)
{
	unsigned int uid(1);
	double nbp(1000);
	
	choix_uid(uid);
	
	if(uid == no_link) 
		return error::too_much_node();
		
	string erreur("");
	
	switch(type)
	{
	  case LOGEMENT :
		erreur = add_node_l(uid,nbp,x,y);
		break;
	  case TRANSPORT :
		erreur = add_node_t(uid,nbp,x,y);
		break;
	  case PRODUCTION :
		erreur = add_node_p(uid,nbp,x,y);
		break;
	  default :
		break;
	}
	if(not erreur.empty())
	{
		supp_noeud(city.size()-1);
		return erreur;
	}
	
	erreur = test_superposition(city, dist_min);    
	if(not erreur.empty())
		supp_noeud(city.size()-1);

	return erreur;
}

void Ville::choix_uid(unsigned int &uid)
{
	bool sortie(false);

	while(!sortie)
	{
		if(city.empty())
			sortie = true;
			
		for(size_t i(0); i<city.size(); ++i)
		{
			if(city[i]->get_uid() == uid) 
			{
				++uid;
				break;
			}
			if(i == city.size()-1)
				sortie = true;
		}
	}
}

string Ville::add_node_l(unsigned int uid,double nbp,double x,double y)
{
	string erreur("");
	
	erreur = test_identical_uid(uid,city);
	if(not erreur.empty())
		return erreur;

	Noeud* N = new Noeud_logement(uid,x,y,nbp);
	city.push_back(N);
	
	erreur = N->test_noeud();
	if(not erreur.empty())
		return erreur;

	return erreur;
}

string Ville::add_node_t(unsigned int uid,double nbp,double x,double y)
{
	string erreur("");

	erreur = test_identical_uid(uid,city);
	if(not erreur.empty())
		return erreur;
	
	Noeud* N = new Noeud_transport(uid,x,y,nbp);
	city.push_back(N);
	
	erreur = N->test_noeud();
	if(not erreur.empty())
		return erreur;
		
	return erreur;
}

string Ville::add_node_p(unsigned int uid,double nbp,double x,double y)
{
	string erreur("");
	
	erreur =test_identical_uid(uid,city);
	if(not erreur.empty())
		return erreur;
		
	Noeud* N = new Noeud_production(uid,x,y,nbp);
	city.push_back(N);
	
	erreur = N->test_noeud();
	if(not erreur.empty())
		return erreur;
		
	return erreur;
}

void Ville::creation_lien(unsigned int noeud_1, unsigned int noeud_2)
{
	for (size_t j(0);j<city.size();++j)
	{
		for(size_t k(0);k<city.size();++k)
		{
			if((city[j]->get_uid() == noeud_1) and (city[k]->get_uid() == noeud_2))
			{
				add_link(city[j],city[k]);
			}
		}
	}
}

void Ville::delete_noeud_courant(Noeud* noeud_courant)
{
	supp_all_link_node(noeud_courant);
	
	for(size_t i(0); i<city.size(); ++i)
	{
		if(noeud_courant->get_uid() == city[i]->get_uid())
			supp_noeud(i);
	}
	noeud_courant = nullptr;
}

void Ville::supp_noeud(size_t indice)
{
	delete city[indice];		
	swap(city[indice],city.back());		
	city.pop_back();	
}
	
void Ville::supp_all()
{
	for(size_t i(0); i<city.size(); ++i)
	{
		supp_noeud(i);
	}
	city.clear();
}

void Ville::critere_enj()
{
	double nbp_l(0), nbp_t(0), nbp_p(0),enj(0);

	for(auto noeud : city)
	{
		if(noeud->get_type() == NodeType::LOGEMENT) nbp_l += noeud->get_nbp();
		
		if(noeud->get_type() == NodeType::TRANSPORT) nbp_t += noeud->get_nbp();
		
		if(noeud->get_type() == NodeType::PRODUCTION) nbp_p += noeud->get_nbp();
	}
	
	if((nbp_l == 0) and (nbp_t == 0) and (nbp_p == 0))
		enj = 0;
	else
		enj=(nbp_l-nbp_t-nbp_p)/(nbp_l+nbp_t+nbp_p);
	
	set_enj(enj);
}

void Ville::critere_ci()
{
	double dist(0),capacite(0),vitesse(0),ci(0);

	for(size_t i(0); i<city.size(); ++i)
	{		
		for(size_t j(0); j<city[i]->get_link().size(); ++j)
		{			
			dist=distance(city[i]->get_centre(), city[i]->get_link()[j]->get_centre());
			
			if(city[i]->get_nbp() < city[i]->get_link()[j]->get_nbp())
				capacite = city[i]->get_nbp();
			else
				capacite = city[i]->get_link()[j]->get_nbp();

			if((city[i]->get_type() == NodeType::TRANSPORT) and 
			   (city[i]->get_link()[j]->get_type() == NodeType::TRANSPORT))
				vitesse = fast_speed;
			else
				vitesse = default_speed;
			
			ci += dist*capacite*vitesse;	
		}	
	}
	
	set_ci(ci/2);
}

void Ville::critere_mta()
{
	int nb_logement(0);
	double mta(0);

	for(auto noeud : city)
	{
		if(noeud->get_type() == NodeType::LOGEMENT) ++ nb_logement;
	}
	
	if(nb_logement == 0)
		mta = 0;
	else	
		mta = (mta_transport()+mta_production())/nb_logement;

	set_mta(mta);
}

double Ville::mta_transport()
{
	unsigned int indice_transport(0);
	double temps_total(0);

	for(size_t i(0); i<city.size();++i)
	{
		if(city[i]->get_type() == NodeType::LOGEMENT)
		{			
			vector<unsigned int> ta(city.size());
			initialisation_mta(ta,i);
			set_production_false();
			
			indice_transport = find_transport(ta);

			if(indice_transport == no_link)
				temps_total += infinite_time;
			else
				temps_total += city[indice_transport]->get_access();
		}
	}
	return temps_total;
}

double Ville::mta_production()
{
	unsigned int indice_production(0);
	double temps_total(0);
	
	for(size_t i(0); i<city.size();++i)
	{
		if(city[i]->get_type() == NodeType::LOGEMENT)
		{			
			vector<unsigned int> ta(city.size());
			initialisation_mta(ta,i);
			
			indice_production = find_production(ta);
			if(indice_production == no_link)
				temps_total += infinite_time;
			else	
				temps_total += city[indice_production]->get_access();
		}
	}
	return temps_total;
}

void Ville::initialisation_mta(vector<unsigned int>& ta, size_t indice)
{
	for(auto noeud : city)
	{
		noeud->set_in(true);
		noeud->set_access(infinite_time);
		noeud->set_parent(no_link);
	}
	
	city[indice]->set_access(0);
	initialisation_ta(ta);
}

void Ville::tri_insertion(vector<unsigned int>& ta)
{
	int j(0),tmp(0);
	
	for(size_t i(1);i<ta.size();++i)
	{
		j=i;
		
		while (j>0 && city[ta[j-1]]->get_access()>city[ta[j]]->get_access())
		{
			tmp = ta[j];
			ta[j] = ta[j-1];
			ta[j-1] = tmp;
			
			--j;
		}
	}
}

void Ville::set_production_false()
{
	for(auto noeud : city)
	{
		if(noeud->get_type() == NodeType::PRODUCTION) noeud->set_in(false);
	}
}


unsigned int Ville::find_transport(vector<unsigned int>& ta)
{
	bool noeud_production(false);
	unsigned int indice(0);
	int indice_non_production(0);
	
	while(!city.empty())
	{		
		indice=find_min_access(ta,indice_non_production);
		
		if((indice == no_link) or (city[indice]-> get_type() == NodeType::TRANSPORT))
			return indice;
		
		if(city[indice]->get_type() == NodeType::PRODUCTION) 
			noeud_production = true;		

		dijkstra(ta,indice,noeud_production);
	}
	return no_link;
}

unsigned int Ville::find_production(vector<unsigned int>& ta)
{
	bool noeud_production(false);
	unsigned int indice(0);
	int indice_non_production(0);
	
	while(!city.empty())
	{
		indice = find_min_access(ta,indice_non_production);

		if((indice == no_link) or (city[indice]-> get_type() == NodeType::PRODUCTION))
			return indice;
		
		noeud_production = false;         		//automatiquement à false car si le 
		dijkstra(ta,indice,noeud_production);	//le noeud est de type transport 
	}											//l'indice est forcément retourner
	return no_link;
}

void Ville::dijkstra(vector<unsigned int>& ta,unsigned int indice, 
					 bool noeud_production)
{	
	double temps(0);
	city[indice]->set_in(false);

	if(!noeud_production)
	{
		for(auto lien : city[indice]->get_link())
		{
		   if(lien->get_in() == true)
		   {
			   temps=city[indice]->get_access()+calcul_temps_trajet(lien,city[indice]);

		       if(lien->get_access() > temps)
		       {
				   lien->set_access(temps);
				   lien->set_parent(indice);
				   tri_insertion(ta);
			   }
		   }
		}
	}
}

unsigned int Ville::find_min_access(const vector<unsigned int>& ta, 
									int indice_non_production)
{
	for(size_t i(0); i<ta.size(); ++i)
	{	
		if(city[ta[i]]->get_in() == true) 
		{

			return ta[i];
		}		
	}
	return no_link;	
}

void Ville::initialisation_ta(vector<unsigned int>& ta)
{
	for(size_t j(0);j<ta.size(); ++j)
	{
		ta[j]=j;
	}
	tri_insertion(ta);
}

void Ville::enregistrement_etat(string filename)
{	
	ofstream fichier;
	
	fichier.open(filename);
	affichage_ville(fichier);
	fichier.close();
}

void Ville::affichage_ville(ofstream& fichier)
{
	fichier << "# file generated by Lilian and Ousmane" << endl << endl;

	string type_node;
	int type(LOGEMENT);

	type_node = "logement";
	affichage_type_noeud(fichier,type_node,type);
	
	type = TRANSPORT;
	type_node = "transport";
	affichage_type_noeud(fichier,type_node,type);
	
	type = PRODUCTION;
	type_node = "production";
	affichage_type_noeud(fichier,type_node,type);
	
	affichage_lien(fichier);
	
}

void Ville::affichage_type_noeud(ofstream& fichier, string type_node, int type)
{
	fichier << "# nb " << type_node << endl;
	fichier << nombre_type_noeud(type) << endl;
	fichier << "# " << type_node << " :" << endl;
	for(auto noeud : city)
	{
		if(noeud->get_type() == type) noeud->affichage_noeud(fichier);
	}
	fichier << endl;
}

void Ville::affichage_lien(ofstream& fichier)
{
	unsigned int nb_lien(0),indice(0);
	
	fichier << "# nb lien" << endl;
	for(auto noeud : city)
	{
		nb_lien += noeud->get_link().size();
	}
	fichier << nb_lien/2 << endl;
	
	if(city.size()>0)
	{		
		for(size_t i(0); i<city.size()-1; ++i)
		{
			for(size_t j(0); j<city[i]->get_link().size(); ++j)
			{
				for(size_t k(0); k<city.size(); ++k)
				{
					if(city[i]->get_link()[j]->get_uid() == city[k]->get_uid())
						indice = k;
				}
				if(indice>i)	//vérification pour ne pas afficher le même lien 2 fois
				{
					fichier << "	" << city[i]->get_uid() << " " 
							<< city[i]->get_link()[j]->get_uid() << endl;
				}
			}
		}
	}
}

int Ville::nombre_type_noeud(int type)
{	
	int compteur(0);
	
	for(auto noeud : city)
	{
		if (noeud->get_type() == type) ++compteur;
	}
	
	return compteur;
}

void Ville::draw_all()
{
	for(auto noeud : city)
	{
		noeud->draw_link_node(BLACK);
	}
	
	for(auto noeud : city)
	{
		noeud->draw_node(BLACK);
	}
}

void Ville::draw_active_node(Noeud* noeud_courant)
{
	for(auto noeud : city)
	{
		noeud->draw_link_node(BLACK);
	}
	
	for(auto noeud : city)
	{
		if(noeud->get_uid() == noeud_courant->get_uid())
			noeud->draw_node(RED);
		else	
			noeud->draw_node(BLACK);
	}
}

Noeud* Ville::noeud_selectionne(double clic_x, double clic_y)
{
	points clic={clic_x, clic_y};
	
	for(auto noeud : city)
	{
		if(distance(clic, noeud->get_centre()) <= noeud->get_rayon())
			return noeud;
	}
	
	return nullptr;
}

unsigned int Ville::find_indice(Noeud* noeud)
{	
	for(size_t i(0); i<city.size(); ++i)
	{
		if(noeud->get_uid() == city[i]->get_uid())
			return i;
	}
	return no_link;
}

string Ville::changement_rayon(double rayon_init, double rayon_fin, Noeud* noeud)
{
	double previous_rayon = noeud->get_rayon();
	double previous_nbp = noeud->get_nbp();
	double new_nbp = pow((previous_rayon+(rayon_fin-rayon_init)),2);
	double new_rayon = sqrt(new_nbp);
	
	noeud->set_nbp(new_nbp);
	noeud->set_rayon(new_rayon);
	
	string erreur = test_superposition(city, dist_min);	
	if(not erreur.empty())
	{
		noeud->set_nbp(previous_nbp);
		noeud->set_rayon(previous_rayon);
		return erreur;
	}
	
	erreur = noeud->test_noeud();
	if(not erreur.empty())
	{
		noeud->set_nbp(previous_nbp);
		noeud->set_rayon(previous_rayon);
		return erreur;
	}
	
	return erreur;
}

string Ville::changement_place(Noeud* noeud, double x, double y)
{
	double init_x = noeud->get_centre().x;
	double init_y = noeud->get_centre().y;
	
	noeud->set_centre(x,y);
	
	string erreur = test_superposition(city, dist_min);
	
	if(not erreur.empty())
		noeud->set_centre(init_x, init_y);
		
	return erreur;
}

void Ville::shortest_path_transport(unsigned int indice)
{
    vector<unsigned int> ta(city.size());
	initialisation_mta(ta,indice);
	set_production_false();
	
	unsigned int indice_transport = find_transport(ta);

	if((indice_transport != no_link) and 
	   (city[indice_transport]->get_access() < infinite_time))
	{
		show_all_shortest_path(indice, indice_transport);
	}
}
  
void Ville::shortest_path_production(unsigned int indice)
{
	vector<unsigned int> ta(city.size());
	initialisation_mta(ta,indice);
	
	unsigned int indice_production = find_production(ta);

	if((indice_production != no_link) and 
	   (city[indice_production]->get_access() < infinite_time))
	{
		show_all_shortest_path(indice, indice_production);
	}
}

void Ville::show_all_shortest_path(unsigned int indice_depart, 
								   unsigned int indice_special)
{
	unsigned int indice_parent(indice_special), previous_indice(no_link), 
				 indice_temp(no_link);
	
	while(indice_parent != no_link)
	{
		show_way(indice_parent,indice_temp,previous_indice);
	}
	
	export_lausanne()->get_city()[indice_depart]->draw_node(RED);
	draw_link(city[indice_depart]->get_quartier(), city[indice_temp]->get_quartier(), 
			  GREEN);
	draw_link(city[indice_temp]->get_quartier(), city[indice_depart]->get_quartier(), 
			  GREEN);
	city[indice_special]->draw_node(GREEN);
}

void Ville::show_way(unsigned int& indice_parent, unsigned int& indice_temp, 
					 unsigned int& previous_indice)
{
	indice_temp = indice_parent;
	indice_parent = city[indice_temp]->get_parent();

	if((indice_temp != no_link) and (indice_parent != no_link))		
	{
		draw_link(city[indice_parent]->get_quartier(), 
				  city[indice_temp]->get_quartier(), GREEN);
		draw_link(city[indice_temp]->get_quartier(), 
				  city[indice_parent]->get_quartier(), GREEN);
		
		if(previous_indice != no_link)
			city[previous_indice]->draw_node(GREEN);
	}
	previous_indice = indice_parent;
}

Ville* export_lausanne()    // étant donné qu'on exporte une instance de Ville
{                           // et non pas un attribut, l'ajout d'un const induit 
	return &lausanne;       // un disfonctionnement du programme.
}

namespace
{	
	double calcul_temps_trajet(Noeud* noeud_1, Noeud* noeud_2)
	{	
		double temps(0);
		
		if(noeud_1->get_type() == NodeType::TRANSPORT and 
		   noeud_2->get_type() == NodeType::TRANSPORT)
		   temps = distance(noeud_1->get_centre(),noeud_2->get_centre())/fast_speed;
		else
		   temps = distance(noeud_1->get_centre(),noeud_2->get_centre())/default_speed;
		
		return temps;
	}
	string erreur_link(unsigned int noeud_1, unsigned int noeud_2, 
					   const vector<unsigned int> tab_lien, const vector<Noeud*> city)
	{
		string erreur("");
		
		erreur = test_self_link_node(noeud_1,noeud_2);
		if(not erreur.empty())
			return erreur;
			
		erreur = test_multiple_same_link(tab_lien, noeud_1, noeud_2);
		if(not erreur.empty())
			return erreur;
			
		erreur = test_link_vacuum(city,noeud_1,noeud_2);
		if(not erreur.empty())
			return erreur;
			
		return erreur;
	}
	
	string erreur_finale(const vector<unsigned int> tab_lien,const vector<Noeud*> city,
						 int nb_node_l, double distance_ecart)
	{
		string erreur("");
		
		erreur = boucle_max_link(tab_lien,city,nb_node_l);
		if(not erreur.empty()) 
			return erreur;

		erreur = test_superposition(city,distance_ecart);
		if(not erreur.empty()) 
			return erreur;
			
		return erreur;
	}
}
