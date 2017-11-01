# Makefile for use with emacs and emacs's flymake mode
# Copyright (C) raodm@miamiOH.edu
#
# From emacs menu select Tools->compile
# To compile one file use the syntax:
#     make SRC=hello.c
# The compile many files use the syntax (change my_exe_name below):
#     make many EXE=my_exe_name
#

.PHONY: check-syntax all clean style many style-many

CXX=g++
CXXFLAGS=-Wall -g -Wextra -D_FILE_OFFSET_BITS=64 -I/usr/include/fuse -pthread -I/usr/include/mysql -I/usr/local/include/mysql++
LIBS=-lm -lfuse -lcurl -lexpat -lmysqlpp -lmysqlclient
LDFLAGS := -L/usr/local/lib

# Target exectuable name if SRC is defined
ifdef SRC
OBJ=$(patsubst %.cpp, %.o, $(SRC))
EXE=$(patsubst %.cpp, %,   $(SRC))
endif

	
build: $(SRC)
ifeq (,$(findstring .h, $(SRC)))
	$(CXX) $(CXXFLAGS) $(SRC) -o $(EXE) $(LIBS)
endif


%.o: %.c
	$(CXX) -c $(CXXFLAGS) $(SRC) -o $(EXE) $(LIBS)

fusedb:	fusedb.o movies.o
	$(CXX)  $(CXXFLAGS) fusedb.o movies.o -o fusedb $(LIBS)
	

