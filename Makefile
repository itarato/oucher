LIBS=-lGL -lm -lpthread -ldl -lrt -lX11 -lraylib
CXXFLAGS=-std=c++2a -Wall -pedantic -Wformat -Werror
BIN=oucher
SRC=$(wildcard src/*.cpp)
OBJ=$(addsuffix .o,$(basename $(SRC)))

all: executable

debug: CXXFLAGS += -DDEBUG -g
debug: executable

executable: $(OBJ)
	$(CXX) -o $(BIN) $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f ./src/*.o
	rm -f ./src/*.out

clean_all:
	rm -f ./src/*.o
	rm -f ./src/*.out
	rm -f ./$(BIN)
