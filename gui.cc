//OUSMANE EL HAMID / LILIAN LAPORTE / gui.cc
//Architecture b1
#include <iostream>
#include <cairomm/context.h>
#include <iomanip>
#include "graphic_gui.h"
#include "gui.h"

#define DROIT 3
#define GAUCHE 1

using namespace std;

namespace
{
	enum Etat_radiobutton{BU_HOUSING,BU_TRANSPORT,BU_PRODUCTION};
	static int etat_radio(BU_HOUSING);
	std::string critere_to_string(const double& critere);
	std::string zoom_to_string(const double& zoom);
	Noeud* noeud_courant(nullptr);
}
 
MyArea::MyArea(): empty(false), shortestPathOn(false), clic_init({0.,0.}), 
				  clic_final({0.,0.})
{
}

MyArea::~MyArea()
{
}

void MyArea::clear()
{
    empty = true; 
    refresh();
}

void MyArea::draw()
{
    empty = false;
    refresh();
}

void MyArea::init_rayon(points p)
{
	clic_init = p;
}

void MyArea::finalize_rayon(points p)
{
	clic_final = p;
}

string MyArea::condition_new_rayon()
{
	string erreur("");
	
	if(clic_final.x == clic_init.x && clic_final.y == clic_init.y)
	{
		noeud_courant = nullptr;
	}
	else
	{
		double rayon_init = distance(clic_init, noeud_courant->get_centre());
		double rayon_fin = distance(clic_final, noeud_courant->get_centre());
		erreur = export_lausanne()->changement_rayon(rayon_init,rayon_fin,noeud_courant);
	}	

	return erreur;
}

bool MyArea::get_shortestPathOn()
{
	return shortestPathOn;
}

void MyArea::set_shortestPathOn(bool new_shortestPathOn)
{
	shortestPathOn = new_shortestPathOn;
}

void MyArea::draw_city()
{
    if(noeud_courant == nullptr)
    {
	    export_lausanne()->draw_all();
    }
    else
    {
	    export_lausanne()->draw_active_node(noeud_courant);
	
	    if(shortestPathOn)
		{
			unsigned int indice = export_lausanne()->find_indice(noeud_courant);
			
			if((indice != no_link) and (noeud_courant->get_type() == LOGEMENT))
			{	
				export_lausanne()->shortest_path_transport(indice);
				export_lausanne()->shortest_path_production(indice);
			}
		}
    }
}

void MyArea::refresh()
{
    auto win = get_window();
    
    if(win)
    {
		Gdk::Rectangle r(0,0,get_allocation().get_width(),
						 get_allocation().get_height());
		win->invalidate_rect(r,false);
    }
}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	if(not empty)
	{
	    Gtk::Allocation allocation = get_allocation();
	    int width = allocation.get_width();
	    int height = allocation.get_height();
	    
	    if (width > height)  
		    width= height; 
	    else 
		    height = width;
	    
	    set_width(width);
	    set_height(height);
	    avoid_distortion();
	    graphic_set_context(cr);
	    set_white_background();
	    draw_square_canvas();
	    draw_city();
    }
    return true;
}

//~ //--------------------------------------

