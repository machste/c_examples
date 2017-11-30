PROGS = glist
PKGS = glib-2.0
MY_CFLAGS = -g -O0 -Werror

INCLUDE_DIRS = $(shell pkg-config --cflags $(PKGS))
CFLAGS = $(MY_CFLAGS) $(INCLUDE_DIRS)
LDLIBS = $(shell pkg-config --libs $(PKGS))

all: $(PROGS)

.PHONY: clean
clean:
	rm -rf *.o $(PROGS)
