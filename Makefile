CPP=g++
RELEASE_OPTS=-O2
DEBUG_OPTS=-g
CPPFILES=$(wildcard *.cpp)
HEADERS=$(wildcard *.h)
BINARY=checker

all: debug

release: $(CPPFILES) $(HEADERS)
	$(CPP) $(RELEASE_OPTS) $(CPPFILES) -o $(BINARY) 

debug:
	$(CPP) $(DEBUG_OPTS) $(CPPFILES) -o $(BINARY) 

.PHONY: release debug all
