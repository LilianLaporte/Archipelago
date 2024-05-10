//OUSMANE EL HAMID / LILIAN LAPORTE / graphic_gui.h
//Architecture b1
#ifndef GTKMM_EXAMPLE_GRAPHIC_GUI_H
#define GTKMM_EXAMPLE_GRAPHIC_GUI_H

#include <gtkmm/drawingarea.h>
#include "graphic.h"

struct WP // window parameters
{
   double XMIN; // field parameters
   double XMAX;
   double YMIN;
   double YMAX;
   int HEIGHT; // window height
   double ASP; // aspect ratio
};

WP getWParam();
void setWParam(WP x); // reads windows parameters
void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr);
double convertRtoDraw(double rayon, double width);
double convertXtoDraw(double coord, double width);
double convertYtoDraw(double coord, double height);
double convertRtoModel(double rayon, double width);
double convertXtoModel(double coord, double width);
double convertYtoModel(double coord, double height);
void set_width (const double& width_canvas);
void set_height (const double& height_canvas);
double get_width();
double get_height();
void set_white_background ();
void avoid_distortion();
void draw_square_canvas ();

#endif // GTKMM_EXAMPLE_GRAPHIC_GUI_H