Gui::Gui() :

  zoom(default_zoom),
  m_Box(Gtk::ORIENTATION_HORIZONTAL,5),
  m_Box_Canvas(Gtk::ORIENTATION_HORIZONTAL, 5),
  m_Box_General(Gtk::ORIENTATION_VERTICAL, 5),
  m_Box_Display(Gtk::ORIENTATION_VERTICAL, 5),
  m_Box_Outils(Gtk::ORIENTATION_VERTICAL,5),
  m_Box_Editor(Gtk::ORIENTATION_VERTICAL,5),
  m_Box_Info(Gtk::ORIENTATION_VERTICAL, 5),
  m_Button_Open("Open"),
  m_Button_Save("Save"),
  m_Button_New("New"),
  m_Button_Exit("Exit"),
  m_Button_shortest_path("shortest path"),
  m_Button_zoom_in("zoom in"),
  m_Button_zoom_out("zoom out"),
  m_Button_zoom_reset("zoom reset"),
  m_Button_edit_link("edit link"),
  m_button_housing("housing"),
  m_button_transport("transport"),
  m_button_production("production"),
  m_Frame_General("General"),
  m_Frame_Display("Display"),
  m_Frame_Editor("Editor"),
  m_Frame_Info ("Informations"),
  m_Label_Zoom ("Zoom: x"+ zoom_to_string(zoom)),
  m_Label_Enj ("ENJ: "+ critere_to_string((*export_lausanne()).get_enj())),
  m_Label_CI ("CI: " + critere_to_string((*export_lausanne()).get_ci())),
  m_Label_MTA ("MTA: "+ critere_to_string((*export_lausanne()).get_mta()))
{
  // Set title and border of the window
  set_title("Archipelago"); 
  set_border_width(10);
  
  add(m_Box);
 
  set_up_boxes();
  pack_start_children();
  
  set_default_prm();
    
  set_signal_handlers();
    
  // Show all children of the window
  show_all_children();
}

Gui::~Gui()
{
}

void Gui::set_label()
{
	set_label_enj();
	set_label_ci();
	set_label_mta();
}

void Gui::set_label_enj()
{
	export_lausanne()->critere_enj();
	m_Label_Enj.set_label("ENJ: "+ critere_to_string(export_lausanne()->get_enj()));	
}

void Gui::set_label_ci()
{
	export_lausanne()->critere_ci();
	m_Label_CI.set_label("CI: " + critere_to_string(export_lausanne()->get_ci()));
}

void Gui::set_label_mta()
{
	export_lausanne()->critere_mta();
	m_Label_MTA.set_label("MTA: "+ critere_to_string(export_lausanne()->get_mta()));
}

void Gui::affichage_erreur(string erreur)
{	
	if(not erreur.empty())
	{
		if((erreur.find("edit") != string::npos) or 
		   (erreur.find("connect") != string::npos))
		{
			Gtk::MessageDialog dialog(*this, "Error edit link");			
			dialog.set_secondary_text(erreur);
			dialog.run();
		}
		else
		{
			Gtk::MessageDialog dialog(*this, "Error loading file");
			dialog.set_secondary_text(erreur);
			dialog.run();
		}
	}
} 

void Gui::set_up_boxes()
{
    m_Box.pack_start(m_Box_Outils,false,false);
    m_Box.pack_start(m_Box_Canvas,true,true);
  
    m_Box_Outils.pack_start(m_Frame_General,false,false);
    m_Frame_General.add(m_Box_General); 	
   
    m_Box_Outils.pack_start(m_Frame_Display,false,false);
    m_Frame_Display.add(m_Box_Display);
  
    m_Box_Outils.pack_start(m_Frame_Editor,false,false);
    m_Frame_Editor.add(m_Box_Editor);
  
    m_Box_Outils.pack_start(m_Frame_Info,false,false);
    m_Frame_Info.add(m_Box_Info);
  
    canvas.set_size_request(default_drawing_size,default_drawing_size);
    m_Box_Canvas.pack_start(canvas);
}

void Gui:: pack_start_children()
{
    m_Box_General.pack_start(m_Button_New,false,false);
    m_Box_General.pack_start(m_Button_Open,false,false);
    m_Box_General.pack_start(m_Button_Save,false,false) ;
    m_Box_General.pack_start(m_Button_Exit,false,false) ;  
 
    m_Box_Display.pack_start(m_Button_shortest_path,false, false);
    m_Box_Display.pack_start(m_Button_zoom_in,false,false);
    m_Box_Display.pack_start(m_Button_zoom_out,false,false);
    m_Box_Display.pack_start(m_Button_zoom_reset,false,false);  
    m_Box_Display.pack_start(m_Label_Zoom,false, false);
	
    m_Box_Editor.pack_start(m_Button_edit_link,false, false);
    m_Box_Editor.pack_start(m_button_housing,false, false);
    m_Box_Editor.pack_start(m_button_transport,false, false);
    m_Box_Editor.pack_start(m_button_production,false,false);
  
    m_button_transport.join_group (m_button_housing);
    m_button_production.join_group (m_button_housing);
 
    m_Box_Info.pack_start(m_Label_Enj,false,false);
    m_Box_Info.pack_start(m_Label_CI,false,false);
    m_Box_Info.pack_start(m_Label_MTA,false,false);
   
    m_button_housing.set_active();
}

