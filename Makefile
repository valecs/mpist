EXECUTABLE := mpist

SRCDIR = ./src
OBJDIR = ./build

SHELL = /bin/sh

CC := mpicc
CFLAGS := -Os -pedantic -D_XOPEN_SOURCE=700

XLKEY := xl
ifeq ($(XLKEY),$(findstring $(XLKEY), $(shell $(CC) 2>&1)))
	CFLAGS += -qlanglvl=stdc99
else # Assume GCC or compatible
	CFLAGS += -std=c99
endif

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
