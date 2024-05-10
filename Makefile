# Definitions de macros
OUT = projet
CXX     = g++
CXXFLAGS = -g -Wall -std=c++11
CXXFILES = tools.cc noeud.cc ville.cc error.cc projet.cc graphic.cc gui.cc
LINKING = `pkg-config --cflags gtkmm-3.0`	
LDLIBS = `pkg-config --libs gtkmm-3.0`
OFILES = $(CXXFILES:.cc=.o)

# Definition de la premiere regle
all: $(OUT)

# Definitions de cibles particulieres

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CXX) -MM $(CXXFLAGS) $(CXXFILES) | \
	  egrep -v "/usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile

clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@/bin/rm -f *.o *.x *.cc~ *.h~ prog

#
# -- Regles de dependances generees automatiquement
#
# DO NOT DELETE THIS LINE
tools.o: tools.cc tools.h graphic.h graphic_color.h
noeud.o: noeud.cc noeud.h tools.h constantes.h error.h
ville.o: ville.cc ville.h noeud.h tools.h constantes.h error.h
error.o: error.cc error.h constantes.h
projet.o: projet.cc gui.h ville.h noeud.h tools.h constantes.h error.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
graphic.o: graphic.cc graphic_gui.h graphic.h graphic_color.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
gui.o: gui.cc graphic_gui.h graphic.h gui.h ville.h noeud.h tools.h \
 constantes.h error.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

$(OUT): $(OFILES)
	$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o $@ $(LDLIBS)
