EXECUTABLE:=mpist

SRCDIR=./src
OBJDIR=./build

SHELL = /bin/sh

CFLAGS := -g -O2 -DHAVE_INLINE
CFLAGS += -I$(HOME)/local/include -I$(CODEROOT)/include


CC := mpicc
# Use for GCC
WARNINGS := -pedantic -Wall -W -Wmissing-prototypes -Wstrict-prototypes -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wnested-externs -Werror -Wuninitialized -Wconversion
CFLAGS += $(WARNINGS)
CFLAGS += -std=c99 -fshort-enums -fno-common

# Use for XL
#CFLAGS += -qlanglvl=stdc99
#LDFLAGS += -L/soft/apps/ibmcmp-feb2013/xlf/bg/14.1/bglib64 -lxlopt -lxl -lxlf90_r -lxlfmath

HEADERS := $(wildcard $(SRCDIR)/*.h)
SOURCES := $(wildcard $(SRCDIR)/*.c) 
OBJECTS := $(patsubst $(SRCDIR)%.c,$(OBJDIR)%.o, $(SOURCES))

TOBJECTS := $(patsubst $(TSTDIR)%.c,$(OBJDIR)%.o, $(TSOURCES))

all: $(SOURCES) $(HEADERS) $(EXECUTABLE)


$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(EXECUTABLE): $(OBJECTS) $(HEADERS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	strip $@

clean:
	rm $(EXECUTABLE) || true
	cd $(OBJDIR) && rm * || true
