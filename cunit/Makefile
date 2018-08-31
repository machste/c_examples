PROGS = cunit-test
CFLAGS = -g -Wall
LDLIBS = $(shell pkg-config --libs cunit)


all: $(PROGS)


.PHONY: clean
clean:
	rm -rf *.o *.xml $(PROGS)
