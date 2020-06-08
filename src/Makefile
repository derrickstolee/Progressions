#
# Base Makefile for Progressions project.
#
TESTS 	=
 
TARGETS = progressions.exe


OBJECTS = 	ProgressionManager.o	\
			ProgressionTable.o	


CC 	= gcc
CXX = g++

OPT = -O0 -g 
# -O7
DEBUG =  
# -g 
# -static 
WARNINGS = -Wall


CFLAGS 		= $(OPT) $(DEBUG) $(WARNINGS)
CXXFLAGS 	= $(OPT) $(DEBUG) $(WARNINGS)
LFLAGS 		= $(OPT) $(DEBUG) $(WARNINGS)


TREESEARCHDIR = ../../TreeSearch

INCLUDES =  -I$(TREESEARCHDIR) 

TREESEARCHOBJS	= $(TREESEARCHDIR)/SearchManager.o 
			
LIBOBJS			= $(TREESEARCHOBJS)
			
			
.SUFFIXES: .c .cpp .o .obj .exe 

all: $(OBJECTS) $(TESTS) $(TARGETS)


# The default object compiler
.c.o: $<
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) -c $< -o $@
        
.cpp.o: $<
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -c $< -o $@
        
.cpp.exe: $< $(OBJECTS)  
	$(CXX) $(LFLAGS)			\
        	$(INCLUDES)	$(DEBUG)			\
        	$(LIBOBJS) $(LIBS)				\
        	`cat $@.objs`           		\
            $< -o $@
        
.c.exe: $< $(COBJECTS)
	$(CC) 	$(LFLAGS)			    \
        	$(INCLUDES)				\
        	$(NAUTYOBJS)  $(COBJECTS) $(LIBS)		\
            $< -o $@
        
clean:
	rm $(OBJECTS) $(TARGETS) $(TESTS)
	
cleanexe:
	rm $(TARGETS)

cleantest:
	rm $(TESTS)

clexe:
	rm $(TARGETS)
