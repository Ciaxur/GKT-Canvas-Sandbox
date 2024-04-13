# COMPILER OTHER INFO
CC 			    = g++
FLAGS 		  = `pkg-config gtkmm-3.0 --cflags --libs`
SRC_DIR 	  = src
INCLUDE_DIR = include

# COMPILING ALL
FILES 		= *.cc
OUT 		  = app

# SPDLOG STUFF
SPDLOG_GIT = vendor/spdlog.git

# SHARED LIBRARY VARS (aside from main)
OBJ_OUT   	= lib

# GENERATE PATHS
INCLUDES  = "-I$(INCLUDE_DIR)"

# BUILDS EVERYTHING INTO BINARY FILE #
build: libspdlog.a ContextArea.o MyWindow.o
	$(CC) $(INCLUDES) $(SRC_DIR)/main.cc *.a *.o -o $(OUT) $(FLAGS)

# Builds the spdlog shared library.
libspdlog.a:
	cd $(SPDLOG_GIT); cd build; cmake ..; make -j 4; mv $(PWD)/$(SPDLOG_GIT)/build/libspdlog.a $(PWD)/.

ContextArea.o:
	$(CC) $(FLAGS) $(INCLUDES) $(SRC_DIR)/ContextArea.cc -c -o ContextArea.o

MyWindow.o:
	$(CC) $(FLAGS) $(INCLUDES) $(SRC_DIR)/MyWindow.cc -c -o MyWindow.o

# REMOVES COMPILED BINARY #
clean:
	rm $(OUT)

# REMOVES PRE-COMPILED PROJECT LIBRARIES #
clean-obj:
	rm *.o

# REMOVES COMPILES BINARY AND OBJECT FILES #
clean-all: clean-obj
	rm $(OUT) *.a
