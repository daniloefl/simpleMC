CC         = g++
LD         = g++

OBJDIR   = obj
SRCDIR   = src
INCLUDES = -Iinc

# only for Analysis
ROOTLIB = $(shell root-config --libs)
INCLUDES += $(shell root-config --cflags)

CXXFLAGS  += -g -Wno-long-long -fPIC -ftree-vectorize -ffast-math

LDFLAGS    = 

OBJS_ANA = $(addprefix $(OBJDIR)/, Analysis.o)
OBJS_GEN = $(addprefix $(OBJDIR)/, generate.o Selectors.o Process.o Unweight.o BhabhaProcess.o Helper.o ProcessComp.o FourVector.o ThreeVector.o)


OPT = -O4

$(OBJDIR)/%.o: $(SRCDIR)/%.cxx $(OBJDIR)
	$(CC) -c $(OPT) $(CXXFLAGS) $(INCLUDES) -o $@ $<

all: generate Analysis

$(OBJDIR):
	mkdir $(OBJDIR)

Analysis: $(OBJS_ANA)
	$(LD) $(OPT) -o $@ $(OBJS_ANA) -L. $(LDFLAGS) $(ROOTLIB)

generate: $(OBJS_GEN)
	$(LD) $(OPT) -o $@ $(OBJS_GEN) -L. $(LDFLAGS)

doxy:
	doxygen doxygen.txt

clean:
	rm -f $(OBJDIR)/*.o Analysis generate