void Gui::set_signal_handlers()
{
    m_Button_Exit.signal_clicked().connect(sigc::mem_fun(*this,
                &Gui::on_button_clicked_Exit) );
              
    m_Button_Save.signal_clicked().connect(sigc::mem_fun(*this,
                &Gui::on_button_clicked_Save) );
              
    m_Button_Open.signal_clicked().connect(sigc::mem_fun(*this,
                &Gui::on_button_clicked_Open) );  
                          
    m_Button_New.signal_clicked().connect(sigc::mem_fun(*this,
                &Gui::on_button_clicked_New) );
              
    m_Button_shortest_path.signal_toggled().connect(sigc::mem_fun(*this,
                &Gui::on_button_clicked_Shortest_Path) );
                                       
    m_Button_zoom_in.signal_clicked().connect(sigc::mem_fun(*this,
                &Gui::on_button_clicked_Zoom_In) ); 
                         
    m_Button_zoom_out.signal_clicked().connect(sigc::mem_fun(*this,
                &Gui::on_button_clicked_Zoom_Out) );   
                              
    m_Button_zoom_reset.signal_clicked().connect(sigc::mem_fun(*this,
                &Gui::on_button_clicked_Zoom_Reset) ); 
                           
    m_Button_edit_link.signal_toggled().connect(sigc::mem_fun(*this,
                &Gui::on_button_clicked_Edit_Link) ); 
               
    m_button_housing.signal_toggled().connect(sigc::mem_fun(*this,
                &Gui::on_button_clicked_housing) );  
                      
    m_button_transport.signal_toggled().connect(sigc::mem_fun(*this,
                &Gui::on_button_clicked_transport) );  
                   
    m_button_production.signal_toggled().connect(sigc::mem_fun(*this,
                &Gui::on_button_clicked_production) ); 
}

void Gui::on_button_clicked_Open()
{
    Gtk::FileChooserDialog dialog("Please choose a file", 
								  Gtk::FILE_CHOOSER_ACTION_OPEN);   
    dialog.set_transient_for(*this);
    //Add response buttons the the dialog:
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Open", Gtk::RESPONSE_OK);  
    m_Label_Info.set_text("choosing a file");

    //Show the dialog and wait for a user response:
    int result = dialog.run();

    m_Label_Info.set_text("Done choosing a file");
    //Handle the response:
    switch(result)
    {
		case(Gtk::RESPONSE_OK):
		{
		    (*export_lausanne()).supp_all();     	  

		    //Notice that this is a std::string, not a Glib::ustring.
		    std::string filename = dialog.get_filename();
		    affichage_erreur(export_lausanne()->lecture(filename));
		    canvas.draw();
		    set_label();
		    break;   
		}
		case(Gtk::RESPONSE_CANCEL):
		{
		    cout << "Cancel clicked." << std::endl;
		    break;
		}
		default:
		{
		    cout << "Unexpected button clicked." << std::endl;
		    break;
		}
    }
}
  
