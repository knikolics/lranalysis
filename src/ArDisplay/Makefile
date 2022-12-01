# // V@B - 14/07/05 - Makefile
# //
# // -=- Makefile for ArDisplay=-
# //
# //

.SILENT:
.PHONY: all 

LIBRARY			:= ArDisplay
CPPFILES		:= $(shell ls -1 *.cc)
OBJECTFILES		:= $(CPPFILES:%.cc=$(LIBDIR)/$(LIBRARY)/%.o)

all: $(LIBDIR)/$(LIBRARY) $(LIBDIR)/lib$(LIBRARY).a  
	echo 3. All done

$(LIBDIR)/$(LIBRARY):
	mkdir $(LIBDIR)/$(LIBRARY)

$(LIBDIR)/lib$(LIBRARY).a: $(OBJECTFILES)
	echo 3. $(LD) $(LDOPTIONS) -r $(OBJECTFILES) -o $@
	$(LD) $(LDOPTIONS) -r $(OBJECTFILES)   -o  $@
	echo

$(LIBDIR)/$(LIBRARY)/%.o: %.cc
	echo 3. $(CPP) -c $(CPPOPTIONS) $(CURDIR)/$< -o $@
	$(CPP) -c $(CPPOPTIONS) $(CURDIR)/$< -o $@
	echo
