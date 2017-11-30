TEST_PROGS = base64_test
PKGS = glib-2.0
OBJECTS = ../base64.o
MY_CFLAGS = -g -O0 -Werror

PKG_CONFIG = pkg-config
INCLUDE_DIRS = $(shell $(PKG_CONFIG) --cflags $(PKGS))
LDLIBS = $(shell $(PKG_CONFIG) --libs $(PKGS))
COV_FLAGS = -fprofile-arcs -ftest-coverage
CFLAGS = $(MY_CFLAGS) $(INCLUDE_DIRS)

COV_PROGS = $(TEST_PROGS:%=%.cov)
COV_OBJS = $(OBJECTS:%.o=%.cov.o)

$(TEST_PROGS): %: %.c $(OBJECTS)
	$(LINK.c) $^ $(LOADLIBS) $(LDLIBS) -o $(@)

%.cov.o: %.c
	$(COMPILE.c) $(COV_FLAGS) $(OUTPUT_OPTION) $<

$(COV_PROGS): %.cov: %.c $(COV_OBJS)
	$(LINK.c) $(COV_FLAGS) $^ $(LOADLIBS) $(LDLIBS) -o $(@)

.PHONY: tests
tests: $(TEST_PROGS)
	$(foreach test_prog,$(TEST_PROGS),./$(test_prog))

.PHONY: coverage
coverage: $(COV_PROGS)
	$(foreach cov_prog,$(COV_PROGS),./$(cov_prog);)
	$(foreach cov_file,$(wildcard ../*.gcda), gcov $(cov_file))

.PHONY: clean
clean:
	rm -rf $(TEST_PROGS) $(COV_OBJS) $(COV_PROGS) *.gcno *.gcda *.gcov
