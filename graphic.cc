//OUSMANE EL HAMID / LILIAN LAPORTE / graphic.cc 
//Architecture b1
#include "graphic_gui.h"
#include "graphic_color.h"

#define BLANC 1, 1, 1
#define ROUGE 1, 0, 0
#define NOIR 0, 0, 0
#define VERT 0, 1, 0

using namespace std;

static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr); 
static double width, height;
static WP prm_area;    
static WP prm_area_ref;

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr)
{
	static bool init(false);

	if(!init)
	{
		ptcr = &cr;
		init = true;
	}
}

WP getWParam() 
{ 
	return prm_area;
}

void setWParam(WP x) 
{ 
	prm_area_ref = x;
	prm_area     = prm_area_ref;
} 

void set_width (const double& width_canvas) 
{
	width = width_canvas ;
}

void set_height (const double& height_canvas) 
{
	height = height_canvas ;
	prm_area.HEIGHT= height_canvas;
}

double get_width() 
{
	return width;
}

double get_height() 
{
	return height;
}

void set_white_background ()
{
    (*ptcr)->set_source_rgb(BLANC); 
    (*ptcr)->paint();	
}	

void setColor(Color couleur) 
{
    switch(couleur) 
    {
        case BLACK:			
            (*ptcr)->set_source_rgb(NOIR);
            break;
        case RED:
            (*ptcr)->set_source_rgb(ROUGE);
            break;
        case GREEN:
            (*ptcr)->set_source_rgb(VERT);
            break;
        default: 
			break;
    }
}
void graphic_draw_cercle( int	 xc, int yc, double r)
{  
	(*ptcr)->save();
	(*ptcr)->arc(convertXtoDraw(xc,width),convertYtoDraw(yc,height),convertRtoDraw(r,width) ,
				 0.0, 2.0 * M_PI); // full circle
	(*ptcr)->set_source_rgb(BLANC);   
	(*ptcr)->fill_preserve();
	(*ptcr)->restore();  // back to opaque black
	(*ptcr)->stroke();
  
}
void graphic_draw_droite( int xo, int yo, int xf, int yf)
{
    (*ptcr)->move_to(convertXtoDraw(xo,width),convertYtoDraw(yo,height));
    (*ptcr)->line_to(convertXtoDraw(xf,width),convertYtoDraw(yf,height));
    (*ptcr)->stroke();
}

double convertRtoDraw(double rayon, double width)
{
    double prefactor = width /(( prm_area.XMAX - prm_area.XMIN )) ; 
    return rayon * prefactor;	
}

double convertXtoDraw (double coord, double width)
{
    double prefactor = width / ( prm_area.XMAX - prm_area.XMIN ); 
    return ( coord - prm_area.XMIN ) * prefactor;	
} 

double convertYtoDraw(double coord, double height)
{	
    double prefactor = height / ( prm_area.YMAX - prm_area.YMIN ); 
    return ( prm_area.YMAX - coord ) * prefactor;	
}

double convertRtoModel(double rayon, double width)
{
    double prefactor = ( prm_area.XMAX - prm_area.XMIN )/width ; 
    return rayon * prefactor;	
}

double convertXtoModel(double coord, double width)
{
	double prefactor = (coord / width) * ( prm_area.XMAX - prm_area.XMIN );
	return prefactor + prm_area.XMIN;
}

double convertYtoModel(double coord, double height)
{
	double prefactor = (coord / height) * ( prm_area.YMAX - prm_area.YMIN );
	return prm_area.YMAX - prefactor;
}

void avoid_distortion()
{
	double new_aspect_ratio((double)width/height);
	if( new_aspect_ratio > prm_area_ref.ASP)
	{ // keep YMAX and YMIN. Adjust XMAX and XMIN
		prm_area.YMAX = prm_area_ref.YMAX ;
		prm_area.YMIN = prm_area_ref.YMIN ;	
	  
		double delta(prm_area_ref.XMAX - prm_area_ref.XMIN);	
		double mid((prm_area_ref.XMAX + prm_area_ref.XMIN)/2);

		prm_area.XMAX = mid + 0.5*(new_aspect_ratio/prm_area_ref.ASP)*delta ;
		prm_area.XMIN = mid - 0.5*(new_aspect_ratio/prm_area_ref.ASP)*delta ;		  	  
	}
	else
	{ // keep XMAX and XMIN. Adjust YMAX and YMIN
		prm_area.XMAX = prm_area_ref.XMAX ;
		prm_area.XMIN = prm_area_ref.XMIN ;
	  	  
		double delta(prm_area_ref.YMAX - prm_area_ref.YMIN);
		double mid((prm_area_ref.YMAX + prm_area_ref.YMIN)/2);

		prm_area.YMAX = mid + 0.5*(prm_area_ref.ASP/new_aspect_ratio)*delta ;
		prm_area.YMIN = mid - 0.5*(prm_area_ref.ASP/new_aspect_ratio)*delta ;	
	}
}

void draw_square_canvas ()
{ 
    (*ptcr)->set_source_rgb(NOIR);
    (*ptcr)->save();
    (*ptcr)->rectangle(0, 0, width, height);
    (*ptcr)->set_source_rgb(BLANC);
    (*ptcr)->fill_preserve();
    (*ptcr)->restore();
    (*ptcr)->set_line_width(1.0);
    (*ptcr)->stroke();
}

