# Shared libraries with GCC on Linux
#
# This example shall give a quick introduction to compile and use shared
# libraries.
#
# URL: https://www.cprogramming.com/tutorial/shared-libraries-linux-gcc.html

all: prog

# Step 1: Compiling with Position Independent Code
#
# We compile our library source code into position-independent code (PIC)
# 
shared.o: shared.c shared.h
	gcc -c -Wall -Werror -fpic shared.c

# Step 2: Creating a shared library from an object file
#
# Now we need to actually turn this object file into a shared library.
libshared.so: shared.o
	gcc -shared -o libshared.so shared.o

# Step 3: Linking with a shared library
#
# We are using rpath in this example. Now let's try rpath (first we'll clear
# LD_LIBRARY_PATH to ensure it's rpath that's finding our library). Rpath, or
# the run path, is a way of embedding the location of shared libraries in the
# executable itself, instead of relying on default locations or environment
# variables. We do this during the linking stage. Notice the lengthy
# "-Wl,-rpath=." option. The -Wl portion sends comma-separated options to the
# linker, so we tell it to send the -rpath option to the linker with our
# working directory.
prog: prog.c libshared.so
	gcc -L. -Wall -Werror -Wl,-rpath=. -o prog prog.c -lshared

.PHONY: clean
clean:
	rm -rf *.o *.so prog
