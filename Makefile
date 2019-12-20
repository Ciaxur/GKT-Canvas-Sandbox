# COMPILER OTHER INFO
CC 			= g++
FLAGS 		= `pkg-config gtkmm-3.0 --cflags --libs`
SRC_DIR 	= src

# COMPILING ALL
FILES 		= *.cc
OUT 		= app

# SHARED LIBRARY VARS (aside from main)
LIB_FILES 	= ContextArea.cc MyWindow.cc
OBJ_FILES 	= ContextArea.o MyWindow.o
OBJ_OUT   	= lib

# GENERATE PATHS
LIB_PATH	= $(patsubst %,../$(SRC_DIR)/%,$(LIB_FILES))



# BUILDS EVERYTHING INTO BINARY FILE #
build-all:
	$(CC) $(SRC_DIR)/*.cc -o $(OUT) $(FLAGS)


# BUILDS MAIN FILE USING OBJECT FILES #
build-with-SharedLibraries:
	$(CC) $(SRC_DIR)/main.cc $(OBJ_OUT)/*.o -o $(OUT) $(FLAGS)


# BUILD SHARED LIBRARIES IN OBJ_OUT #
generate-SharedLibraries:
	mkdir -p $(OBJ_OUT)
	cd $(OBJ_OUT); $(CC) -c $(LIB_PATH) $(FLAGS)

# REMOVES COMPILED BINARY #
clean:
	rm $(OUT)

# REMOVES COMPILES BINARY AND OBJECT FILES #
clean-all:
	rm $(OUT) $(OBJ_OUT)/*.o
