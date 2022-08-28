LIBS=-lGL -lm -lpthread -ldl -lrt -lX11 -lraylib
CXXFLAGS=-std=c++2a -Wall -pedantic -Wformat -Werror

BIN=oucher
SRC=src/oucher.cpp
OBJ=$(addsuffix .o,$(basename $(SRC)))

EDITOR_BIN=editor
EDITOR_SRC=src/editor.cpp
EDITOR_OBJ=$(addsuffix .o,$(basename $(EDITOR_SRC)))

all: executable

debug: CXXFLAGS += -DDEBUG -g
debug: executable

executable: $(OBJ)
	$(CXX) -o $(BIN) $^ $(CXXFLAGS) $(LIBS)

editor: $(EDITOR_OBJ)
	$(CXX) -o $(EDITOR_BIN) $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f ./src/*.o
	rm -f ./src/*.out
	rm -f ./$(BIN)
	rm -f ./$(EDITOR_BIN)
