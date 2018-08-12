SUBDIRS = plugins

export CFLAGS = -Wall -Werror -g

all: convert $(SUBDIRS)

plugins.o: plugins.h

convert: convert.c plugins.o
	$(CC) $(CFLAGS) -Wl,--export-dynamic $^ -ldl -o $@

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

clean: $(SUBDIRS)
	rm -rf *.o $(PROG)

.PHONY: all clean $(SUBDIRS)
