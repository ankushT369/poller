CC = gcc
CXX = g++
CFLAGS = -I./src
CXXFLAGS = -I./src
LDFLAGS = -lyaml-cpp

SRC_C = src/main.c
SRC_CPP = src/yaml_parser.cpp

OBJ = build/main.o build/yaml_parser.o

all: build/poller

build/main.o: $(SRC_C) | build
	$(CC) $(CFLAGS) -c $< -o $@

build/yaml_parser.o: $(SRC_CPP) | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/poller: $(OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

build:
	mkdir -p build

clean:
	rm -rf build/*

