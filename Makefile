
CXX= g++ -std=c++11 
CXXFLAGS= -I/usr/include/qt4 -I/usr/include/qwt -I/usr/include/qt4/Qt
LDFLAGS= 
LDLIBS= -lqwt -lQtGui -lQtCore -lpthread 

Q_OBJ= dtv.o 
OBJ= 
MOC_OBJ=$(Q_OBJ:.o=.moc.o)
MOC_SRC=$(MOC_OBJ:.o=.cc)

	
all: ndtv

ndtv: main.cc $(OBJ) $(Q_OBJ) $(MOC_OBJ)
	$(CXX) $(CXXFLAGS) main.cc $(OBJ) $(Q_OBJ) $(MOC_OBJ) $(LDFLAGS) $(LDLIBS) -o $@


$(MOC_SRC): %.moc.cc: %.h
	moc $< >$@
	
clean:
	@rm -f ndtv $(OBJ) $(Q_OBJ) $(MOC_OBJ) $(MOC_SRC) core
	

########################################################################

dtv.o: dtv.h loader.h
