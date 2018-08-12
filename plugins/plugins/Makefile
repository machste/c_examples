CFLAGS_SO = $(CFLAGS) -fpic

PLUGINS := $(patsubst %.c,%.so,$(wildcard *.c))

all: $(PLUGINS)

%.o: %.c
	$(CC) -c $(CFLAGS_SO) $^

%.so: %.o
	$(CC) -shared -o $@ $^

clean:
	rm -rf *.o *.so

.PHONY: clean
