//OUSMANE EL HAMID / LILIAN LAPORTE / projet.cc  
//Architecture b1
#include "gui.h"

using namespace std;

int main(int argc, char * argv[])
{
	int fake_argc = 1;
	
	auto app = Gtk::Application::create(fake_argc, argv, "org.gtkmm.example");
	    
    Gui window;
    window.set_default_size(800,800);
    window.set_resizable(false);
    
	if(argc == 2)
	{
		string nomfichier = argv[1];
		(*export_lausanne()).lecture(nomfichier);
	}
	
	window.set_label();
	
	return app->run(window);
}


