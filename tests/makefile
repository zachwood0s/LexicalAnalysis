CC := g++ # This is the main compiler
CFLAGS := -g -Wall -std=c++11

SRCEXT := cpp
SRCDIR := ../src
BUILDDIR := ../build
SOURCES := $(SRCDIR)/lexar.cpp $(SRCDIR)/parser.cpp
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

tests: tests.o $(OBJECTS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

run:
	./tests

clean:
	rm tests.o tests