void Gui::on_button_clicked_Save()
{
    Gtk::FileChooserDialog dialog("Please choose a file", 
								  Gtk::FILE_CHOOSER_ACTION_SAVE); 
    Gtk::Entry texte;      
    dialog.set_transient_for(*this);
    //Add response buttons the the dialog:
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Save", Gtk::RESPONSE_OK);  
    m_Label_Info.set_text("choosing a file");

    //Show the dialog and wait for a user response:
    int result = dialog.run();

    m_Label_Info.set_text("Done choosing a file");

    //Handle the response:
    switch(result)
    {
		case(Gtk::RESPONSE_OK):
		{
		    //Notice that this is a std::string, not a Glib::ustring.
		    std::string filename = dialog.get_filename();
		    export_lausanne()->enregistrement_etat(filename);
		    break;
		}
		case(Gtk::RESPONSE_CANCEL):
		{
		    cout << "Cancel clicked." << std::endl;
		    break;
		}
		default:
		{
		    cout << "Unexpected button clicked." << std::endl;
		    break;
		}
    }
}

void Gui::on_button_clicked_Exit()
{
    hide();
}

void Gui::on_button_clicked_New()
{
    canvas.clear();
    export_lausanne()->supp_all();
    set_label();
    canvas.draw();
    noeud_courant = nullptr;
    m_Button_edit_link.set_active(false);
    m_Button_shortest_path.set_active(false);
}

void Gui::on_button_clicked_Shortest_Path()
{	
	if(m_Button_shortest_path.get_active())
	{
		if((noeud_courant != nullptr) and (noeud_courant->get_type() == LOGEMENT))
		{
			canvas.set_shortestPathOn(true);
			canvas.draw();
		}
	}
	else
	{
		canvas.set_shortestPathOn(false);
		canvas.draw();
	}
}

void Gui::on_button_clicked_Zoom_In()
{
    if (zoom < max_zoom) 
    {
		set_default_prm();
		WP newprm = getWParam();
		zoom += delta_zoom;
		newprm.XMIN = newprm.XMIN/zoom;
		newprm.XMAX = newprm.XMAX/zoom;
		newprm.YMIN = newprm.YMIN/zoom;
		newprm.YMAX = newprm.YMAX/zoom;
		newprm.ASP = ( newprm.XMAX-newprm.XMIN ) / ( newprm.YMAX-newprm.YMIN );
		setWParam(newprm);
		m_Label_Zoom.set_text("Zoom: "+ zoom_to_string(zoom));
		canvas.draw();
    }
}

void Gui::on_button_clicked_Zoom_Out()
{
	if (zoom > min_zoom+epsil_zero) //adding epsil_zero because min_zoom isn't precise
    { 
		set_default_prm();
		WP newprm = getWParam();
		zoom -= delta_zoom;
		newprm.XMIN = newprm.XMIN/zoom;
		newprm.XMAX = newprm.XMAX/zoom;
		newprm.YMIN = newprm.YMIN/zoom;
		newprm.YMAX = newprm.YMAX/zoom;
		newprm.ASP = ( newprm.XMAX-newprm.XMIN ) / ( newprm.YMAX-newprm.YMIN );
		m_Label_Zoom.set_text("Zoom: "+ zoom_to_string(zoom));
		setWParam(newprm);
		canvas.draw();
   }
}

void Gui::on_button_clicked_Zoom_Reset()
{
	set_default_prm();
	zoom=default_zoom;
	m_Label_Zoom.set_text("Zoom: "+ zoom_to_string(zoom));
	canvas.draw();
}
bool Gui::on_key_press_event(GdkEventKey * key_event)
{
  if(key_event->type == GDK_KEY_PRESS)
  {
    switch(gdk_keyval_to_unicode(key_event->keyval))
   {
     case 'i': 
     on_button_clicked_Zoom_In();
     break;
     case 'o':
     on_button_clicked_Zoom_Out();
     break;
     case 'r':
     on_button_clicked_Zoom_Reset();
     break;
   }
  }
  return Gtk::Window::on_key_press_event(key_event);
}
void Gui::on_button_clicked_Edit_Link()
{
	if(m_Button_edit_link.get_active())
	{
		if(export_lausanne()->get_city().size() <= 1)
		{
			affichage_erreur(error::not_enough_node());
			m_Button_edit_link.set_active(false);
		}
		else
		{
			if(noeud_courant == nullptr)
			{
				affichage_erreur(error::not_selected());
				m_Button_edit_link.set_active(false);
			}
		}
	}
}

