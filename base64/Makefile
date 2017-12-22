# List of all applications
APPS = encode64 decode64

# List of all source files
SRCS = base64.c

# Define your CFLAGS here
CFLAGS = -g -O0 -Werror

# External packages
PKGS = glib-2.0
# Define additional paths for pkg-config
PKG_CONFIG_PATH =

# Test report file
TEST_REPORT = test.log

# Folder for automatic header file dependency rules
DEP_DIR = .cdeps

# Generate a list of all test applications
TEST_APPS := $(patsubst %.c,%,$(wildcard tests/*.c))

# Generate a list of all object files
OBJS := $(SRCS:%.c=%.o)

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

all: $(APPS)
.PHONY: all

$(APPS): %: %.c $(OBJS)
$(TEST_APPS): %: %.c $(OBJS)
	$(LINK.c) $^ $(LOADLIBS) $(LDLIBS) -o $(@)
	$(POSTCOMPILE.c)

%.o : %.c
%.o : %.c $(DEP_DIR)/%.d
	$(COMPILE.c) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE.c)

$(DEP_DIR):
	mkdir -p $(DEP_DIR)/tests

$(DEP_DIR)/%.d: $(DEP_DIR) ;
.PRECIOUS: $(DEP_DIR)/%.d

$(TEST_REPORT): $(TEST_APPS)
	@rm -rf $(@)
	$(foreach test_app,$(TEST_APPS),@./$(test_app) | tee $(@))

include $(wildcard $(patsubst %,$(DEP_DIR)/%.d,$(basename $(SRCS))))

clean: clean-objs clean-deps clean-tests clean-coverage
	rm -rf $(APPS)

clean-objs:
	rm -rf $(OBJS)

clean-deps:
	rm -rf $(DEP_DIR)

clean-tests:
	rm -rf $(TEST_APPS) $(TEST_REPORT)

clean-coverage:
	rm -rf *.gcno *.gcda *.gcov

.PHONY: clean-objs clean-deps clean-tests clean-coverage
