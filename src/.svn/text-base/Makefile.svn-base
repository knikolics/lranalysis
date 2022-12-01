# $(BINDIR)/XXXX: $(LIBS) $(LIBDIR)/XXXX.o 
#	echo 2. $(LINK) $(LINKOPTIONS) $(LIBDIR)/XXXX.o $(shell ls -1 $(LIBDIR)/lib*.a) -o $@
#	$(LINK) $(LINKOPTIONS) $(LIBDIR)/XXXX.o $(shell ls -1 $(LIBDIR)/lib*.a) -o $@
#	echo

.SILENT:

SUBDIRS			:= ArAnalysis ArRun ArIO ArDisplay
LIBS			:= $(shell ls -1 $(LIBDIR)/lib*.a)


#NR means no root in the definition of the libraries
.PHONY: all  $(SUBDIRS)



$(SUBDIRS):
	echo 2. Entering in $(SRCDIR)/$@
	gmake -C $@ all
	echo 2. Leaving  in $(SRCDIR)/$@
	echo 

all: $(SUBDIRS)  $(BINDIR)/LRAnalysis $(BINDIR)/LookAt
	echo 2. All done
	echo

$(BINDIR)/LRAnalysis: $(LIBS) $(LIBDIR)/LRAnalysis.o 
	echo 2. $(LINK) $(LINKOPTIONS) $(LIBDIR)/LRAnalysis.o $(LIBDIR)/libArAnalysis.a $(LIBDIR)/libArRun.a $(LIBDIR)/libArIO.a $(LIBDIR)/libArDisplay.a $(INCDIR)/ArDictionary.o -o $@
	$(LINK) $(LINKOPTIONS) $(LIBDIR)/LRAnalysis.o $(LIBDIR)/libArAnalysis.a $(LIBDIR)/libArRun.a $(LIBDIR)/libArIO.a $(LIBDIR)/libArDisplay.a $(INCDIR)/ArDictionary.o  -o $@
	echo

$(BINDIR)/LookAt: $(LIBS) $(LIBDIR)/LookAt.o 
	echo 2. $(LINK) $(LINKOPTIONS) $(LIBDIR)/LookAt.o $(LIBDIR)/libArAnalysis.a $(LIBDIR)/libArRun.a $(LIBDIR)/libArIO.a $(LIBDIR)/libArDisplay.a $(INCDIR)/ArDictionary.o -o $@
	$(LINK) $(LINKOPTIONS) $(LIBDIR)/LookAt.o $(LIBDIR)/libArAnalysis.a $(LIBDIR)/libArRun.a $(LIBDIR)/libArIO.a $(LIBDIR)/libArDisplay.a $(INCDIR)/ArDictionary.o  -o $@
	echo

$(LIBDIR)/%.o: %.cpp
	echo 2. $(CPP) -c $(CPPOPTIONS) $(CURDIR)/$< -o $@
	$(CPP) -c $(CPPOPTIONS) $(CURDIR)/$< -o $@
	echo
