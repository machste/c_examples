CFLAGS = -g -O0 -Werror
PROGS = encode64 decode64
OBJECTS = base64.o

.PHONY: all
all: $(PROGS)

$(PROGS): %: %.c $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -rf $(PROGS) *.o *.gcno *.gcda *.gcov
