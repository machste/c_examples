# List of all applications
APPS = encode64 decode64

# List of all source files
SRCS = base64.c

# Define compiler flags here
CFLAGS = -g -O0 -Werror

# External packages
PKGS = glib-2.0
# Define additional paths for pkg-config
PKG_CONFIG_PATH =

# Test report file
TEST_REPORT = test.log

# Folder for automatic header file dependency rules
DEP_DIR = .cdeps

# Compiler flags needed to build the test coverage applications
COV_FLAGS = -fprofile-arcs -ftest-coverage

# Generate a list of all test applications
TEST_APPS := $(patsubst %.c,%,$(wildcard tests/*.c))

# Generate a list of applications to examine the test coverage
TEST_COV_APPS := $(TEST_APPS:%=%.cov)

# Generate a list of all object files
OBJS := $(SRCS:%.c=%.o)

# Generate a list object files needed to build the test coverage applications
COV_OBJS := $(OBJS:%.o=%.cov.o)

# Define 'pkg-config' command
PKG_CONFIG := pkg-config
ifneq ($(PKG_CONFIG_PATH),)
	PKG_CONFIG := PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) $(PKG_CONFIG)
endif
# Calculate package include directories and libraries with 'pkg-config'
ifneq ($(PKGS),)
	INCLUDE_DIRS := $(shell $(PKG_CONFIG) --cflags $(PKGS))
	LDLIBS := $(shell $(PKG_CONFIG) --libs $(PKGS))
endif

# Flags for calculating header dependencies
DEP_FLAGS = -MT $(@) -MMD -MP -MF $(DEP_DIR)/$*.Td

# Define the compile command
COMPILE.c = $(CC) $(DEP_FLAGS) $(CFLAGS) $(CPPFLAGS) $(INCLUDE_DIRS) \
		$(TARGET_ARCH) -c
# Define the link command
LINK.c = $(CC) $(DEP_FLAGS) $(CFLAGS) $(CPPFLAGS) $(INCLUDE_DIRS) $(LDFLAGS) \
		$(TARGET_ARCH)
# Define post compile command
POSTCOMPILE.c = @mv -f $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d && touch $(@)

# Define the gcov command
GCOV := gcov

all: $(APPS)
.PHONY: all

$(APPS): %: %.c $(OBJS)
$(TEST_APPS): %: %.c $(OBJS)
	$(LINK.c) $^ $(LOADLIBS) $(LDLIBS) -o $(@)
	$(POSTCOMPILE.c)

$(TEST_COV_APPS): %.cov: %.c $(COV_OBJS)
	$(LINK.c) $(COV_FLAGS) $^ $(LOADLIBS) $(LDLIBS) -o $(@)
	$(POSTCOMPILE.c)

%.o : %.c
%.o : %.c $(DEP_DIR)/%.d
	$(COMPILE.c) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE.c)

%.cov.o: %.c $(DEP_DIR)/%.d
	$(COMPILE.c) $(COV_FLAGS) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE.c)

$(DEP_DIR):
	mkdir -p $(DEP_DIR)/tests

.PRECIOUS: $(DEP_DIR)/%.d
$(DEP_DIR)/%.d: $(DEP_DIR) ;

include $(wildcard $(patsubst %,$(DEP_DIR)/%.d,$(basename $(SRCS))))

$(TEST_REPORT): $(TEST_APPS)
	@rm -rf $(@)
	$(foreach test_app,$(TEST_APPS),@./$(test_app) | tee $(@))

.PHONY: coverage
coverage: $(TEST_COV_APPS)
	$(foreach test_cov_app,$(TEST_COV_APPS),./$(test_cov_app);)
	@for cov_file in *.gcda; do $(GCOV) $$cov_file; done

clean: clean-objs clean-deps clean-tests clean-coverage
	rm -rf $(APPS)

clean-objs:
	rm -rf $(OBJS)

clean-deps:
	rm -rf $(DEP_DIR)

clean-tests:
	rm -rf $(TEST_APPS) $(TEST_REPORT)

clean-coverage:
	rm -rf $(TEST_COV_APPS) $(COV_OBJS) *.gcno *.gcda *.gcov

.PHONY: clean-objs clean-deps clean-tests clean-coverage
