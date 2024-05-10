//OUSMANE EL HAMID / LILIAN LAPORTE / projet.cc  
//Architecture b1
#include "gui.h"

using namespace std;

int main(int argc, char * argv[])
{
	int fake_argc = 1;
	string erreur("");
	
	auto app = Gtk::Application::create(fake_argc, argv);
   
    Gui window;
    
	if(argc == 2)
	{
		string nomfichier = argv[1];
		erreur = export_lausanne()->lecture(nomfichier);
		window.affichage_erreur(erreur);
	}
	
	window.set_label();
	
	return app->run(window);
}


