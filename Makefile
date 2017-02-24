CC         = g++
LD         = g++

#HEPMCDIR   = $(shell echo $(HEPMCDIR))
HEPMCLIB   = -L$(HEPMCDIR)/lib -lHepMC

ROOTLIB = $(shell root-config --libs)

INCLUDES   = -I$(HEPMCDIR)/include
INCLUDES  += $(shell root-config --cflags)

CXXFLAGS  += -g -Wno-long-long -fPIC $(INCLUDES) -ftree-vectorize -ffast-math

LDFLAGS    = 
LDFLAGS   += $(HEPMCLIB)
LDFLAGS   += $(ROOTLIB)

OBJS_ANA = Analysis.o
OBJS_GEN = generate.o Selectors.o Process.o Unweight.o BhabhaProcess.o Helper.o ProcessComp.o

OPT = -O4

%.o: %.cxx
	$(CC) -c $(OPT) $(CXXFLAGS) -o $@ $<

all: Analysis generate

Analysis: $(OBJS_ANA)
	$(LD) $(OPT) -o $@ $(OBJS_ANA) -L. $(LDFLAGS)

generate: $(OBJS_GEN)
	$(LD) $(OPT) -o $@ $(OBJS_GEN) -L. $(LDFLAGS)

clean:
	rm -f *.o *.so Analysis generate

distclean: clean
	rm -f input.hepmc2g out.root

