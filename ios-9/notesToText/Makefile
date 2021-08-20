# MIT License
# Copyright 2017 Patrick Laughrea

OPTIONS = -std=c++11 -Wall -Wextra -Wno-missing-field-initializers -iquote ..
LIBRARY = libnotesToText.a
CPP_FILES = $(wildcard *.cpp)
OBJ_FILES = $(notdir $(CPP_FILES:.cpp=.o))

all: $(LIBRARY)

$(LIBRARY): $(OBJ_FILES)
	ar -rcs $@ $^

%.o: %.cpp
	$(CXX) $(OPTIONS) -o $@ -c $<

clean:
	rm -rf $(LIBRARY) *.o

op: OPTIONS += -O3 -DNDEBUG
op: all