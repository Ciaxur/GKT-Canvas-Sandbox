CC = g++
FLAGS = `pkg-config gtkmm-3.0 --cflags --libs`
SRC_DIR = src
FILES = *.cc
OUT = app

build:
	$(CC) $(SRC_DIR)/*.cc -o $(OUT) $(FLAGS)

clean:
	rm $(OUT)