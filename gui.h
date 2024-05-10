//OUSMANE EL HAMID / LILIAN LAPORTE / gui.h
//Architecture b1
#ifndef GTKMM_EXAMPLE_MYEVENT_H
#define GTKMM_EXAMPLE_MYEVENT_H
#include <gtkmm.h>
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <string>
#include "ville.h"

class MyArea : public Gtk::DrawingArea
{
public:
  MyArea();
  virtual ~MyArea();
  void clear();
  void draw();
  void init_rayon(points p);
  void finalize_rayon(points p);
  std::string condition_new_rayon();
  bool get_shortestPathOn();
  void set_shortestPathOn(bool new_shortestPathOn);
  void draw_city();

private:
  bool empty;
  bool shortestPathOn;
  void refresh();  
  points clic_init;
  points clic_final;
  
protected:
  //Override default signal handler:   
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
};


class Gui : public Gtk::Window
{
public:
  Gui();
  virtual ~Gui();
  void set_label();
  void set_label_enj();
  void set_label_ci();
  void set_label_mta();
  void affichage_erreur(std::string erreur);

protected:
  double zoom;
  void set_up_boxes();
  void pack_start_children();
  void set_signal_handlers();
  //Button Signal handlers:
  void on_button_clicked_Open();
  void on_button_clicked_Save();
  void on_button_clicked_New();
  void on_button_clicked_Exit();
  void on_button_clicked_Shortest_Path();
  void on_button_clicked_Zoom_In();
  void on_button_clicked_Zoom_Out();
  void on_button_clicked_Zoom_Reset();
  bool on_key_press_event(GdkEventKey * key_event);
  void on_button_clicked_Edit_Link();
  void on_button_clicked_housing();
  void on_button_clicked_transport();
  void on_button_clicked_production();
  bool on_button_press_event(GdkEventButton * event);
  void clique_sur_noeud(Noeud* noeud_choisi);
  void suppression_noeud();
  void clique_sur_noeud_non_courant(Noeud* noeud_choisi);
  void nouveau_lien(Noeud* noeud_choisi);
  void suppression_lien(Noeud* noeud_choisi);
  void nouveau_noeud_courant(Noeud* noeud_choisi);
  void nouveau_noeud(double x, double y);
  void clique_hors_noeud(double x, double y, points clic);
  void nouvel_emplacement(double x, double y);
  bool on_button_release_event(GdkEventButton * event);
  void nouvelle_taille(points clic);
  points clic_coord(double x, double y, double origine_x, double origine_y); 
  void set_color_noeud_courant();
  void set_default_prm();
  
  Gtk::Box 			  m_Box, m_Box_Canvas,m_Box_General,m_Box_Display, m_Box_Outils, 
					  m_Box_Editor, m_Box_Info;
  Gtk::Button 		  m_Button_Open;
  Gtk::Button 		  m_Button_Save;
  Gtk::Button 		  m_Button_New;
  Gtk::Button 		  m_Button_Exit;
  Gtk::ToggleButton   m_Button_shortest_path;
  Gtk::Button         m_Button_zoom_in;
  Gtk::Button         m_Button_zoom_out;
  Gtk::Button         m_Button_zoom_reset;
  Gtk::ToggleButton   m_Button_edit_link;
  Gtk::RadioButton    m_button_housing;
  Gtk::RadioButton    m_button_transport;
  Gtk::RadioButton    m_button_production;
  Gtk::Frame          m_Frame_General;
  Gtk::Frame          m_Frame_Display;
  Gtk::Frame          m_Frame_Editor;
  Gtk::Frame          m_Frame_Info;
  Gtk ::Label         m_Label_Zoom;      
  Gtk ::Label         m_Label_Enj; 
  Gtk ::Label         m_Label_CI; 
  Gtk ::Label         m_Label_Info; 
  Gtk ::Label         m_Label_MTA; 
  MyArea              canvas;                  
  
};

#endif // GTKMM_EXAMPLE_MYEVENT_H
