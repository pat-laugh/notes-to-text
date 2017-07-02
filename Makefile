# MIT License
# Copyright 2017 Patrick Laughrea

DIR_NOTES_TO_TEXT = ./lib
DIR_VARIOUS = ./various

OPTIONS = -std=c++11 -Wall -Wextra -Wno-missing-field-initializers -iquote .
CPP_FILES = $(wildcard *.cpp)
OBJ_FILES = $(notdir $(CPP_FILES:.cpp=.o))
EXEC = notes-to-text.out

all: COMPILE_LIB $(EXEC)

COMPILE_LIB:
	cd $(DIR_NOTES_TO_TEXT) && make -s

$(EXEC): $(OBJ_FILES)
	$(CXX) -o $@ $^ -L$(DIR_NOTES_TO_TEXT) -lnotesToText -L$(DIR_VARIOUS) -lvarious

%.o: %.cpp
	$(CXX) $(OPTIONS) -o $@ -c $<

clean:
	cd $(DIR_NOTES_TO_TEXT) && make clean -s
	rm -rf $(EXEC) *.o