CC = gcc
CXX = g++
CFLAGS = -I./src
CXXFLAGS = -I./src
LDFLAGS = -lyaml-cpp
ZLFLAGS = -lzlog
PTFLAGS = -lpthread

SRC_C = src/main.c src/db_adapter.c src/pg_adapter.c src/log_conf.c
SRC_CPP = src/yaml_parser.cpp

OBJ_C = $(patsubst src/%.c, build/%.o, $(SRC_C))
OBJ_CPP = $(patsubst src/%.cpp, build/%.o, $(SRC_CPP))
OBJ = $(OBJ_C) $(OBJ_CPP)

all: build/poller

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/poller: $(OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS) $(ZLFLAGS) $(PTFLAGS)

build:
	mkdir -p build

clean:
	rm -rf build/*