void Gui::on_button_clicked_housing()
{
	if(!(etat_radio == BU_HOUSING))
	{
		etat_radio = BU_HOUSING;
	}
}

void Gui::on_button_clicked_transport()
{
	if(!(etat_radio == BU_TRANSPORT))
	{
		etat_radio = BU_TRANSPORT;
	}
}

void Gui::on_button_clicked_production()
{
	if(!(etat_radio == BU_PRODUCTION))
	{
		etat_radio = BU_PRODUCTION;
	}
}

bool Gui::on_button_press_event(GdkEventButton * event)
{
	if(event->type == GDK_BUTTON_PRESS)
	{
		// raw mouse coordinates in the window frame
		double clic_x = event->x ;
		double clic_y = event->y ;
		
		// get width and height of drawing area
		double origin_x = canvas.get_allocation().get_x();
		double origin_y = canvas.get_allocation().get_y();
	
		points clic = clic_coord(clic_x, clic_y, origin_x, origin_y);
		double x = clic.x;
		double y = clic.y;
		
		// retain only mouse events located within the drawing area
		if(clic_x >= origin_x && clic_x <= origin_x + get_width() &&
		   clic_y >= origin_y && clic_y <= origin_y + get_height())
		{ 	
			if(event->button == GAUCHE) // Left mouse button
			{
				Noeud* noeud_choisi = export_lausanne()->noeud_selectionne(x, y);
				
				if(noeud_choisi != nullptr)
					clique_sur_noeud(noeud_choisi);
				else
					clique_hors_noeud(x,y,clic);
			}
			if(event->button == DROIT) // Right mouse button
			{
				if(noeud_courant!= nullptr)
					nouvel_emplacement(x,y);
			}
		}
	}
	return true;
}

void Gui::clique_sur_noeud(Noeud* noeud_choisi)
{
	if(noeud_choisi == noeud_courant)
	{
		if(m_Button_edit_link.get_active())
			affichage_erreur(error::link_to_itself());
		else
			suppression_noeud();
	}
	else
	{
		clique_sur_noeud_non_courant(noeud_choisi);
	}
}

void Gui::suppression_noeud()
{
	export_lausanne()->delete_noeud_courant(noeud_courant);
	canvas.draw();
	set_label();
	noeud_courant = nullptr;
}

void Gui::clique_sur_noeud_non_courant(Noeud* noeud_choisi)
{
	if(m_Button_edit_link.get_active())
	{
		if(not export_lausanne()->existence_lien(noeud_courant,noeud_choisi))
			nouveau_lien(noeud_choisi);
		else
			suppression_lien(noeud_choisi);
	}
	else
	{
		nouveau_noeud_courant(noeud_choisi);
	}
}

void Gui::nouveau_lien(Noeud* noeud_choisi)
{
	string erreur ("");
	erreur = export_lausanne()->creation_lien_interactif(noeud_courant,noeud_choisi);
	if(not erreur.empty())
		affichage_erreur(erreur);
	
	canvas.draw();
	set_label_ci();
	set_label_mta();
}

void Gui::suppression_lien(Noeud* noeud_choisi)
{
	export_lausanne()->supp_link(noeud_courant,noeud_choisi);
	canvas.draw();
	set_label_ci();
	set_label_mta();
}

void Gui::nouveau_noeud_courant(Noeud* noeud_choisi)
{
	noeud_courant = noeud_choisi;
	
	if(m_Button_shortest_path.get_active() and noeud_courant->get_type() == LOGEMENT)
		canvas.set_shortestPathOn(true);
	
	canvas.draw();
}

