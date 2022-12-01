# // V@B - 05/02/08 - Makefile
# //
# // -=- Makefile for LRAnalysis=-
# //
# //

ROOTCFLAGS	= $(shell root-config --cflags)
ROOTLIBS	= $(shell root-config --glibs)

.SILENT:
.PHONY: all clean distclean Depend LRAnalysis

export ROOTDIR		= $(CURDIR)

export BINDIR		= $(ROOTDIR)/bin
export LIBDIR		= $(ROOTDIR)/lib
export TMPDIR		= $(ROOTDIR)/tmp
export SRCDIR		= $(ROOTDIR)/src
export INCDIR		= $(ROOTDIR)/include
export DEPDIR		= $(ROOTDIR)/Depend
export TESTDIR		= $(SRCDIR)/test
#export SYSCONF_LIBS_X11 = -lXft -lXext -lX11 -lm

#export QTLIBSFLAG       = -L$(QTLIB) $(SYSCONF_LIBS_X11) 
#-lQtGSI
#export QTCXXFLAGS       = -I$(QTINC) $(CXXFLAGS) -DDEBUG_LEVEL=$(QDL) $(shell root-config --cflags)

export CPPOPTIONS	= -g3 -O0 -fPIC -I$(SRCDIR) -I$(INCDIR) $(ROOTCFLAGS) $(QTCXXFLAGS) 
export CPPOPTIONSNR	= -g3 -O0 -I$(SRCDIR) 

# -Wall
# export DEPENDOPTIONS	= -D__MAKEDEPEND
export LDOPTIONS	= 
export LINKOPTIONS      = -L$(LIBDIR) $(ROOTLIBS) $(QTLIBSFLAG) -lSpectrum -lncurses -lpthread
# export LINKOPTIONS	= -L$(LIBDIR) -lpthread -lcurses $(ROOTLIBS)

export LINKOPTIONSNR	= -lpthread -lcurses 

export CPP		= g++
export LD		= ld
export LINK		= g++

all: $(BINDIR) $(LIBDIR) $(INCDIR) Dictionary LRAnalysis


LRAnalysis: $(BINDIR) $(LIBDIR) 
	echo 1. Entering in $(SRCDIR)
	gmake -C src all
	#make -C src all
	echo 1. Leaving  in $(SRCDIR)
	echo 1. Generating the shared library
	g++ -O -shared -L$(ROOTSYS)/lib -lCore -lCint -lHist -lGraf -lGraf3d -lGpad -lTree -lRIO -lNet -lRint -lPostscript -lMatrix -lPhysics -lGui -pthread -lm -ldl -rdynamic -o $(LIBDIR)/ArDM.so `ls lib/*/*.o` $(INCDIR)/ArDictionary.o
	echo

distclean: clean

clean:
	rm -fr $(BINDIR)
	rm -fr $(LIBDIR)
	rm -fr $(INCDIR)
	rm -fr src/InspectorDict.*
	rm -f core*
	echo 1. Distribution cleared

$(BINDIR):
	mkdir $(BINDIR)

$(LIBDIR):
	mkdir $(LIBDIR)

$(INCDIR):
	mkdir $(INCDIR)
	for lib in ${SRCDIR} ; do \
	  echo -n "0. Setting links... "; \
	  listLib=`ls $${lib}/*/*.hh $${lib}/ArLinkDef.h`; \
	  for hname in $${listLib} ; do \
	    ln -fs $${hname} $$INCDIR/$${hname##*/}; \
	  done;\
	done;\
	echo done

Dictionary:
	echo 0. Creating Dictionary
	rm -f $(INCDIR)/ArDictionary.h
	rootcint -f $(INCDIR)/ArDictionary.cxx -c -p -I$(INCDIR) $(INCDIR)/*.hh $(INCDIR)/*.h
	g++ -g -O -Wall -fPIC -DUSE_ROOT -pthread -I$(ROOTSYS)/include -I. -c $(INCDIR)/ArDictionary.cxx -o $(INCDIR)/ArDictionary.o

