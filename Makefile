EXECUTABLE := mpist
LIBRARY := MPIGrid
EXAMPLE := grid-example

SRCDIR = ./src
LIBDIR = ./lib
EXADIR = ./example
OBJDIR = ./build

SHELL = /bin/sh

CC := mpicc
export OMPI_CC=gcc-6.2
CFLAGS := -g -Os -pedantic -D_XOPEN_SOURCE=600

XLKEY := /wrappers/xl
ifeq ($(XLKEY),$(findstring $(XLKEY), $(PATH)))
	CFLAGS += -qlanglvl=stdc99
else # Assume GCC or compatible
	CFLAGS += -std=c99 -Wall
endif

LIBNAME := lib$(LIBRARY).a

HEADERS := $(wildcard $(SRCDIR)/*.h)
SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst $(SRCDIR)%.c,$(OBJDIR)%.o, $(SOURCES))
LSOURCE := $(wildcard $(LIBDIR)/*.c) 
LOBJECT := $(patsubst $(LIBDIR)%.c,$(OBJDIR)%.o, $(LSOURCE))
ESOURCE := $(wildcard $(EXADIR)/*.c) 
EOBJECT := $(patsubst $(EXADIR)%.c,$(OBJDIR)%.o, $(ESOURCE))

all: $(SOURCES) $(HEADERS) $(EXECUTABLE) library


$(OBJDIR)/%.o : */%.c
	$(CC) -c $(CFLAGS) $< -o $@


$(EXECUTABLE): $(OBJECTS) $(HEADERS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	strip $@

$(LIBRARY): $(LOBJECT)
	ar rs $(LIBDIR)/$(LIBNAME) $(LOBJECT)

$(EXAMPLE): $(LIBRARY) $(EOBJECT)
	$(CC) $(EOBJECT) $(LDFLAGS) -L$(LIBDIR) -l$(LIBRARY) -o $@
	@echo "Try: mpirun -np 2 ./$(EXAMPLE) -f 2-3 A B C"

example: $(EXAMPLE)

library: $(LIBDIR)/$(LIBRARY)

clean:
	rm $(EXECUTABLE)        || true
	rm $(LIBDIR)/$(LIBNAME) || true
	rm $(EXAMPLE)    	|| true
	cd $(OBJDIR) && rm * 	|| true