void Gui::nouveau_noeud(double x,double y)
{
	switch(etat_radio)
	{
	    case BU_HOUSING :
			affichage_erreur(export_lausanne()->creation_noeud(LOGEMENT,x, y));
			break;
	    case BU_TRANSPORT :
			affichage_erreur(export_lausanne()->creation_noeud(TRANSPORT,x, y));
			break;
	    case BU_PRODUCTION :
			affichage_erreur(export_lausanne()->creation_noeud(PRODUCTION,x, y));
			break;
	    default :
			break;
	}
	
	canvas.draw();
	
	set_label_enj();
	set_label_mta();
}

void Gui::clique_hors_noeud(double x, double y, points clic)
{
	if(noeud_courant == nullptr)
		nouveau_noeud(x,y);
	else
		canvas.init_rayon(clic);
}
	
void Gui::nouvel_emplacement(double x, double y)
{
	affichage_erreur(export_lausanne()->changement_place(noeud_courant, x, y));
	canvas.draw();
	set_label_ci();
	set_label_mta();
}

bool Gui::on_button_release_event(GdkEventButton * event)
{
	if(event->type == GDK_BUTTON_RELEASE)
	{
		// raw mouse coordinates in the window fradouble x = clic.x;
		double clic_x = event->x ;
		double clic_y = event->y ;
		
		// get width and height of drawing area
		double origin_x = canvas.get_allocation().get_x();
		double origin_y = canvas.get_allocation().get_y();
	
		points clic = clic_coord(clic_x, clic_y, origin_x, origin_y);
		double x = clic.x;
		double y = clic.y;
		
		if(m_Button_edit_link.get_active())
			return true;
		
		// retain only mouse events located within the drawing area
		if(clic_x >= origin_x && clic_x <= origin_x + get_width() &&
		   clic_y >= origin_y && clic_y <= origin_y + get_height()) 
		{ 
			if(event->button == GAUCHE)
			{
				Noeud* noeud_choisi = export_lausanne()->noeud_selectionne(x, y);
				if((noeud_courant != nullptr) and (noeud_choisi != noeud_courant))
				{
					nouvelle_taille(clic);
				}
			}
		}
	}
	return true;
}

void Gui::nouvelle_taille(points clic)
{
	canvas.finalize_rayon(clic);
	string erreur = canvas.condition_new_rayon();
	affichage_erreur(erreur);
	
	canvas.draw();
	set_label_enj();
	set_label_ci();
}

points Gui::clic_coord(double x, double y, double origine_x, double origine_y)
{
	double width = canvas.get_allocation().get_width();
	double height= canvas.get_allocation().get_height();
    if (width > height)  
		width= height; 
	else 
	    height = width;
	double real_clic_x = x - origine_x;
	double real_clic_y = y - origine_y;
	
	double x_coord = convertXtoModel(real_clic_x,width);
	double y_coord = convertYtoModel(real_clic_y,height);
	
	points clic = {x_coord,y_coord};
	
	return clic;
}

void Gui::set_color_noeud_courant()
{
	int type=noeud_courant->get_type();
	
	switch(type)
	{
		case LOGEMENT:
			noeud_courant->draw_node(RED);
			break;
		case TRANSPORT:
			noeud_courant->draw_node(RED);
			break;
		case PRODUCTION:
			noeud_courant->draw_node(RED);
			break;
		default :
			break;
	}
}

void Gui::set_default_prm() 
{
    WP prm; 
    prm.XMIN = -dim_max;
    prm.XMAX = dim_max;
    prm.YMIN = -dim_max;
    prm.YMAX = dim_max;;
    prm.ASP = ( prm.XMAX-prm.XMIN ) / ( prm.YMAX-prm.YMIN );
    setWParam(prm);
}


namespace
{
	std::string critere_to_string(const double& critere)
	{
		stringstream critere_;
		critere_ << critere;
		string string_critere = critere_.str();
		return string_critere;
	}
	
	std::string zoom_to_string(const double& zoom) 
	{
	  std::stringstream zoom_;
	  zoom_ << std::fixed << std::setprecision(2) << zoom;
	  std::string string_zoom = zoom_.str();
	  return string_zoom;
	}
}
